#include "sct_plots.h"

#include "treeCollection.h"
#include <iostream>
#include "cluster.h"
#include "TCanvas.h"
#include "TVector.h"
#include "TMath.h"
#include "sct_plots_internal.h"
#include "TH2.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TH1.h"
#include "TTree.h"
#include "s_cuts.h"
#include "s_plot_collection.h"
#include "s_plane.h"
#include "s_DrawOption.h"


#include "s_process_files.h"

#include "TF1.h"
#include "TProfile.h"











void SCT_helpers::CutTH2(TH2* h, const S_Cut& cut_)
{

  if (!h)
  {
    std::cout << "Histogram is empty\n";
    return;
  }
  Double_t x = 0, y = 0, binContent = 0;

  for (Int_t y_bin = 0; y_bin <= h->GetNbinsY(); ++y_bin)
  {
    for (Int_t x_bin = 0; x_bin <= h->GetNbinsX(); ++x_bin)
    {
      x = h->GetXaxis()->GetBinCenter(x_bin);
      auto bin = h->GetBin(x_bin, y_bin);
      y = h->GetYaxis()->GetBinCenter(y_bin);
      binContent = h->GetBinContent(bin);


      if (cut_.isOutOfRange(binContent, x, y))
      {
        h->SetBinContent(bin, 0);
      }
    }

  }
}

void SCT_helpers::CutTH1(TH1* h1, const S_Cut& cut_)
{
  Double_t x = 0, binContent = 0;


  for (Int_t x_bin = 0; x_bin <= h1->GetNbinsX(); ++x_bin)
  {
    x = h1->GetXaxis()->GetBinCenter(x_bin);
    auto bin = h1->GetBin(x_bin);

    binContent = h1->GetBinContent(bin);


    if (cut_.isOutOfRange(binContent, x))
    {
      h1->SetBinContent(bin, 0);
    }
  }


}

TH1* SCT_helpers::HistogrammSilhouette(TH2* h2, axis_def ax)
{

  if (ax == x_axis_def)
  {

    std::string title = std::string(h2->GetTitle()) + std::string("_sil_x");
    std::string name = std::string(h2->GetName()) + std::string("_sil_x");
    TH1* ret = new TH1D(name.c_str(), title.c_str(), h2->GetNbinsX(), h2->GetXaxis()->GetBinCenter(0), h2->GetXaxis()->GetBinCenter(h2->GetNbinsX()));


    for (Int_t x_bin = 0; x_bin <= h2->GetNbinsX(); ++x_bin)
    {
      Double_t max_binContent = 0;
      for (Int_t y_bin = 0; y_bin <= h2->GetNbinsY(); ++y_bin)
      {


        auto bin = h2->GetBin(x_bin, y_bin);
        max_binContent = std::max(h2->GetBinContent(bin), max_binContent);

      }

      ret->SetBinContent(x_bin, max_binContent);
    }

    return ret;
  }
  else if (ax == y_axis_def){

    std::string title = std::string(h2->GetTitle()) + std::string("_sil_y");
    std::string name = std::string(h2->GetName()) + std::string("_sil_y");
    TH1* ret = new TH1D(name.c_str(), title.c_str(), h2->GetNbinsY(), h2->GetYaxis()->GetBinCenter(0), h2->GetYaxis()->GetBinCenter(h2->GetNbinsY()));

    for (Int_t y_bin = 0; y_bin <= h2->GetNbinsY(); ++y_bin)
    {
      Double_t max_binContent = 0;
      for (Int_t x_bin = 0; x_bin <= h2->GetNbinsX(); ++x_bin)
      {




        auto bin = h2->GetBin(x_bin, y_bin);
        max_binContent = std::max(h2->GetBinContent(bin), max_binContent);

      }

      ret->SetBinContent(y_bin, max_binContent);
    }

    return ret;
  }
  std::cout << "[SCT_helpers::HistogrammSilhouette] Unknown axis \n";
  return 0;
}



Long64_t SCT_helpers::DrawTTree(TTree * tree, const S_DrawOption& opt)
{
  return opt.Draw(tree);
}

TF1 SCT_helpers::LinearFit_Of_Profile(TH2D* h2)
{
TProfile* p1 = h2->ProfileX();
TF1 f("f1", "pol1", h2->GetXaxis()->GetBinCenter(0), h2->GetXaxis()->GetBinCenter(h2->GetNbinsX()));

p1->Fit(&f,"Q");
return f;
}

s_plane_collection_find_closest sct_plot::find_nearest(S_plot_collection& pl, Double_t x_cutoff, Double_t y_cutoff, const S_plane_def& planeA, const S_plane_def& planeB, const s_plot_prob&  plot_prob_) {
  auto collection_= pl.addPlot(sct_plot::find_nearest(x_cutoff, y_cutoff, plot_prob_), planeA, planeB);

  s_plane_collection_find_closest ret;
  ret.setResidual(collection_.getByType("nearest_distance")());
  ret.setHitOnPlaneA(collection_.getByType("nearest_plane1")());
  ret.setHitOnPlaneB(collection_.getByType("nearest_plane2")());
  return ret;
}

s_plane_collection_find_closest sct_plot::find_nearest_strip(S_plot_collection& pl, axis_def search_axis, Double_t cutOfff, const S_plane_def& planeA, const S_plane_def& planeB, const s_plot_prob& plot_prob_/*= ""*/) {
  auto collection_ = pl.addPlot(sct_plot::find_nearest_strip(search_axis,cutOfff, plot_prob_), planeA, planeB);

  s_plane_collection_find_closest ret;
  ret.setResidual(collection_.getByType("nearest_strip_distance")());
  ret.setHitOnPlaneA(collection_.getByType("nearest_strip_plane1")());
  ret.setHitOnPlaneB(collection_.getByType("nearest_strip_plane2")());
  return ret;
}

s_plane_collection sct_plot::misalignment_strip(S_plot_collection& pl, S_plane_def fitted_plane, S_plane_def plane_hit_, axis_def Unknown_axis, const s_plot_prob& plot_prob)
{
  auto apix_true_hits = pl.addPlot(sct_plot::find_nearest(0.1, 0.2, s_plot_prob().doNotSaveToDisk()), sct_coll::apix_fitted(), sct_coll::apix_hit()).get(1);

  auto dut_fitted_trackts = pl.addPlot(sct_plot::find_nearest(1, 1, s_plot_prob().doNotSaveToDisk()), fitted_plane, apix_true_hits).get(1);



  axis_def search_axis;
  if (Unknown_axis == y_axis_def)
  {
    search_axis = x_axis_def;
  }
  else
  {
    search_axis = y_axis_def;
  }

  auto res = pl.addPlot(sct_plot::find_nearest_strip(search_axis, 100, s_plot_prob().doNotSaveToDisk()), plane_hit_, dut_fitted_trackts);

  if (Unknown_axis == y_axis_def)
  {
    return pl.addPlot(sct_plot::hitmap(), res.get(2).getY_def(), res.get(0).getX_def());
  }
  else
  {
    return pl.addPlot(sct_plot::hitmap(), res.get(2).getX_def(), res.get(0).getY_def());

  }


}

s_plane_collection sct_plot::misalignment_pixel(S_plot_collection& pl, S_plane_def fitted_plane, S_plane_def plane_hit_, const s_plot_prob& plot_prob/*= ""*/)
{
  auto apix_true_hits = pl.addPlot(sct_plot::find_nearest(0.1, 0.2, s_plot_prob().doNotSaveToDisk()), sct_coll::apix_fitted(), sct_coll::apix_hit()).get(1);

  auto dut_fitted_trackts = pl.addPlot(sct_plot::find_nearest(1, 1, s_plot_prob().doNotSaveToDisk()), fitted_plane, apix_true_hits);

  auto res = pl.addPlot(sct_plot::find_nearest(100, 100, s_plot_prob().doNotSaveToDisk()), plane_hit_, dut_fitted_trackts.get(1));

  return pl.addPlot(sct_plot::hitmap(), res.get(2).getX_def(), res.get(0).getY_def()) + pl.addPlot(sct_plot::hitmap(), res.get(2).getY_def(), res.get(0).getX_def());

}



S_plane_def sct_plot::Crate_True_Fitted_DUT_Hits(S_plot_collection& pl, const s_plot_prob& plot_p/*= ""*/)
{
  auto apix_true_hits = pl.addPlot(sct_plot::find_nearest(0.1, 0.2, s_plot_prob().doNotSaveToDisk()), sct_coll::apix_fitted(), sct_coll::apix_hit()).get(1);

  auto dut_fitted_trackts = pl.addPlot(sct_plot::find_nearest(1, 1, plot_p), sct_coll::DUT_fitted(), apix_true_hits).get(1);

  return dut_fitted_trackts;
}

S_plane_def sct_plot::Create_True_Fitted_DUT_Hits_in_channels(S_plot_collection& pl, double pitchSize, double rotate, double move_x, double move_y, const s_plot_prob& plot_p/*= ""*/)
{
  auto trueHits = Crate_True_Fitted_DUT_Hits(pl, s_plot_prob().doNotSaveToDisk());
  auto dut_rotated_17 = pl.addPlot(sct_plot::rotated(rotate, s_plot_prob().doNotSaveToDisk()), trueHits);

  auto projected_strip_rotated = pl.addPlot(sct_plot::rotated(-TMath::Pi() / 2), dut_rotated_17());

  auto fitted_in_channels = pl.addPlot(sct_plot::coordinate_transform(1 / pitchSize, move_x , 1, move_y, plot_p), projected_strip_rotated());
  return fitted_in_channels();
}

s_plane_collection_correlations sct_plot::Create_Correlations_of_true_Fitted_hits_with_DUT_Hits_in_channels(S_plot_collection& pl, double pitchSize, double rotate, double move_x, double move_y, const S_Cut& fiducial_cut_, double residualCut, const s_plot_prob& /*= ""*/) {

  auto truehits = sct_plot::Create_True_Fitted_DUT_Hits_in_channels(pl, pitchSize, rotate, move_x, move_y, s_plot_prob().doNotSaveToDisk());

  auto trueHits_cut = pl.addPlot(sct_plot::cut_x_y(fiducial_cut_), truehits);
  auto cor = pl.addPlot(sct_plot::find_nearest_strip(x_axis_def, residualCut, s_plot_prob()), sct_coll::DUT_zs_data(), trueHits_cut());
  s_plane_collection hitmap__rot = pl.addPlot(sct_plot::hitmap(), cor.get("nearest_strip_distance")().getX_def(), cor.get("nearest_strip_plane2")().getY_def());
  s_plane_collection_correlations ret;
  ret.setResidual(cor.get("nearest_strip_distance")());
  ret.setResidualVsMissing(hitmap__rot());
  ret.setTrueHitsWithDUT(cor.get("nearest_strip_plane2")());
  ret.setTotalTrueHits(trueHits_cut());
  ret.setDUT_Hits(cor.get("nearest_strip_plane1")());
  return ret;
}

s_plane_collection_correlations sct_plot::GBL_Create_Correlations_of_true_Fitted_hits_with_DUT_Hits_in_channels(S_plot_collection& pl, double pitchSize, double rotate, double move_x, double move_y, const S_Cut& fiducial_cut_, double residualCut, const s_plot_prob& plot_prob_ /*= ""*/) {


  auto apix_transform = pl.addPlot(sct_plot::coordinate_transform(1 / 3.98148, -156.98 + 118.43, 1 / -19.4048, -34.0894, s_plot_prob().doNotSaveToDisk()), sct_coll::apix_zs_data());


  auto fitted_true = sct_plot::find_nearest(pl, 1, 3, sct_coll::DUT_fitted_local_GBL(), apix_transform(), s_plot_prob().doNotSaveToDisk());

  // auto trueHits = pl.addPlot(sct_plot::coordinate_transform(13.3934 / 1.00377, 669.918 - 1.75912 + 0.404092+0.01228, 1, 0, s_plot_prob().SaveToDisk()), sct_coll::DUT_fitted_local_GBL());

  auto trueHits_cut = pl.addPlot(sct_plot::cut_x_y(fiducial_cut_, s_plot_prob().doNotSaveToDisk()), fitted_true.getHitOnPlaneA());
  std::string true_hits_name= std::string(plot_prob_.getName()) + "_true_hits";
  auto trueHits = pl.addPlot(sct_plot::coordinate_transform(1 / pitchSize, move_x, 1, move_y, s_plot_prob(true_hits_name.c_str()).setSaveOptione(plot_prob_.getPlotSaveOption())), trueHits_cut());


  std::string find_closest_name = std::string(plot_prob_.getName()) + "_closest";
  auto find_closest = sct_plot::find_nearest_strip(pl, x_axis_def, 100, trueHits(), sct_coll::DUT_zs_data(), s_plot_prob(find_closest_name.c_str()).setSaveOptione(plot_prob_.getPlotSaveOption()));
  

  std::string res_vs_missing_name = std::string(plot_prob_.getName()) + "_res_vs_missing";
  auto res_vs_missing = pl.addPlot(sct_plot::hitmap(s_plot_prob( res_vs_missing_name.c_str()).setSaveOptione(plot_prob_.getPlotSaveOption())), find_closest.getResidual().getX_def(), find_closest.getHitOnPlaneA().getY_def());


  s_plane_collection_correlations ret;
  ret.setResidual(find_closest.getResidual());
  ret.setResidualVsMissing(res_vs_missing());
  ret.setTotalTrueHits(trueHits());
  ret.setTrueHitsWithDUT(find_closest.getHitOnPlaneA());
  ret.setDUT_Hits(find_closest.getHitOnPlaneB());
  return ret;

}


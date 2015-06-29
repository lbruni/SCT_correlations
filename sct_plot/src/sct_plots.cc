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














void SCT_helpers::CutTH2(TH2* h, const S_Cut& cut_)
{

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

S_plane_def sct_plot::Crate_True_Fitted_DUT_Hits_in_channels(S_plot_collection& pl, double pitchSize, double rotate, double move_x, double move_y, const s_plot_prob& plot_p/*= ""*/)
{
  auto trueHits = Crate_True_Fitted_DUT_Hits(pl, s_plot_prob().doNotSaveToDisk());
  auto dut_rotated_17 = pl.addPlot(sct_plot::rotated(rotate, s_plot_prob().doNotSaveToDisk()), trueHits);
  auto dut_rot_moved = pl.addPlot(sct_plot::coordinate_transform_move(move_x, move_y, s_plot_prob().doNotSaveToDisk()), dut_rotated_17());
  auto projected_strip_rotated = pl.addPlot(sct_plot::rotated(-TMath::Pi() / 2), dut_rot_moved());

  auto fitted_in_channels = pl.addPlot(sct_plot::coordinate_transform(1 / pitchSize, 317.046 - 8.07124e-001, 1, 0, plot_p), projected_strip_rotated());
  return fitted_in_channels();
}

s_plot_prob::s_plot_prob()
{

}

s_plot_prob::s_plot_prob(const char* name) :m_name(name)
{

}

s_plot_prob& s_plot_prob::setName(const char* name)
{
  m_name = name;
  return *this;
}

s_plot_prob& s_plot_prob::setSaveOptione(plot_save_option_def save_)
{
  m_save = save_;
  return *this;
}

s_plot_prob& s_plot_prob::doNotSaveToDisk()
{
  return setSaveOptione(do_not_save_to_disk);
}

s_plot_prob& s_plot_prob::SaveToDisk()
{
  return setSaveOptione(save_to_disk);
}

const char* s_plot_prob::getName() const
{
  return m_name.c_str();
}

bool s_plot_prob::getNameSize() const
{
  return m_name.size();
}

plot_save_option_def s_plot_prob::getPlotSaveOption() const
{
  return m_save;
}

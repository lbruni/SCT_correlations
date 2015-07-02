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
#include "sct_runs.h"
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

void sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_150V(s_process_files& p, std::string path__, std::string outputPath)
{
  outputPath+="DEVICE_1_ASIC_on_Position_7_Jim_150V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0171112);
  p.push_files((path__ + std::string("run000823-fitter.root")).c_str(), 20, 823, 150);

  p.push_files((path__ + std::string("run000824-fitter.root")).c_str(), 35, 824, 150);
  p.push_files((path__ + std::string("run000704-fitter.root")).c_str(), 50, 704, 150);
  p.push_files((path__ + std::string("run000705-fitter.root")).c_str(), 65, 705, 150);
  p.push_files((path__ + std::string("run000706-fitter.root")).c_str(), 80, 706, 150);
  p.push_files((path__ + std::string("run000707-fitter.root")).c_str(), 95, 707, 150);
  p.push_files((path__ + std::string("run000708-fitter.root")).c_str(), 110, 708, 150);

  p.push_files((path__ + std::string("run000709-fitter.root")).c_str(), 125, 709, 150);

  p.push_files((path__ + std::string("run000710-fitter.root")).c_str(), 140, 710, 150);
  p.push_files((path__ + std::string("run000711-fitter.root")).c_str(), 155, 711, 150);

  p.push_files((path__ + std::string("run000712-fitter.root")).c_str(), 170, 712, 150);
  p.push_files((path__ + std::string("run000713-fitter.root")).c_str(), 185, 713, 150);

  p.push_files((path__ + std::string("run000714-fitter.root")).c_str(), 200, 714, 150);

  p.push_files((path__ + std::string("run000715-fitter.root")).c_str(), 215, 715, 150);
  p.push_files((path__ + std::string("run000716-fitter.root")).c_str(), 230, 716, 150);
  p.push_files((path__ + std::string("run000717-fitter.root")).c_str(), 245, 717, 150);




}

void sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_400V(s_process_files& p, std::string path__, std::string outputPath)
{

  outputPath+="DEVICE_1_ASIC_on_Position_7_Jim_400V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0171112);
  p.push_files((path__ + std::string("run000811-fitter.root")).c_str(), 20, 811, 400);

  p.push_files((path__ + std::string("run000812-fitter.root")).c_str(), 35, 812, 400);
  p.push_files((path__ + std::string("run000797-fitter.root")).c_str(), 50, 797, 400);
  p.push_files((path__ + std::string("run000798-fitter.root")).c_str(), 65, 798, 400);
  p.push_files((path__ + std::string("run000799-fitter.root")).c_str(), 80, 799, 400);
  p.push_files((path__ + std::string("run000800-fitter.root")).c_str(), 95, 800, 400);
  p.push_files((path__ + std::string("run000801-fitter.root")).c_str(), 110, 801, 400);
  p.push_files((path__ + std::string("run000813-fitter.root")).c_str(), 110, 813, 400);
  p.push_files((path__ + std::string("run000802-fitter.root")).c_str(), 125, 802, 400);

  p.push_files((path__ + std::string("run000803-fitter.root")).c_str(), 140, 803, 400);
  p.push_files((path__ + std::string("run000804-fitter.root")).c_str(), 155, 804, 400);
  p.push_files((path__ + std::string("run000805-fitter.root")).c_str(), 170, 805, 400);
  p.push_files((path__ + std::string("run000814-fitter.root")).c_str(), 170, 814, 400);
  p.push_files((path__ + std::string("run000806-fitter.root")).c_str(), 185, 806, 400);

  p.push_files((path__ + std::string("run000807-fitter.root")).c_str(), 200, 807, 400);
  p.push_files((path__ + std::string("run000808-fitter.root")).c_str(), 215, 808, 400);
  p.push_files((path__ + std::string("run000815-fitter.root")).c_str(), 215, 815, 400);
  p.push_files((path__ + std::string("run000809-fitter.root")).c_str(), 230, 809, 400);
  p.push_files((path__ + std::string("run000810-fitter.root")).c_str(), 245, 810, 400);

}

void sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_250V(s_process_files& p, std::string path__, std::string outputPath)
{

  outputPath+="DEVICE_1_ASIC_on_Position_7_Jim_250V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0171112);
  p.push_files((path__ + std::string("run000825-fitter.root")).c_str(), 20, 825, 250);

  p.push_files((path__ + std::string("run000826-fitter.root")).c_str(), 35, 826, 250);
  p.push_files((path__ + std::string("run000718-fitter.root")).c_str(), 50, 718, 250);
  p.push_files((path__ + std::string("run000719-fitter.root")).c_str(), 65, 719, 250);
  p.push_files((path__ + std::string("run000720-fitter.root")).c_str(), 80, 720, 250);
  p.push_files((path__ + std::string("run000721-fitter.root")).c_str(), 95, 721, 250);
  p.push_files((path__ + std::string("run000722-fitter.root")).c_str(), 110, 722, 250);

  p.push_files((path__ + std::string("run000723-fitter.root")).c_str(), 125, 723, 250);
  p.push_files((path__ + std::string("run000724-fitter.root")).c_str(), 140, 724, 250);
  p.push_files((path__ + std::string("run000827-fitter.root")).c_str(), 140, 827, 250);
  p.push_files((path__ + std::string("run000725-fitter.root")).c_str(), 155, 725, 250);

  p.push_files((path__ + std::string("run000726-fitter.root")).c_str(), 170, 726, 250);
  p.push_files((path__ + std::string("run000727-fitter.root")).c_str(), 185, 727, 250);
  p.push_files((path__ + std::string("run000728-fitter.root")).c_str(), 200, 728, 250);
  p.push_files((path__ + std::string("run000826-fitter.root")).c_str(), 200, 826, 250);

  p.push_files((path__ + std::string("run000729-fitter.root")).c_str(), 215, 729, 250);
  p.push_files((path__ + std::string("run000730-fitter.root")).c_str(), 230, 730, 250);
  p.push_files((path__ + std::string("run000731-fitter.root")).c_str(), 245, 731, 250);



}
void sct_runs::DEVICE_1_ASIC_on_Position_7_Jim_350V(s_process_files& p, std::string path__, std::string outputPath)
{
  outputPath += "/DEVICE_1_ASIC_on_Position_7_Jim_350V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0171112);
//  p.push_files((path__ + std::string("run000816-fitter.root")).c_str(), 20, 816, 350); //broken file
  p.push_files((path__ + std::string("run000818-fitter.root")).c_str(), 20, 818, 350);
  p.push_files((path__ + std::string("run000817-fitter.root")).c_str(), 35, 817, 350);
  p.push_files((path__ + std::string("run000703-fitter.root")).c_str(), 50, 703, 350);
  p.push_files((path__ + std::string("run000688-fitter.root")).c_str(), 65, 688, 350);
  p.push_files((path__ + std::string("run000689-fitter.root")).c_str(), 80, 689, 350);
  p.push_files((path__ + std::string("run000690-fitter.root")).c_str(), 95, 690, 350);
  p.push_files((path__ + std::string("run000691-fitter.root")).c_str(), 110, 691, 350);

  p.push_files((path__ + std::string("run000692-fitter.root")).c_str(), 125, 692, 350);

  p.push_files((path__ + std::string("run000693-fitter.root")).c_str(), 140, 693, 350);
   p.push_files((path__ + std::string("run000694-fitter.root")).c_str(), 155, 694, 350);
 
   p.push_files((path__ + std::string("run000695-fitter.root")).c_str(), 170, 695, 350);
   p.push_files((path__ + std::string("run000696-fitter.root")).c_str(), 185, 696, 350);
 
   p.push_files((path__ + std::string("run000702-fitter.root")).c_str(), 200, 702, 350);
 
  // p.push_files((path__ + std::string("run000698-fitter.root")).c_str(), 215, 698, 350);
   p.push_files((path__ + std::string("run000699-fitter.root")).c_str(), 230, 699, 350);
   p.push_files((path__ + std::string("run000701-fitter.root")).c_str(), 245, 701, 350);

}

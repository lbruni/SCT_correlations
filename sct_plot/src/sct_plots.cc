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

  auto projected_strip_rotated = pl.addPlot(sct_plot::rotated(-TMath::Pi() / 2), dut_rotated_17());

  auto fitted_in_channels = pl.addPlot(sct_plot::coordinate_transform(1 / pitchSize, move_x , 1, move_y, plot_p), projected_strip_rotated());
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
  p.SetPosition(3.16097e+002, 0);
  //p.push_files((path__ + std::string("run000823-fitter.root")).c_str(), 20, 823, 150);

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
  p.SetPosition(3.16097e+002, 0);
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
 // p.push_files((path__ + std::string("run000815-fitter.root")).c_str(), 215, 815, 400);
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
  p.SetPosition(3.16097e+002, 0);
  p.push_files((path__ + std::string("run000825-fitter.root")).c_str(), 20, 825, 250);

  p.push_files((path__ + std::string("run000826-fitter.root")).c_str(), 35, 826, 250);
  p.push_files((path__ + std::string("run000718-fitter.root")).c_str(), 50, 718, 250);
  p.push_files((path__ + std::string("run000719-fitter.root")).c_str(), 65, 719, 250);
  p.push_files((path__ + std::string("run000720-fitter.root")).c_str(), 80, 720, 250);
  p.push_files((path__ + std::string("run000721-fitter.root")).c_str(), 95, 721, 250);
  p.push_files((path__ + std::string("run000722-fitter.root")).c_str(), 110, 722, 250);

  p.push_files((path__ + std::string("run000723-fitter.root")).c_str(), 125, 723, 250);
  p.push_files((path__ + std::string("run000724-fitter.root")).c_str(), 140, 724, 250);
  //p.push_files((path__ + std::string("run000827-fitter.root")).c_str(), 140, 827, 250);
 // p.push_files((path__ + std::string("run000725-fitter.root")).c_str(), 155, 725, 250);

 // p.push_files((path__ + std::string("run000726-fitter.root")).c_str(), 170, 726, 250);
  p.push_files((path__ + std::string("run000727-fitter.root")).c_str(), 185, 727, 250);
  p.push_files((path__ + std::string("run000728-fitter.root")).c_str(), 200, 728, 250);
  //p.push_files((path__ + std::string("run000826-fitter.root")).c_str(), 200, 826, 250);

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
  p.setActiveArea(20, 110);
  p.SetRotation(0.0171112);
  p.SetPosition(3.16097e+002, 0);
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

void sct_runs::DEVICE_1_ASIC_on_Position_5_150V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_1_ASIC_on_Position_5_150V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(20, 110);
  p.SetRotation(0.0181651);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run000890-fitter.root")).c_str(), 35, 890, 150);
  p.push_files((path__ + std::string("run000891-fitter.root")).c_str(), 50, 891, 150);
  p.push_files((path__ + std::string("run000892-fitter.root")).c_str(), 65, 892, 150);
  p.push_files((path__ + std::string("run000893-fitter.root")).c_str(), 80, 893, 150);
  p.push_files((path__ + std::string("run000894-fitter.root")).c_str(), 95, 894, 150);
  p.push_files((path__ + std::string("run000895-fitter.root")).c_str(), 110, 895, 150);
  p.push_files((path__ + std::string("run000896-fitter.root")).c_str(), 125, 896, 150);
  p.push_files((path__ + std::string("run000897-fitter.root")).c_str(), 140, 897, 150);
  p.push_files((path__ + std::string("run000898-fitter.root")).c_str(), 155, 898, 150);
  p.push_files((path__ + std::string("run000899-fitter.root")).c_str(), 170, 899, 150);
  p.push_files((path__ + std::string("run000900-fitter.root")).c_str(), 185, 900, 150);
  p.push_files((path__ + std::string("run000901-fitter.root")).c_str(), 200, 901, 150);
  p.push_files((path__ + std::string("run000902-fitter.root")).c_str(), 215, 902, 150);
  p.push_files((path__ + std::string("run000903-fitter.root")).c_str(), 230, 903, 150);
  //p.push_files((path__ + std::string("run000904-fitter.root")).c_str(), 245, 904, 150);



}


void sct_runs::DEVICE_1_ASIC_on_Position_5_250V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_1_ASIC_on_Position_5_250V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(20, 110);
  p.SetRotation(0.0181651);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run000871-fitter.root")).c_str(), 35, 871, 250);
  p.push_files((path__ + std::string("run000872-fitter.root")).c_str(), 50, 872, 250);
  p.push_files((path__ + std::string("run000873-fitter.root")).c_str(), 65, 873, 250);
  p.push_files((path__ + std::string("run000874-fitter.root")).c_str(), 80, 874, 250);
  p.push_files((path__ + std::string("run000875-fitter.root")).c_str(), 95, 875, 250);
  p.push_files((path__ + std::string("run000876-fitter.root")).c_str(), 110, 876, 250);
  p.push_files((path__ + std::string("run000877-fitter.root")).c_str(), 125, 877, 250);
  p.push_files((path__ + std::string("run000878-fitter.root")).c_str(), 140, 878, 250);
  p.push_files((path__ + std::string("run000879-fitter.root")).c_str(), 155, 879, 250);
  p.push_files((path__ + std::string("run000880-fitter.root")).c_str(), 170, 880, 250);
  p.push_files((path__ + std::string("run000881-fitter.root")).c_str(), 185, 881, 250);
  p.push_files((path__ + std::string("run000882-fitter.root")).c_str(), 200, 882, 250);
  p.push_files((path__ + std::string("run000883-fitter.root")).c_str(), 215, 883, 250);
  p.push_files((path__ + std::string("run000884-fitter.root")).c_str(), 230, 884, 250);
  p.push_files((path__ + std::string("run000885-fitter.root")).c_str(), 245, 885, 250);

}

void sct_runs::DEVICE_1_ASIC_on_Position_5_350V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_1_ASIC_on_Position_5_350V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(20, 110);
  p.SetRotation(0.0181651);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run000849-fitter.root")).c_str(), 35, 849, 350);
  p.push_files((path__ + std::string("run000850-fitter.root")).c_str(), 50, 850, 350);
  p.push_files((path__ + std::string("run000851-fitter.root")).c_str(), 65, 851, 350);
  p.push_files((path__ + std::string("run000852-fitter.root")).c_str(), 80, 852, 350);
  p.push_files((path__ + std::string("run000853-fitter.root")).c_str(), 95, 853, 350);
  p.push_files((path__ + std::string("run000854-fitter.root")).c_str(), 110, 854, 350);
  p.push_files((path__ + std::string("run000855-fitter.root")).c_str(), 125, 855, 350);
  p.push_files((path__ + std::string("run000856-fitter.root")).c_str(), 140, 856, 350);
  p.push_files((path__ + std::string("run000857-fitter.root")).c_str(), 155, 857, 350);
  p.push_files((path__ + std::string("run000858-fitter.root")).c_str(), 170, 858, 350);
  
  
  p.push_files((path__ + std::string("run000868-fitter.root")).c_str(), 185, 868, 350);
  
  
  p.push_files((path__ + std::string("run000869-fitter.root")).c_str(), 200, 869, 350);
  p.push_files((path__ + std::string("run000862-fitter.root")).c_str(), 215, 862, 350);
  p.push_files((path__ + std::string("run000863-fitter.root")).c_str(), 230, 863, 350);
  p.push_files((path__ + std::string("run000864-fitter.root")).c_str(), 245, 864, 350);



}

void sct_runs::DEVICE_1_ASIC_on_Position_5_400V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_1_ASIC_on_Position_5_400V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-4, 2));
  p.setActiveArea(20, 110);
  p.SetRotation(0.0181651);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run000834-fitter.root")).c_str(), 35, 834, 400);
  p.push_files((path__ + std::string("run000835-fitter.root")).c_str(), 50, 835, 400);
  p.push_files((path__ + std::string("run000836-fitter.root")).c_str(), 65, 836, 400);
  p.push_files((path__ + std::string("run000837-fitter.root")).c_str(), 80, 837, 400);
  p.push_files((path__ + std::string("run000865-fitter.root")).c_str(), 95, 865, 400);
  p.push_files((path__ + std::string("run000839-fitter.root")).c_str(), 110, 839, 400);
  p.push_files((path__ + std::string("run000840-fitter.root")).c_str(), 125, 840, 400);
  p.push_files((path__ + std::string("run000841-fitter.root")).c_str(), 140, 841, 400);
  p.push_files((path__ + std::string("run000866-fitter.root")).c_str(), 155, 866, 400);
  p.push_files((path__ + std::string("run000843-fitter.root")).c_str(), 170, 843, 400);
  p.push_files((path__ + std::string("run000844-fitter.root")).c_str(), 185, 844, 400);
  p.push_files((path__ + std::string("run000845-fitter.root")).c_str(), 200, 845, 400);
  p.push_files((path__ + std::string("run000867-fitter.root")).c_str(), 215, 867, 400);
  p.push_files((path__ + std::string("run000847-fitter.root")).c_str(), 230, 847, 400);
  p.push_files((path__ + std::string("run000848-fitter.root")).c_str(), 245, 848, 400);

}
void sct_runs::DEVICE_2_ASIC_on_Position_5_150V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_5_150V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(20, 110);
  p.SetRotation(0.00568813);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run001099-fitter.root")).c_str(), 20, 1099, 150);
  p.push_files((path__ + std::string("run001100-fitter.root")).c_str(), 35, 1100, 150);
  p.push_files((path__ + std::string("run001101-fitter.root")).c_str(), 50, 1101, 150);
  p.push_files((path__ + std::string("run001102-fitter.root")).c_str(), 65, 1102, 150);
  p.push_files((path__ + std::string("run001103-fitter.root")).c_str(), 80, 1103, 150);
  p.push_files((path__ + std::string("run001104-fitter.root")).c_str(), 95, 1104, 150);
  p.push_files((path__ + std::string("run001105-fitter.root")).c_str(), 110, 1105, 150);
  p.push_files((path__ + std::string("run001106-fitter.root")).c_str(), 125, 1106, 150);
  p.push_files((path__ + std::string("run001107-fitter.root")).c_str(), 140, 1107, 150);
  p.push_files((path__ + std::string("run001108-fitter.root")).c_str(), 155, 1108, 150);
  p.push_files((path__ + std::string("run001109-fitter.root")).c_str(), 170, 1109, 150);
  p.push_files((path__ + std::string("run001110-fitter.root")).c_str(), 185, 1110, 150);
  p.push_files((path__ + std::string("run001112-fitter.root")).c_str(), 200, 1112, 150);
  p.push_files((path__ + std::string("run001113-fitter.root")).c_str(), 215, 1113, 150);
  p.push_files((path__ + std::string("run001116-fitter.root")).c_str(), 230, 1116, 150);
  p.push_files((path__ + std::string("run001115-fitter.root")).c_str(), 245, 1115, 150);




}

void sct_runs::DEVICE_2_ASIC_on_Position_5_250V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_5_250V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(20, 110);
  p.SetRotation(0.00568813);
  p.SetPosition(65.9819, 0);
  p.push_files((path__ + std::string("run001083-fitter.root")).c_str(), 20, 1083, 250);
  p.push_files((path__ + std::string("run001084-fitter.root")).c_str(), 35, 1084, 250);
  p.push_files((path__ + std::string("run001085-fitter.root")).c_str(), 50, 1085, 250);
  p.push_files((path__ + std::string("run001086-fitter.root")).c_str(), 65, 1086, 250);
  p.push_files((path__ + std::string("run001087-fitter.root")).c_str(), 80, 1087, 250);
  p.push_files((path__ + std::string("run001088-fitter.root")).c_str(), 95, 1088, 250);
  p.push_files((path__ + std::string("run001089-fitter.root")).c_str(), 110, 1089, 250);
  p.push_files((path__ + std::string("run001090-fitter.root")).c_str(), 125, 1090, 250);
  p.push_files((path__ + std::string("run001091-fitter.root")).c_str(), 140, 1091, 250);
  p.push_files((path__ + std::string("run001092-fitter.root")).c_str(), 155, 1092, 250);
  p.push_files((path__ + std::string("run001093-fitter.root")).c_str(), 170, 1093, 250);
  p.push_files((path__ + std::string("run001094-fitter.root")).c_str(), 185, 1094, 250);
  p.push_files((path__ + std::string("run001095-fitter.root")).c_str(), 200, 1095, 250);
  p.push_files((path__ + std::string("run001096-fitter.root")).c_str(), 215, 1096, 250);
  p.push_files((path__ + std::string("run001097-fitter.root")).c_str(), 230, 1097, 250);
  p.push_files((path__ + std::string("run001098-fitter.root")).c_str(), 245, 1098, 250);


}

void sct_runs::DEVICE_2_ASIC_on_Position_5_350V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_5_350V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(20, 110);
  p.SetRotation(0.00568813);
  p.SetPosition(65.9819, 0);
  p.push_files((path__ + std::string("run001067-fitter.root")).c_str(), 20, 1067, 350);
  p.push_files((path__ + std::string("run001068-fitter.root")).c_str(), 35, 1068, 350);
  p.push_files((path__ + std::string("run001069-fitter.root")).c_str(), 50, 1069, 350);
  p.push_files((path__ + std::string("run001070-fitter.root")).c_str(), 65, 1070, 350);
  p.push_files((path__ + std::string("run001071-fitter.root")).c_str(), 80, 1071, 350);
  p.push_files((path__ + std::string("run001072-fitter.root")).c_str(), 95, 1072, 350);
  p.push_files((path__ + std::string("run001073-fitter.root")).c_str(), 110, 1073, 350);
  p.push_files((path__ + std::string("run001074-fitter.root")).c_str(), 125, 1074, 350);
  p.push_files((path__ + std::string("run001075-fitter.root")).c_str(), 140, 1075, 350);
  p.push_files((path__ + std::string("run001076-fitter.root")).c_str(), 155, 1076, 350);
  p.push_files((path__ + std::string("run001077-fitter.root")).c_str(), 170, 1077, 350);
  p.push_files((path__ + std::string("run001078-fitter.root")).c_str(), 185, 1078, 350);
  p.push_files((path__ + std::string("run001079-fitter.root")).c_str(), 200, 1079, 350);
  p.push_files((path__ + std::string("run001080-fitter.root")).c_str(), 215, 1080, 350);
  p.push_files((path__ + std::string("run001081-fitter.root")).c_str(), 230, 1081, 350);
  p.push_files((path__ + std::string("run001082-fitter.root")).c_str(), 245, 1082, 350);

}

void sct_runs::DEVICE_2_ASIC_on_Position_5_400V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_5_400V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(20, 110);
  p.SetRotation(0.00568813);
  p.SetPosition(65.9819, 0);

  p.push_files((path__ + std::string("run001051-fitter.root")).c_str(), 20, 1051, 400);
  p.push_files((path__ + std::string("run001052-fitter.root")).c_str(), 35, 1052, 400);
  p.push_files((path__ + std::string("run001053-fitter.root")).c_str(), 50, 1053, 400);
  p.push_files((path__ + std::string("run001054-fitter.root")).c_str(), 65, 1054, 400);
  p.push_files((path__ + std::string("run001055-fitter.root")).c_str(), 80, 1055, 400);
  p.push_files((path__ + std::string("run001056-fitter.root")).c_str(), 95, 1056, 400);
  p.push_files((path__ + std::string("run001057-fitter.root")).c_str(), 110, 1057, 400);
  p.push_files((path__ + std::string("run001058-fitter.root")).c_str(), 125, 1058, 400);
  p.push_files((path__ + std::string("run001059-fitter.root")).c_str(), 140, 1059, 400);
  p.push_files((path__ + std::string("run001060-fitter.root")).c_str(), 155, 1060, 400);
  p.push_files((path__ + std::string("run001061-fitter.root")).c_str(), 170, 1061, 400);
  p.push_files((path__ + std::string("run001062-fitter.root")).c_str(), 185, 1062, 400);
  p.push_files((path__ + std::string("run001063-fitter.root")).c_str(), 200, 1063, 400);
  p.push_files((path__ + std::string("run001064-fitter.root")).c_str(), 215, 1064, 400);
  p.push_files((path__ + std::string("run001065-fitter.root")).c_str(), 230, 1065, 400);
  p.push_files((path__ + std::string("run001066-fitter.root")).c_str(), 245, 1066, 400);

}



void sct_runs::DEVICE_2_ASIC_on_Position_7_150V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_7_150V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0125492);
  p.SetPosition(3.14151e+002, 0);

  p.push_files((path__ + std::string("run001136-fitter.root")).c_str(), 20, 1136, 150);
  p.push_files((path__ + std::string("run001121-fitter.root")).c_str(), 35, 1121, 150);
  p.push_files((path__ + std::string("run001122-fitter.root")).c_str(), 50, 1122, 150);
  p.push_files((path__ + std::string("run001123-fitter.root")).c_str(), 65, 1123, 150);
  p.push_files((path__ + std::string("run001124-fitter.root")).c_str(), 80, 1124, 150);
  p.push_files((path__ + std::string("run001125-fitter.root")).c_str(), 95, 1125, 150);
  p.push_files((path__ + std::string("run001126-fitter.root")).c_str(), 110, 1126, 150);
  p.push_files((path__ + std::string("run001127-fitter.root")).c_str(), 125, 1127, 150);
  p.push_files((path__ + std::string("run001128-fitter.root")).c_str(), 140, 1128, 150);
  p.push_files((path__ + std::string("run001129-fitter.root")).c_str(), 155, 1129, 150);
  p.push_files((path__ + std::string("run001130-fitter.root")).c_str(), 170, 1130, 150);
  p.push_files((path__ + std::string("run001131-fitter.root")).c_str(), 185, 1131, 150);
  p.push_files((path__ + std::string("run001132-fitter.root")).c_str(), 200, 1132, 150);
  p.push_files((path__ + std::string("run001133-fitter.root")).c_str(), 215, 1133, 150);
  p.push_files((path__ + std::string("run001137-fitter.root")).c_str(), 230, 1137, 150);
  p.push_files((path__ + std::string("run001138-fitter.root")).c_str(), 245, 1138, 150);


}

void sct_runs::DEVICE_2_ASIC_on_Position_7_250V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_7_250V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0125492);
  p.SetPosition(3.14151e+002, 0);

  p.push_files((path__ + std::string("run001173-fitter.root")).c_str(), 20, 1173, 250);
  p.push_files((path__ + std::string("run001174-fitter.root")).c_str(), 35, 1174, 250);
  p.push_files((path__ + std::string("run001175-fitter.root")).c_str(), 50, 1175, 250);
  p.push_files((path__ + std::string("run001176-fitter.root")).c_str(), 65, 1176, 250);
  p.push_files((path__ + std::string("run001177-fitter.root")).c_str(), 80, 1177, 250);
  p.push_files((path__ + std::string("run001178-fitter.root")).c_str(), 95, 1178, 250);
  p.push_files((path__ + std::string("run001179-fitter.root")).c_str(), 110, 1179, 250);
  p.push_files((path__ + std::string("run001180-fitter.root")).c_str(), 125, 1180, 250);
  p.push_files((path__ + std::string("run001181-fitter.root")).c_str(), 140, 1181, 250);
  p.push_files((path__ + std::string("run001182-fitter.root")).c_str(), 155, 1182, 250);
  p.push_files((path__ + std::string("run001183-fitter.root")).c_str(), 170, 1183, 250);
  p.push_files((path__ + std::string("run001184-fitter.root")).c_str(), 185, 1184, 250);
  p.push_files((path__ + std::string("run001185-fitter.root")).c_str(), 200, 1185, 250);
  p.push_files((path__ + std::string("run001187-fitter.root")).c_str(), 215, 1187, 250);
  p.push_files((path__ + std::string("run001188-fitter.root")).c_str(), 230, 1188, 250);
  p.push_files((path__ + std::string("run001189-fitter.root")).c_str(), 245, 1189, 250);

}

void sct_runs::DEVICE_2_ASIC_on_Position_7_350V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{
  outputPath += "/DEVICE_2_ASIC_on_Position_7_350V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0125492);
  p.SetPosition(3.14151e+002, 0);
  p.push_files((path__ + std::string("run001155-fitter.root")).c_str(), 20, 1155, 350);
  p.push_files((path__ + std::string("run001157-fitter.root")).c_str(), 35, 1157, 350);
  p.push_files((path__ + std::string("run001158-fitter.root")).c_str(), 50, 1158, 350);
  p.push_files((path__ + std::string("run001159-fitter.root")).c_str(), 65, 1159, 350);
  p.push_files((path__ + std::string("run001160-fitter.root")).c_str(), 80, 1160, 350);
  p.push_files((path__ + std::string("run001161-fitter.root")).c_str(), 95, 1161, 350);
  p.push_files((path__ + std::string("run001162-fitter.root")).c_str(), 110, 1162, 350);
  p.push_files((path__ + std::string("run001163-fitter.root")).c_str(), 125, 1163, 350);
  p.push_files((path__ + std::string("run001164-fitter.root")).c_str(), 140, 1164, 350);
  p.push_files((path__ + std::string("run001166-fitter.root")).c_str(), 155, 1166, 350);
  p.push_files((path__ + std::string("run001167-fitter.root")).c_str(), 170, 1167, 350);
  p.push_files((path__ + std::string("run001168-fitter.root")).c_str(), 185, 1168, 350);
  p.push_files((path__ + std::string("run001169-fitter.root")).c_str(), 200, 1169, 350);
  p.push_files((path__ + std::string("run001170-fitter.root")).c_str(), 215, 1170, 350);
  p.push_files((path__ + std::string("run001171-fitter.root")).c_str(), 230, 1171, 350);
  p.push_files((path__ + std::string("run001172-fitter.root")).c_str(), 245, 1172, 350);

}

void sct_runs::DEVICE_2_ASIC_on_Position_7_400V(s_process_files& p, std::string path__, std::string outputPath /*= "."*/)
{

  outputPath += "/DEVICE_2_ASIC_on_Position_7_400V.root";
  p.setOutputName(outputPath.c_str());
  p.SetNumberOfBins(400);
  p.AddCut(S_YCut(-1, 5));
  p.setActiveArea(280, 360);
  p.SetRotation(0.0125492);
  p.SetPosition(3.14151e+002, 0);
  p.push_files((path__ + std::string("run001139-fitter.root")).c_str(), 20, 1139, 400);
  p.push_files((path__ + std::string("run001140-fitter.root")).c_str(), 35, 1140, 400);
  p.push_files((path__ + std::string("run001141-fitter.root")).c_str(), 50, 1141, 400);
  p.push_files((path__ + std::string("run001142-fitter.root")).c_str(), 65, 1142, 400);
  p.push_files((path__ + std::string("run001143-fitter.root")).c_str(), 80, 1143, 400);
  p.push_files((path__ + std::string("run001144-fitter.root")).c_str(), 95, 1144, 400);
  p.push_files((path__ + std::string("run001145-fitter.root")).c_str(), 110, 1145, 400);
  p.push_files((path__ + std::string("run001146-fitter.root")).c_str(), 125, 1146, 400);
  p.push_files((path__ + std::string("run001147-fitter.root")).c_str(), 140, 1147, 400);
  p.push_files((path__ + std::string("run001148-fitter.root")).c_str(), 155, 1148, 400);
  p.push_files((path__ + std::string("run001149-fitter.root")).c_str(), 170, 1149, 400);
  p.push_files((path__ + std::string("run001150-fitter.root")).c_str(), 185, 1150, 400);
  p.push_files((path__ + std::string("run001151-fitter.root")).c_str(), 200, 1151, 400);
  p.push_files((path__ + std::string("run001152-fitter.root")).c_str(), 215, 1152, 400);
  p.push_files((path__ + std::string("run001153-fitter.root")).c_str(), 230, 1153, 400);
  p.push_files((path__ + std::string("run001154-fitter.root")).c_str(), 245, 1154, 400);

}

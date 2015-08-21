
#include "s_process_files.h"
#include "TError.h"


#include <iostream>
#include <sstream>


#include "tclap/CmdLine.h"
#include "xml_helpers/xml_fileList.hh"
#include <thread>
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile.h"
#include "TF1.h"
#include "TBrowser.h"
#include "TH2.h"
#include "geometry/setup_description.hh"

using namespace xml_util;

using namespace rapidxml;

std::vector<TCanvas*> gCanvas;
TBrowser* gBrowser =NULL;
int ADDRun(s_process_files& p, std::string xmlInputFileName, std::string path__, std::string outputPath = "."){
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty())
  {
    return -1;
  }
  auto collname = xml_imput.globalConfig().CollectionName();
  outputPath += "/" + collname + ".root";

  p.setOutputName(outputPath.c_str());

  p.SetNumberOfBins(xml_imput.globalConfig().NumberOfBins());
  p.AddCut(xml_imput.globalConfig().cut());
  p.setActiveArea(xml_imput.globalConfig().AvtiveStrips().getMin(), xml_imput.globalConfig().AvtiveStrips().getMax());
  p.SetRotation(xml_imput.globalConfig().Rotation());
  p.SetPosition(xml_imput.globalConfig().Position_value(), 0);
  p.setResidualCut(xml_imput.globalConfig().residual_cut());

  for (auto& e : xml_imput.fileList()){
    p.push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());
  }

  return 0;
}

using namespace std;
using namespace TCLAP;

void remove_root_printouts() {
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
}

struct  inParam {
  int argc;
  char **argv;
};


int asyncMain(void *arg) {
  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;
  TApplication theApp("App", &argc, argv);
  TFile * file_ = new TFile("D:/GBL/run000835_fitter.root");
  TFile * out_file = new TFile("output.root","recreate");
  S_plot_collection pl(file_);
  pl.setOutputFile(out_file);

  //auto gbl_collection = sct_plot::Create_Correlations_of_true_Fitted_hits_with_DUT_Hits_in_channels(pl, 0.0745, 0, 669.366 + 2 - 0.5 - 0.2, 0, S_YCut(-43000, 3600), 100000, s_plot_prob("GBL").SaveToDisk()); // 1 / 13.4031 / 1.00365 / 0.996267
  auto gbl_collection = sct_plot::GBL_Create_Correlations_of_true_Fitted_hits_with_DUT_Hits(pl, S_YCut(-46, -40), 100000, s_plot_prob("GBL").SaveToDisk()); // 1 / 13.4031 / 1.00365 / 0.996267
  auto corr = pl.addPlot(sct_plot::correlation(s_plot_prob().SaveToDisk()), gbl_collection.getTotalTrueHits().getX_def(), sct_coll::DUT_zs_data().getX_def());
  auto res = pl.addPlot(sct_plot::residual(s_plot_prob().SaveToDisk()), gbl_collection.getTotalTrueHits().getX_def(), gbl_collection.getDUT_Hits().getX_def());
  auto corr2 = pl.addPlot(sct_plot::correlation(), sct_coll::DUT_fitted_local_GBL().getY_def(), sct_coll::apix_zs_data().getY_def());
  auto res2 = pl.addPlot(sct_plot::residual(), sct_coll::DUT_fitted_local_GBL().getX_def(), sct_coll::DUT_hit_local().getX_def());


  pl.loop();



  gCanvas.push_back(new TCanvas());
  
  TH2D* h3 = new TH2D("asdfkjsfdjhdsfadsfj", "corr2", 100, 0, 0, 100, 0, 0);
  pl.Draw(corr2(), S_DrawOption().draw_y_VS_x().output_object(h3).opt_colz());
   auto f1 = SCT_helpers::LinearFit_Of_Profile(h3, 0.2);
   f1.Print();
  gCanvas.push_back(new TCanvas());
  pl.Draw(res2(), S_DrawOption().draw_x().cut_x(-0.5,0.5));
  gCanvas.push_back(new TCanvas());
  TH2D* h22 = new TH2D("asd22", "ResidualVSmissing", 100, 0, 0, 100, 0, 0);
  pl.Draw(gbl_collection.getResidualVSmissing(), S_DrawOption().draw_x_VS_y().output_object(h22));
   auto f222 = SCT_helpers::LinearFit_Of_Profile(h22, 0.2);
   f222.Print();



  gCanvas.push_back(new TCanvas());
  TH2D* h2 = new TH2D("asd", "corr", 100, 0, 0, 100, 0, 0);
  pl.Draw(corr(), S_DrawOption().draw_y_VS_x().output_object(h2));
  auto f=SCT_helpers::LinearFit_Of_Profile(h2,0.2);
  f.Print();
  
  
  gCanvas.push_back(new TCanvas());
  pl.Draw(res(), S_DrawOption().draw_x().cut_x(-1,1));


  gCanvas.push_back(new TCanvas());
  
  pl.Draw(sct_coll::tel_fitted_local_GBL(8), S_DrawOption().draw_y().color_red());
  pl.Draw(gbl_collection.getTotalTrueHits(), S_DrawOption().draw_y().opt_same());
  
  pl.Draw(gbl_collection.getTrueHitsWithDUT(), S_DrawOption().draw_y().opt_same().color_blue());


  gCanvas.push_back(new TCanvas());
  TH1D trueHit("true", "TotalTrueHits", 100, -60, 0);
  //pl.Draw(sct_coll::tel_fitted_local_GBL(8), S_DrawOption().draw_x().color_red());
  pl.Draw(gbl_collection.getTotalTrueHits(), S_DrawOption().draw_x().output_object(&trueHit));
  TH1D DUTHit("dut", "TrueHitsWithDUT", 100, -60, 0);
  pl.Draw(gbl_collection.getTrueHitsWithDUT(), S_DrawOption().draw_x().opt_same().color_blue().output_object(&DUTHit));

  DUTHit.Divide(&trueHit);
  gCanvas.push_back(new TCanvas());
  DUTHit.Draw();

  gBrowser = new TBrowser();
  theApp.Run();
  
  exit(0);
  return 0;
}
int main(int argc, char **argv) {
  rapidxml::file<> m_file("D:/GBL/alignedGear-check-iter0-run000835.xml");
  rapidxml::xml_document<> m_doc;
   m_doc.parse<0>(m_file.data());

   auto gear=sct_corr::Xgear(m_doc.first_node("gear"));
  remove_root_printouts();
  
  




  inParam para;
  para.argc = argc;
  para.argv = argv;

  std::cout << "press q to quit the program" << std::endl;
  std::thread thr(asyncMain, &para);
  thr.detach();
  std::string i;
  
  while (i != "q") {
    std::cin >> i;

  }


  return 0;

}


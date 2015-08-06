
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

using namespace xml_util;

using namespace rapidxml;


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
  TFile * file_ = new TFile("C:/Users/Argg/OneDrive/alibava/GBL/AllTracks-WithStateOnDUT.root");
  TFile * out_file = new TFile("output.root","recreate");
  S_plot_collection pl(file_);
  pl.setOutputFile(out_file);
  auto res = pl.addPlot(sct_plot::residual(s_plot_prob("residual").SaveToDisk()), sct_coll::DUT_fitted_local_GBL().getX_def(), sct_coll::DUT_hit_local().getX_def());


  auto apix_transform = pl.addPlot(sct_plot::coordinate_transform(1 / 3.98148, -156.98 + 118.43, 1 / -19.4048, -34.0894, s_plot_prob().SaveToDisk()), sct_coll::apix_zs_data());

  auto fitted_true= pl.addPlot(sct_plot::find_nearest(1,3, s_plot_prob().SaveToDisk()), sct_coll::DUT_fitted_local_GBL(), apix_transform());
  auto cor_apix = pl.addPlot(sct_plot::correlation(s_plot_prob("apix").SaveToDisk()), sct_coll::DUT_fitted_local_GBL().getY_def(), apix_transform().getY_def());

 // auto trueHits = pl.addPlot(sct_plot::coordinate_transform(13.3934 / 1.00377, 669.918 - 1.75912 + 0.404092+0.01228, 1, 0, s_plot_prob().SaveToDisk()), sct_coll::DUT_fitted_local_GBL());

  auto trueHits_cut = pl.addPlot(sct_plot::cut_x_y(S_YCut(-43,-36), s_plot_prob().SaveToDisk()), fitted_true.get(1));
  auto trueHits = pl.addPlot(sct_plot::coordinate_transform(1, 0, 1, 0, s_plot_prob().SaveToDisk()), trueHits_cut());
  auto corr = pl.addPlot(sct_plot::correlation(s_plot_prob("correlation").SaveToDisk()), trueHits().getX_def(), sct_coll::DUT_hit_local().getX_def());

  auto res_channel = pl.addPlot(sct_plot::residual(s_plot_prob("residual_channel").SaveToDisk()), trueHits().getX_def(), sct_coll::DUT_hit_local().getX_def());

  auto find_closest = pl.addPlot(sct_plot::find_nearest_strip( x_axis_def, 100,  s_plot_prob("closest").SaveToDisk()), trueHits(), sct_coll::DUT_hit_local());
  auto res_vs_missing = pl.addPlot(sct_plot::hitmap(s_plot_prob("res_vs_missing").SaveToDisk()), find_closest.get(0).getX_def(), find_closest.get(1).getY_def());
  pl.loop();
  TH2D h2("asd", "sad", 100, 0, 0, 100, 0, 0);
  pl.Draw(corr, S_DrawOption().draw_y_VS_x().opt_colz().output_object(&h2));
  SCT_helpers::CutTH2(&h2, S_Cut_BinContent(200));
  h2.Draw("colz");
  auto p = h2.ProfileX();
  TF1 f1("f1", "pol1");
  p->Fit(&f1);

  p->Draw("same");
  new TCanvas();
  pl.Draw(res_channel(), S_DrawOption().draw_x().cut_x(-0.05,0.05));
  new TCanvas();
  pl.Draw(res_vs_missing(), S_DrawOption().draw_x_VS_y().cut_x(-0.05, 0.05));

  new TCanvas();
  TH1D h("trueHits", "true hits", 401, -50,-10);
  auto trueHIts_entries=pl.Draw(trueHits(), S_DrawOption().draw_y_VS_x());
  pl.Draw(trueHits(), S_DrawOption().draw_x().output_object(&h));
  new TCanvas();
  TH1D h1("DUT_Hits", "DUT hits", 401, -50, -10);
  auto DUT_hits_entries=pl.Draw(find_closest.get(1), S_DrawOption().draw_y_VS_x());
  pl.Draw(find_closest.get(1), S_DrawOption().draw_x().output_object(&h1));

  std::cout << "DUT_hits_entries: "<< DUT_hits_entries << std::endl;
  std::cout << "trueHIts_entries: "<<trueHIts_entries << std::endl;
  std::cout << "efficiency: "<<DUT_hits_entries / (Double_t)trueHIts_entries * 100 << std::endl;
   auto h_eff = (TH1D*)h1.Clone("efficiency");
   h_eff->Divide(&h);
   new TCanvas();
   h_eff->Draw();
  

  new TCanvas();
  TH2D h3("asd", "sad", 100, 0, 0, 100, 0, 0);
  pl.Draw(cor_apix(), S_DrawOption().draw_y_VS_x().output_object(&h3));
  SCT_helpers::CutTH2(&h3, S_Cut_BinContent(200));
  h3.Draw("colz");
  auto p3 = h3.ProfileX();
  TF1 f2("f2", "pol1");
  f2.SetParameter("p1", 1);
  p3->Fit(&f2);
  p3->Draw("same");


  new TCanvas();
  pl.Draw(fitted_true.get(0), S_DrawOption().draw_y_VS_x());

  theApp.Run();
  
  exit(0);
  return 0;
}
int main(int argc, char **argv) {

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


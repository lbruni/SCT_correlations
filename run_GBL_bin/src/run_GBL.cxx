
#include "s_process_files.h"
#include "TError.h"


#include <iostream>
#include <sstream>


#include "tclap/CmdLine.h"
#include "xml_helpers/xml_fileList.hh"
#include <thread>
#include "TApplication.h"
#include "TCanvas.h"


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
  TFile * file_ = new TFile("C:/Users/Argg/OneDrive/alibava/GBL/fitterCHECK.root");
  TFile * out_file = new TFile("output.root","recreate");
  S_plot_collection pl(file_);
  pl.setOutputFile(out_file);
  auto corr=pl.addPlot(sct_plot::correlation(s_plot_prob("correlation").SaveToDisk()), sct_coll::DUT_fitted_local_GBL().getX_def(), sct_coll::DUT_hit_local().getX_def());
  auto res = pl.addPlot(sct_plot::residual(s_plot_prob("residual").SaveToDisk()), sct_coll::DUT_fitted_local_GBL().getX_def(), sct_coll::DUT_hit_local().getX_def());
  auto find_closest = pl.addPlot(sct_plot::find_nearest_strip( x_axis_def, 100,  s_plot_prob("closest").SaveToDisk()), sct_coll::DUT_fitted_local_GBL(), sct_coll::DUT_hit_local());
  auto res_vs_missing = pl.addPlot(sct_plot::hitmap(s_plot_prob("res_vs_missing").SaveToDisk()), find_closest.get(0).getX_def(), find_closest.get(1).getY_def());
  pl.loop();
  
  pl.Draw(corr, S_DrawOption().draw_y_VS_x().opt_colz());
  new TCanvas();
  pl.Draw(res(), S_DrawOption().draw_x().cut_x(-0.1, 0.1));
  new TCanvas();
  pl.Draw(res_vs_missing(), S_DrawOption().draw_x_VS_y().cut_x(-0.1, 0.1));
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


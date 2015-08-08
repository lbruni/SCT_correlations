
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

  auto gbl_collection = sct_plot::Create_Correlations_of_true_Fitted_hits_with_DUT_Hits_in_channels(pl, 0.0745, 0, 669.366 + 2 - 0.5 - 0.2, 0, S_YCut(-43, -36), 100000, s_plot_prob("GBL").SaveToDisk()); // 1 / 13.4031 / 1.00365 / 0.996267

  auto corr = pl.addPlot(sct_plot::correlation(s_plot_prob().SaveToDisk()), gbl_collection.getTotalTrueHits().getX_def(), gbl_collection.getDUT_Hits().getX_def());
  auto res = pl.addPlot(sct_plot::residual(s_plot_prob().SaveToDisk()), gbl_collection.getTotalTrueHits().getX_def(), gbl_collection.getDUT_Hits().getX_def());


  pl.loop();


  new TCanvas();

  pl.Draw(gbl_collection.getResidualVSmissing(), S_DrawOption().draw_y_VS_x().cut_x(-5,5));

  new TCanvas();
  TH2D* h2 = new TH2D("asd", "asda", 100, 0, 0, 100, 0, 0);
  pl.Draw(corr(), S_DrawOption().draw_y_VS_x().output_object(h2));
  auto f=SCT_helpers::LinearFit_Of_Profile(h2,0.2);
  f.Print();
  
  
  new TCanvas();
  pl.Draw(res(), S_DrawOption().draw_x().cut_x(-1,1));

  new TBrowser();
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


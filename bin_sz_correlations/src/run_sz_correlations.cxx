
#include "internal/inStripEfficiency.hh"
#include <iostream>
#include <sstream>
#include <thread>

#include "processorBase.h"
#include "TError.h"




#include "tclap/CmdLine.h"
#include "xml_helpers/xml_fileList.hh"
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile.h"
#include "TF1.h"
#include "TBrowser.h"
#include "TH2.h"
#include "geometry/setup_description.hh"
#include "s_file_base.h"
#include "sct_processors.h"
#include "SCT_helpers.h"
#include "internal/hit_efficiency.hh"
#include "internal/inStripClusterSize.hh"

#include "internal/residual_efficienct.hh"
#include "processors/find_second_nearest_strip.hh"
#include "processors/find_nearest.hh"
#include "sct_processors.h"
#include "processors/modulo.hh"
#include "processors/find_nearest_strip.hh"
#include "s_plot_collection.h"
#include "plane_def.h"


using namespace xml_util;

using namespace rapidxml;

std::vector<TCanvas*> gCanvas;
TBrowser* gBrowser = NULL;


using namespace std;
using namespace TCLAP;

void remove_root_printouts() {
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
}

struct  inParam {
  int argc;
  char **argv;
};



using namespace sct_type;
int asyncMain(void *arg) {
  int argc_asdasddsa = 1;
  char **argv_sadsda = new char*[1];
  argv_sadsda[0] = "dummy";
  TApplication theApp("App", &argc_asdasddsa, argv_sadsda);

  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;

  CmdLine cmd("ProcessFile", ' ', "0.1");
  ValueArg<std::string> FileNameArg("i", "inFile", "root filename", true, "", "string");
  cmd.add(FileNameArg);


  ValueArg<std::string> outFileNameArg("o", "outFile", "output root file", false, "dummy.root", "string");
  cmd.add(outFileNameArg);

  cmd.parse(argc, argv);  //terminates on error

  sct_files::sz_data_file file__(FileNameArg.getValue().c_str());

  TFile * file_1 = new TFile(outFileNameArg.getValue().c_str(),"recreate");
  auto pl1 = file__.get_collection();
  pl1->setOutputFile(file_1);
  

  auto sz_dut = file__.DUT_sz_data();
  auto sz_tel = file__.TEL_sz_data(sct_type::ID_t(5));

  auto cuted = sct_corr::processor::cut_x_y(sz_tel, S_XCut(200) + S_YCut(200),s_plot_prob().doNotSaveToDisk());
  auto corr = sct_corr::processor::correlation(sz_dut.getX_def(), cuted.getY_def(), s_plot_prob("correlation_x_y").SaveToDisk());
  auto corryx = sct_corr::processor::correlation(sz_dut.getY_def(), cuted.getX_def(), s_plot_prob("correlation_y_x").SaveToDisk());
  auto corrxx = sct_corr::processor::correlation(sz_dut.getX_def(), cuted.getX_def(), s_plot_prob("correlation_x_x").SaveToDisk());
  auto corryy = sct_corr::processor::correlation(sz_dut.getY_def(), cuted.getY_def(), s_plot_prob("correlation_y_y").SaveToDisk());
  pl1->loop();
  new TCanvas();
  SCT_helpers::Draw(corr, S_DrawOption().opt_colz());
  new TCanvas();
  SCT_helpers::Draw(corryx, S_DrawOption().opt_colz());
  new TCanvas();
  SCT_helpers::Draw(corrxx, S_DrawOption().opt_colz());
  new TCanvas();
  SCT_helpers::Draw(corryy, S_DrawOption().opt_colz());

  gBrowser = new TBrowser();
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
  _exit(0);

  return 0;

}


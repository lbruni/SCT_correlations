
#include "internal/inStripEfficiency.hh"
#include <iostream>
#include <sstream>
#include <thread>

#include "s_process_collection.h"
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



int asyncMain(void *arg) {
  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;
  TApplication theApp("App", &argc, argv);
  TFile * file_ = new TFile("D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/run000703_fitter.root");
  rapidxml::file<> m_file("D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/alignedGear-check-iter2-run000703_with_plane20.xml");
  rapidxml::xml_document<> m_doc;
  m_doc.parse<0>(m_file.data());

  auto gear = sct_corr::Xgear(m_doc.first_node("gear"));


  TFile * out_file = new TFile("output.root", "recreate");
  r_plot_collection pl(file_);
  pl.setOutputFile(out_file);
  s_file_fitter file___(pl.get_plot_collection_ptr(), &gear);


  auto gbl_collection = file___.get_correlations_channel(
    S_YCut(-42, -36),
    1, 
    0, 
    -6.36702e-001, 
    s_plot_prob("GBL").SaveToDisk()
    );
    

  sct_corr::inStripEfficiency instrip(
    gbl_collection.getTotalTrueHits(),
    gbl_collection.getTrueHitsWithDUT(),
    S_XCut(280, 360),
    x_axis_def,
    3,
    s_plot_prob("inStripEffi")
    );


  sct_corr::hit_efficiency eff(
    gbl_collection.getTotalTrueHits(),
    gbl_collection.getTrueHitsWithDUT(),
    s_plot_prob("effi")
    );
  sct_corr::inStripClusterSize cl_instrip(
    gbl_collection.getTrueHitsWithDUT(), 
    file___.DUT_zs_data(), 10,
    x_axis_def, 
    3, 
    s_plot_prob("cluster_size_instrip").SaveToDisk()
    );

  sct_corr::residual_efficienct res_eff(
    gbl_collection.getTotalTrueHits(), 
    file___.DUT_zs_data(), 
    400, 
    x_axis_def, 
    s_plot_prob("Res_efficiency")
    );

  pl.loop(40000);

  gCanvas.push_back(new TCanvas());

  res_eff.Draw();

  
  gCanvas.push_back(new TCanvas());
  instrip.Draw(S_DrawOption());


  gCanvas.push_back(new TCanvas());
  
  eff.Draw();
 
  gCanvas.push_back(new TCanvas());

  cl_instrip.Draw();
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


  return 0;

}


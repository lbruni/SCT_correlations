
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


  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;
  TApplication theApp("App", &argc, argv);
  std::string path_ = "D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_400V/";
  std::string name_ = "run000801_";
  std::string name_suffix = "fitter";
  std::string extension_="root";
  std::string fullName = path_ + name_ + name_suffix+"." + extension_;
  
 //TFile * file_ = new TFile(fullName.c_str());
//   rapidxml::file<> m_file("D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/alignedGear-check-iter2-run000703_with_plane20.xml");
//   rapidxml::xml_document<> m_doc;
//   m_doc.parse<0>(m_file.data());
// 
//   auto gear = sct_corr::Xgear(m_doc.first_node("gear"));
//  auto gear = sct_corr::load_gear("D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/alignedGear-check-iter2-run000703_with_plane20.xml");

 // r_plot_collection pl(file_);
//  pl.setOutputFile(out_file);
  sct_files::fitter_file file___(fullName.c_str(), "D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/alignedGear-check-iter2-run000703_with_plane20.xml");
  TFile * out_file = new TFile("output.root", "recreate");
  auto pl = file___.get_collection();
  pl->setOutputFile(out_file);
  


  auto gbl_collection = file___.get_correlations_channel(
    S_YCut(-42, -36),
    residualCut_t(10),
    rot_angle_t(0),
    move_t(-6.36702e-001),
    s_plot_prob("GBL").SaveToDisk()
    );
 


    auto trueHit_with_tdc_cut = sct_corr::processor::if_a_get_b(
      file___.DUT_TTC_data(),
      gbl_collection.getTotalTrueHits(), 
      S_XCut(0, 100),
      s_plot_prob("withTDC_CUT")
      );


    auto  second_nearest = sct_processor::find_nearest_strip(
    trueHit_with_tdc_cut,
    file___.DUT_zs_data(),
    x_axis_def, 
    3, 
    "first"
    );
  
  auto mod = sct_processor::modulo(
    second_nearest.getHitOnPlaneA(), 
    modulo_t(3), 
    x_axis_def
    );


  auto rmap = sct_corr::processor::correlation(
    mod.getModulo().getX_def(), 
    second_nearest.getResidual().getX_def()
    );


  sct_corr::inStripEfficiency instrip(
    gbl_collection.getTotalTrueHits(),
    second_nearest.getHitOnPlaneA(),
    S_XCut(280, 360),
    x_axis_def,
    modulo_t(3),
    s_plot_prob("inStripEffi")
    );


  sct_corr::hit_efficiency eff(
    gbl_collection.getTotalTrueHits(),
    second_nearest.getHitOnPlaneA(),
    s_plot_prob("effi")
    );
  sct_corr::inStripClusterSize cl_instrip(
    gbl_collection.getTrueHitsWithDUT(), 
    file___.DUT_zs_data(), 10,
    x_axis_def, 
    modulo_t(3),
    s_plot_prob("cluster_size_instrip").SaveToDisk()
    );

  sct_corr::residual_efficienct res_eff(
    gbl_collection.getTotalTrueHits(), 
    second_nearest.getHitOnPlaneB(), 
    S_XCut(280,360),
    stripNr_t(400),
    x_axis_def, 
    s_plot_prob("Res_efficiency")
    );

  pl->loop(40000);

  //pl->loop();

  gCanvas.push_back(new TCanvas());
  SCT_helpers::Draw(rmap, S_DrawOption().draw_y_VS_x());

  gCanvas.push_back(new TCanvas());

  res_eff.Draw();
  SCT_helpers::saveTH1_as_txt(*res_eff.getEfficiency_map(), (path_ + name_ + "residual_efficiency" + "." + "txt").c_str());
  
  gCanvas.push_back(new TCanvas());
  instrip.Draw(S_DrawOption());
  std::cout << "Efficiency " << instrip.get_total_efficiency() << std::endl;
  SCT_helpers::saveTH1_as_txt(*instrip.getEfficiency_map(), (path_ + name_ + "instripEffi" + "." + "txt").c_str());

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


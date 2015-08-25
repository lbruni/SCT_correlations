
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

using namespace xml_util;

using namespace rapidxml;

std::vector<TCanvas*> gCanvas;
TBrowser* gBrowser = NULL;
int ADDRun(s_process_collection& p, std::string xmlInputFileName, std::string path__, std::string outputPath = ".") {
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty()) {
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

  for (auto& e : xml_imput.fileList()) {
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

int asyncMain1(void *arg) {
  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;
  TApplication theApp("App", &argc, argv);
#if 0
  TFile * file_ = new TFile("D:/DAFF/run000703-fitter.root");
  rapidxml::file<> m_file("D:/DAFF/gear_desy2012_150mm_fei4000703_pre_aligned.xml");

#else
  TFile * file_ = new TFile("D:/GBL/Neuer Ordner/run000703_fitter.root");
  rapidxml::file<> m_file("D:/GBL/Neuer Ordner/alignedGear-check-iter2-run000703_with_plane20.xml");


#endif




  rapidxml::xml_document<> m_doc;
  m_doc.parse<0>(m_file.data());

  auto gear = sct_corr::Xgear(m_doc.first_node("gear"));


  TFile * out_file = new TFile("output.root", "recreate");
  S_plot_collection pl(file_);
  pl.setOutputFile(out_file);
#if 0
  double ID = 1;
  //auto loc = sct_plot::convert_local_to_global(pl, *gear.detector.layer_by_ID(ID), sct_coll::tel_hit_local(ID));
  auto loc = sct_plot::convert_global_to_local(pl, *gear.detector.layer_by_ID(ID), sct_coll::tel_hit(ID));
  auto corrxx = sct_plot::correlation(pl, sct_coll::tel_hit_local(ID).getX_def(), loc.getX_def());
  auto corryy = sct_plot::correlation(pl, sct_coll::tel_hit_local(ID).getY_def(), loc.getY_def());
#else
#if 0

  auto loc = sct_plot::convert_global_to_local(pl, *gear.detector.layer_by_ID(8), sct_coll::DUT_hit());
  auto corrxx = sct_plot::correlation(pl, loc.getX_def(), sct_coll::DUT_hit_local().getX_def());
  auto corryy = sct_plot::correlation(pl, loc.getY_def(), sct_coll::DUT_hit_local().getY_def());
#else
  auto loc = sct_plot::convert_local_to_global(pl, *gear.detector.layer_by_ID(20), sct_coll::apix_hit_local());
  auto loc11 = sct_plot::convert_global_to_local(pl, *gear.detector.layer_by_ID(8), loc);

  auto closest = sct_plot::find_nearest(pl, 1, 1, loc11, sct_coll::DUT_fitted_local_GBL());
  auto pix = sct_plot::convert_hits_to_zs_data_GBL(pl, *gear.detector.layer_by_ID(8), closest.getHitOnPlaneB());
  auto corrxx = sct_plot::correlation(pl, pix.getX_def(), sct_coll::DUT_zs_data().getX_def());
  auto corryy = sct_plot::correlation(pl, loc11.getY_def(), sct_coll::DUT_zs_data().getY_def());
#endif

#endif
  auto co = sct_plot::correlation(pl, sct_coll::apix_hit().getX_def(), sct_coll::tel_hit(5).getX_def());

  pl.loop(40000);
  gCanvas.push_back(new TCanvas());
  pl.Draw(closest.getResidual(), S_DrawOption().opt_colz());
  gCanvas.push_back(new TCanvas());
  TH2D* h3 = new TH2D("asdfkjsadasdsfdjhdsfadsfj", "corrxx", 100, 0, 0, 100, 0, 0);
  pl.Draw(corrxx, S_DrawOption().draw_y_VS_x().output_object(h3));


  gCanvas.push_back(new TCanvas());
  TH2D* h33 = new TH2D("asdfkjsfdjhdsfadsfj", "corryy", 100, 0, 0, 100, 0, 0);
  pl.Draw(corryy, S_DrawOption().draw_y_VS_x().output_object(h33));
#if 1
  auto f222 = SCT_helpers::LinearFit_Of_Profile(h3, 0.2);
  f222.Print();
  auto f22222 = SCT_helpers::LinearFit_Of_Profile(h33, 0.2);
  f22222.Print();
#endif // 0

  new TBrowser();
  theApp.Run();

  return 1;
}

int asyncMain(void *arg) {
  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;
  TApplication theApp("App", &argc, argv);
  TFile * file_ = new TFile("D:/GBL/DEVICE_1_ASIC_on_Position_7_Jim_350V/run000818_fitter.root");
  rapidxml::file<> m_file("D:/GBL/Neuer Ordner/alignedGear-check-iter2-run000703_with_plane20.xml");
  rapidxml::xml_document<> m_doc;
  m_doc.parse<0>(m_file.data());

  auto gear = sct_corr::Xgear(m_doc.first_node("gear"));


  TFile * out_file = new TFile("output.root", "recreate");
  r_plot_collection pl(file_);
  pl.setOutputFile(out_file);
  s_file_fitter file___(pl.get_plot_collection_ptr(), &gear);


  auto gbl_collection = file___.get_correlations_channel(
    S_YCut(-42, -36),
    100000, 
    0, 
    -6.36702e-001, 
    s_plot_prob("GBL").SaveToDisk()
    );
    
  //gbl_collection.set_s_plot_collection(pl.get_plot_collection_ptr());
  auto cut_true = sct_processor::cut_x_y(
    gbl_collection.getTotalTrueHits(),
    S_XCut(280, 360),

    s_plot_prob().doNotSaveToDisk()
    );


  auto cut_dut = sct_processor::cut_x_y(
    gbl_collection.getTrueHitsWithDUT(),
    S_XCut(280, 360),
    s_plot_prob().doNotSaveToDisk()
    );
  auto sz_data = sct_coll::DUT_zs_data();
  sz_data.set_s_plot_collection(pl.get_plot_collection_ptr());
  auto cluster_ = sct_processor::cluster_strip(
    sz_data,
    x_axis_def,
    2,
    s_plot_prob().doNotSaveToDisk()
    );

  auto cluster__cut = sct_processor::cut_x_y(
    cluster_,
    S_YCut(0, 15),

    s_plot_prob().doNotSaveToDisk()
    );

  auto cluster_closest = sct_processor::find_nearest_strip(
    cluster__cut,
    gbl_collection.getTotalTrueHits(),
    x_axis_def,
    1000,

    s_plot_prob().doNotSaveToDisk()
    );

  auto mod_total_closest = sct_processor::moduloHitMap(
    cluster_closest.getHitOnPlaneB(),
    3,
    11111113.0,
    s_plot_prob("mod")
    );


  auto hitmap = sct_processor::hitmap(
    mod_total_closest.getX_def(), cluster_closest.getHitOnPlaneA().getY_def()
    );

  auto DUT_fitted_local_GBL__ = sct_coll::DUT_fitted_local_GBL();
  DUT_fitted_local_GBL__.set_s_plot_collection(pl.get_plot_collection_ptr());
  auto DUT_hit_local___ = sct_coll::DUT_hit_local();
  DUT_hit_local___.set_s_plot_collection(pl.get_plot_collection_ptr());
  auto res = sct_processor::residual(
    DUT_fitted_local_GBL__.getX_def(),
    DUT_hit_local___.getX_def(),
    s_plot_prob("residualVSEvent")
    );
  auto mod_total = sct_processor::moduloHitMap(
    cut_true,
    3,
    11111113.0,
    s_plot_prob("mod")
    );

  auto mod_DUT = sct_processor::moduloHitMap(
    cut_dut,
    3,
    300000000.0,
    s_plot_prob("modDUT")
    );

  pl.loop(40000);

  gCanvas.push_back(new TCanvas());
  TH2D cl_pos("asda", "cl_pos", 100, 0, 3, 20, 0, 20);
  pl.Draw(hitmap, S_DrawOption().draw_y_VS_x().opt_colz().output_object(&cl_pos));
  auto prof = cl_pos.ProfileX();
  prof->Draw("same");

  gCanvas.push_back(new TCanvas());
  TH1D trueHitsMod("sadad", "true hits", 300, 0, 3);
  TH1D DUtHitsMod("sadaasdad", "DUT hits", 300, 0, 3);

  pl.Draw(mod_total, S_DrawOption().draw_x().output_object(&trueHitsMod));
  pl.Draw(mod_DUT, S_DrawOption().draw_x().output_object(&DUtHitsMod).opt_same().color_green());
  auto effi = SCT_helpers::calc_efficiency(&trueHitsMod, &DUtHitsMod);
  effi->SetTitle("efficiency");
  gCanvas.push_back(new TCanvas());

  effi->Draw();

  gBrowser = new TBrowser();
  theApp.Run();
  gCanvas.push_back(new TCanvas());
  pl.Draw(res, S_DrawOption().draw_x_VS_y());


  gCanvas.push_back(new TCanvas());
  pl.Draw(gbl_collection.getResidual(), S_DrawOption().draw_x().cut_x(-5, 5));

  gCanvas.push_back(new TCanvas());
  TH2D* h22 = new TH2D("asd22", "ResidualVSmissing", 100, 0, 0, 100, 0, 0);
  pl.Draw(gbl_collection.getResidualVSmissing(), S_DrawOption().draw_x_VS_y().output_object(h22).cut_x(-2, 2));
  //  auto f222 = SCT_helpers::LinearFit_Of_Profile(h22, 0.2);
  //  f222.Print();



  gCanvas.push_back(new TCanvas());

  //  pl.Draw(sct_coll::tel_fitted_local_GBL(8), S_DrawOption().draw_y().color_red());
  pl.Draw(gbl_collection.getTotalTrueHits(), S_DrawOption().draw_y());

  pl.Draw(gbl_collection.getTrueHitsWithDUT(), S_DrawOption().draw_y().opt_same().color_blue());


  gCanvas.push_back(new TCanvas());
  TH1D DUTHit("dut", "Efficency", 1600, -100, 400);
  pl.Draw(gbl_collection.getTrueHitsWithDUT(), S_DrawOption().draw_x().color_blue().output_object(&DUTHit));

  TH1D trueHit("true", "TotalTrueHits", 1600, -100, 400);
  //pl.Draw(sct_coll::tel_fitted_local_GBL(8), S_DrawOption().draw_x().color_red());
  pl.Draw(gbl_collection.getTotalTrueHits(), S_DrawOption().draw_x().output_object(&trueHit));
  DUTHit.Divide(&trueHit);
  gCanvas.push_back(new TCanvas());
  DUTHit.Draw();


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


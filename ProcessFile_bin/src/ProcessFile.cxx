
#include "s_process_collection.h"

#include "TError.h"

#include "TApplication.h"
#include "TBrowser.h"
#include "s_cuts.h"
#include "TF1.h"
#include "TProfile.h"
#include "xml_helpers/xml_fileList.hh"

#include "tclap/CmdLine.h"
#include <iostream>
#include "TCanvas.h"
#include <thread>

#include "TSystem.h"

using namespace xml_util;
using namespace TCLAP;
using namespace std;







int ADDRun(s_process_collection& p, std::string xmlInputFileName, std::string path__, int element, std::string outputPath = ".") {
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty()) {
    return -1;
  }
  auto collname = xml_imput.globalConfig().CollectionName();
  outputPath += "/" + collname + "_" + std::to_string(element) + ".root";

  p.setOutputName(outputPath.c_str());

  p.SetNumberOfBins(xml_imput.globalConfig().NumberOfBins());
  p.SetNumberOfStrips(xml_imput.globalConfig().NumberOfStrips());
  p.AddCut(xml_imput.globalConfig().cut());
  p.setActiveArea(xml_imput.globalConfig().AvtiveStrips().getMin(), xml_imput.globalConfig().AvtiveStrips().getMax());
  p.SetRotation(xml_imput.globalConfig().Rotation());
  p.SetPosition(xml_imput.globalConfig().Position_value(), 0);
  p.setResidualCut(xml_imput.globalConfig().residual_cut());
  p.setGearFile(xml_imput.globalConfig().gearFile().c_str());
  if (element >= xml_imput.fileList().size()) {
    return -1;
  }
  auto& e = xml_imput.fileList()[element];
  p.push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());


  return 0;
}
std::string trim(const std::string &s) {
  static const std::string spaces = " \t\n\r\v";
  size_t b = s.find_first_not_of(spaces);
  size_t e = s.find_last_not_of(spaces);
  if (b == std::string::npos || e == std::string::npos) {
    return "";
  }
  return std::string(s, b, e - b + 1);
}

std::vector<std::string> split(const std::string &str,
  const std::string &delim, bool dotrim) {
  std::string s(str);
  std::vector<std::string> result;
  if (str == "")
    return result;
  size_t i;
  while ((i = s.find_first_of(delim)) != std::string::npos) {
    result.push_back(dotrim ? trim(s.substr(0, i)) : s.substr(0, i));
    s = s.substr(i + 1);
  }
  result.push_back(s);
  return result;
}
std::unique_ptr<xmlImputFiles::MinMaxRange<double>> make_range(const std::string& r_) {
  auto r = split(r_, ":", true);

  if (r.size() == 3) {
    return std::unique_ptr<xmlImputFiles::MinMaxRange<double>>(new xmlImputFiles::MinMaxRange<double>(stof(r[0]), stof(r[1]), stof(r[2])));
  }
  else if (r.size() == 2) {
    return  std::unique_ptr<xmlImputFiles::MinMaxRange<double>>( new xmlImputFiles::MinMaxRange<double>(stof(r[0]), stof(r[1])));
  }

  std::cout << "error processing range " << r_ << std::endl;
  return   std::unique_ptr<xmlImputFiles::MinMaxRange<double>>(); 

}
void drawResidual(s_process_collection& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_){
      p.DrawResidual(range_->getMin(), range_->getMax());

  } else {
      p.DrawResidual();

  }

}

void Draw_Track_hits(s_process_collection& p) {
  new TCanvas();
  p.Draw_Hit_map();
}
void Draw_DUT_hits(s_process_collection& p) {
  new TCanvas();
  p.Draw_DUT_Hits_map();
}
void draw_efficiency_map(s_process_collection& p) {
  new TCanvas();
  p.Draw_Efficinecy_map();
}

void Draw_missing_coordinate(s_process_collection& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_)
  {
    p.DrawResidualVsMissingCordinate(range_->getMin(),range_->getMax());
  } else {
    p.DrawResidualVsMissingCordinate();
  }
}
void Draw_Residual_VS_N(s_process_collection& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_) {
    p.DrawResidualVsEvent(range_->getMin(), range_->getMax());
  } else {
    p.DrawResidualVsEvent();
  }
}

struct  inParam {
  int argc;
  char **argv;
};
int asyncMain(void *arg) {

  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;

  CmdLine cmd("ProcessFile", ' ', "0.1");
  ValueArg<std::string> FileNameArg("i", "inFile", "xml filename", true, "", "string");
  cmd.add(FileNameArg);
  ValueArg<std::string>  inPath("p", "inPath", "path to the root files", true, "", "string");
  cmd.add(inPath);
  ValueArg<int> element("e", "element", "element of interest  in the XML file", true, 1, "int");
  cmd.add(element);

  ValueArg<std::string> residualRange("c", "residualCut", "range for the residual -r -10:10 => min =-10 , max = 10  ", false, "-10:10", "range");

  cmd.add(residualRange);

  TCLAP::SwitchArg res("r", "residualplot", "draws the residual plot");
  cmd.add(res);

  TCLAP::SwitchArg efficiency_map("m", "efficiencyMap", "draws the efficiency map");
  cmd.add(efficiency_map);

  TCLAP::SwitchArg ResVsN("n", "ResVsN", "draws the Residual vs Event Number");
  cmd.add(ResVsN);


  TCLAP::SwitchArg DUT_hits("d", "DUThitMap", "draws the DUT hitmap");

  cmd.add(DUT_hits);


  TCLAP::SwitchArg TrackHits("t", "TrackHitMap", "draws the Telescope hitmap");

  cmd.add(TrackHits);

  TCLAP::SwitchArg unknownCoordinate("u", "res_VS_unknown", "draws the residual VS the unknown coordinate");
  cmd.add(unknownCoordinate);

  TCLAP::SwitchArg DrawAll("a", "DrawAll", "DrawAllPlots");
  cmd.add(DrawAll);

  cmd.parse(argc, argv);  //terminates on error





  s_process_collection p;
  p.setPrintout(true);

  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 

  TFile * __file1 = new TFile("dummy.root", "recreate");

  p.Add_XML_RunList(FileNameArg.getValue(), inPath.getValue(), ".",element.getValue());
  std::unique_ptr<xmlImputFiles::MinMaxRange<double>> r;

  if (residualRange.isSet()) {
    r = make_range(residualRange.getValue());
  }
  TApplication theApp("App", &argc, argv);
  p.process();
  if (res.isSet() || DrawAll.isSet()) {

    drawResidual(p, r.get());
  }

  if (DUT_hits.isSet() || DrawAll.isSet()) {
    Draw_DUT_hits(p);
  }

  if (TrackHits.isSet() || DrawAll.isSet()) {
    Draw_Track_hits(p);
  }

  if (efficiency_map.isSet() || DrawAll.isSet()) {
    draw_efficiency_map(p);
  }
  if (ResVsN.isSet() || DrawAll.isSet()) {
    Draw_Residual_VS_N(p, r.get());
  }
  if (unknownCoordinate.isSet() || DrawAll.isSet()) {
    Draw_missing_coordinate(p, r.get());
  }


  new TBrowser();

  theApp.Run();

  return 0;
}
int asyncMainTest(void *arg) {
  inParam* para = static_cast<inParam *>(arg);
  int argc = para->argc;
  char **argv = para->argv;

  TApplication theApp("App", &argc, argv);
  TFile * file_ = new TFile("D:/data/DEVICE_1_ASIC_on_Position_7_Jim_350V/run000703-fitter.root");
  TFile * out_file = new TFile("output.root", "recreate");
  S_plot_collection pl(file_);
  pl.setOutputFile(out_file);
  auto res = pl.addPlot(sct_plot::residual(s_plot_prob("residual").SaveToDisk()), sct_coll::DUT_fitted_local().getX_def(), sct_coll::DUT_hit_local().getX_def());
  auto trueHits = pl.addPlot(sct_plot::coordinate_transform(1, 0, 1, 0, s_plot_prob().SaveToDisk()), sct_coll::DUT_fitted_local());
  auto corr = pl.addPlot(sct_plot::correlation(s_plot_prob("correlation").SaveToDisk()), trueHits().getX_def(), sct_coll::DUT_hit_local().getX_def());

  auto res_channel = pl.addPlot(sct_plot::residual(s_plot_prob("residual_channel").SaveToDisk()), trueHits().getX_def(), sct_coll::DUT_hit_local().getX_def());

  auto find_closest = pl.addPlot(sct_plot::find_nearest_strip(x_axis_def, 1, s_plot_prob("closest").SaveToDisk()), trueHits(), sct_coll::DUT_hit_local());
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
  pl.Draw(res_channel(), S_DrawOption().draw_x().cut_x(-0.2, 0.2));
  new TCanvas();
  pl.Draw(res_vs_missing(), S_DrawOption().draw_x_VS_y().cut_x(-0.2, 0.2));

  new TCanvas();
  TH1D h("trueHits", "true hits", 401, 0, 400);
  //pl.Draw(trueHits(), S_DrawOption().draw_x().output_object(&h));
  auto trueHIts_entries = pl.Draw(trueHits(), S_DrawOption().draw_y_VS_x());
    new TCanvas();
  TH1D h1("DUT_Hits", "DUT hits", 401, 0, 400);
  // pl.Draw(find_closest.get(1), S_DrawOption().draw_x().output_object(&h1));
  auto DUT_hits_entries = pl.Draw(find_closest.get(1), S_DrawOption().draw_y_VS_x());
  std::cout << DUT_hits_entries / (Double_t)trueHIts_entries * 100 << std::endl;
  //   auto h_eff = (TH1D*)h1.Clone("efficiency");
  //   h_eff->Divide(&h);
  //   new TCanvas();
  //   h_eff->Draw();
  theApp.Run();
  return 0;
}
int main(int argc, char **argv) {


  inParam para;
  para.argc = argc;
  para.argv = argv;
  std::cout << "press q to quit the program" << std::endl;
  std::thread thr(asyncMain, &para);
  thr.detach();
  std::string i;
  while (i!="q") {
    std::cin >> i;

  }
  

  return 0;


}


#include "s_process_files.h"

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







int ADDRun(s_process_files& p, std::string xmlInputFileName, std::string path__, int element, std::string outputPath = ".") {
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty()) {
    return -1;
  }
  auto collname = xml_imput.globalConfig().CollectionName();
  outputPath += "/" + collname + "_" + std::to_string(element) + ".root";

  p.setOutputName(outputPath.c_str());

  p.SetNumberOfBins(xml_imput.globalConfig().NumberOfBins());
  p.AddCut(xml_imput.globalConfig().cut());
  p.setActiveArea(xml_imput.globalConfig().AvtiveStrips().getMin(), xml_imput.globalConfig().AvtiveStrips().getMax());
  p.SetRotation(xml_imput.globalConfig().Rotation());
  p.SetPosition(xml_imput.globalConfig().Position_value(), 0);
  p.setResidualCut(xml_imput.globalConfig().residual_cut());

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
void drawResidual(s_process_files& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_){
      p.DrawResidual(range_->getMin(), range_->getMax());

  } else {
      p.DrawResidual();

  }

}

void Draw_Track_hits(s_process_files& p) {
  new TCanvas();
  p.Draw_Hit_map();
}
void Draw_DUT_hits(s_process_files& p) {
  new TCanvas();
  p.Draw_DUT_Hits_map();
}
void draw_efficiency_map(s_process_files& p) {
  new TCanvas();
  p.Draw_Efficinecy_map();
}

void Draw_missing_coordinate(s_process_files& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_)
  {
    p.DrawResidualVsMissingCordinate(range_->getMin(),range_->getMax());
  } else {
    p.DrawResidualVsMissingCordinate();
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


  TCLAP::SwitchArg DUT_hits("d", "DUThitMap", "draws the DUT hitmap");

  cmd.add(DUT_hits);


  TCLAP::SwitchArg TrackHits("t", "TrackHitMap", "draws the Telescope hitmap");

  cmd.add(TrackHits);

  TCLAP::SwitchArg unknownCoordinate("u", "res_VS_unknown", "draws the residual VS the unknown coordinate");

  cmd.add(unknownCoordinate);

  cmd.parse(argc, argv);  //terminates on error





  s_process_files p;


  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 

  TFile * __file1 = new TFile("dummy.root", "recreate");

  ADDRun(p, FileNameArg.getValue(), inPath.getValue(), element.getValue());
  std::unique_ptr<xmlImputFiles::MinMaxRange<double>> r;

  if (residualRange.isSet()) {
    r = make_range(residualRange.getValue());
  }
  TApplication theApp("App", &argc, argv);
  p.process();
  if (res.isSet()) {

    drawResidual(p, r.get());
  }

  if (DUT_hits.isSet()) {
    Draw_DUT_hits(p);
  }

  if (TrackHits.isSet()) {
    Draw_Track_hits(p);
  }

  if (efficiency_map.isSet()) {
    draw_efficiency_map(p);
  }
  if (unknownCoordinate.isSet()) {
    Draw_missing_coordinate(p, r.get());
  }
  new TBrowser();

  theApp.Run();

  return 0;
}
int main(int argc, char **argv) {


  inParam para;
  para.argc = argc;
  para.argv = argv;
  std::cout << "press q to quit the program" << std::endl;
  std::thread thr(asyncMain, &para);
  std::string i;
  while (i!="q") {
    std::cin >> i;

  }
  

  return 0;


}

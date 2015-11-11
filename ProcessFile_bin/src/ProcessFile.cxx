
#include "processorBase.h"

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
#include "internal/exceptions.hh"

using namespace xml_util;
using namespace TCLAP;
using namespace std;







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
void drawResidual(s_process_collection_standard& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_){
      p.DrawResidual(range_->getMin(), range_->getMax());

  } else {
      p.DrawResidual();

  }

}

void Draw_Track_hits(s_process_collection_standard& p) {
  new TCanvas();
  p.Draw_Hit_map();
}
void Draw_DUT_hits(s_process_collection_standard& p) {
  new TCanvas();
  p.Draw_DUT_Hits_map();
}
void draw_efficiency_map(s_process_collection_standard& p) {
  new TCanvas();
  p.Draw_Efficinecy_map();
}

void Draw_missing_coordinate(s_process_collection_standard& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
  new TCanvas();
  if (range_)
  {
    p.DrawResidualVsMissingCordinate(range_->getMin(),range_->getMax());
  } else {
    p.DrawResidualVsMissingCordinate();
  }
}
void Draw_Residual_VS_N(s_process_collection_standard& p, const xmlImputFiles::MinMaxRange<double> * range_ = nullptr) {
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
  try {
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





    s_process_collection_standard p;
    p.setPrintout(true);

    gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 

    TFile * __file1 = new TFile("dummy.root", "recreate");

    p.Add_XML_RunList(FileNameArg.getValue(), inPath.getValue(), ".", element.getValue());
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
  }
  catch (...) {
    return sct_corr::handleExceptions();
  }
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

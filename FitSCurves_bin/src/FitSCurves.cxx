#include "TFile.h"
#include "TApplication.h"
#include "TBrowser.h"




#include <iostream>
#include "TCanvas.h"
#include "TMath.h"
#include "TH2.h"
#include <vector>
#include "TProfile.h"
#include "TFitResult.h"
#include "TF1.h"
#include "TBenchmark.h"
#include "s_plane_def.h"
#include "s_plot_collection.h"
#include "s_DrawOption.h"
#include "s_cuts.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "s_process_collection.h"

#include "landgausFit.h"
#include <algorithm>
#include <fstream>
#include "tclap/CmdLine.h"
#include "tclap/ArgException.h"
#include <string>
#include "sct_events/rootEventBase.hh"
#include "sct_events/rootEventRunOutput.hh"
#include "treeCollection.h"
#include "TTree.h"
#include "SCT_helpers.h"
#include "scurve_fit_collection.hh"








using namespace std;
using namespace TCLAP;
int main(int argc, char **argv) {
#ifdef _DEBUG
  int argc_asdasddsa = 1;
  char **argv_sadsda = new char*[1];
  argv_sadsda[0] = "dummy";
  TApplication theApp("App", &argc_asdasddsa, argv_sadsda);

#endif // _DEBUG


  try {

    CmdLine cmd("SCurve Fit", ' ', "0.1");
    ValueArg<std::string> FileNameArg("f", "inFile", "filename", true, "", "string");
    cmd.add(FileNameArg);
    ValueArg<std::string> outFilename("o", "outFile", "output filename", false, "", "string");
    cmd.add(outFilename);
    ValueArg<double> MPV_arg("m", "mpv", "start value for the landau gauss fit", false, 80, "double");
    cmd.add(MPV_arg);
    cmd.parse(argc, argv);



    std::string inFile = FileNameArg.getValue();

    std::string outfile;
    if (!outFilename.isSet()) {
      outfile = inFile.substr(0, inFile.size() - 5);
      outfile += "_scurve.txt";
    } else {
      outfile = outFilename.getValue();
    }
    std::cout << outfile << std::endl;

    TFile _file0(inFile.c_str());
    if (!_file0.IsOpen()) {
      std::cout << "Unable to Open the File    \n ";
      return -3;
    }

    TTree* noise = (TTree*)_file0.Get("hitmap");
    TTree* Hits = (TTree*)_file0.Get("out");

    if (!noise&&!Hits) {
      std::cout << "TTree not found " << std::endl;
      return -2;

    }
    scurve_fit_collection p(outfile.c_str());
    p.setStartMPV(MPV_arg.getValue());
    if (noise) {
      p.setNoiseRun(noise);
    } else if (Hits) {
      p.setBeamRuns(Hits);
    }
    gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
    p.processTotal("total_efficiency:Threshold:error_efficiency");
    p.processStrip("Occupancy:Threshold:Occupancy_error", x_axis_def, 1, 400);
#ifdef _DEBUG
    theApp.Run();
#endif
    return 0;
  } catch (ArgException &e)  // catch any exceptions
  {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return -1;
  }

  return 0;
}

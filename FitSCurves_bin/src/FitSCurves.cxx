#include "TFile.h"
#include "TApplication.h"
#include "TBrowser.h"



#include "sct_plots.h"
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


double sum(const std::vector<double> & vec) {
  double N_event = 0;
  for (const auto&e : vec) {
    N_event += e;
  }
  return N_event;
}
class getScurvesFromTTree {
public:
  getScurvesFromTTree(TTree* tree) : m_tree(tree) {
    tree_pointer = new sct_corr::treeCollection(m_tree);
    tree->SetBranchAddress("Threshold", &m_Threshold_out);
    tree_pointer->GetEntry(0);
    m_max_index = tree_pointer->m_rootBuffer.getData(x_axis_def)->size();

    m_Threshold.resize(tree_pointer->GetEntries());
    m_Ocuupancy.resize(m_max_index);
    m_Occuoancy_error.resize(m_max_index);
    m_total_hits.resize(m_max_index);
    x_pos.resize(m_max_index);



    for (Int_t i = 0; i < tree_pointer->GetEntries(); ++i) {

      tree_pointer->GetEntry(i);
      for (size_t index = 0; index < m_max_index; ++index) {
        m_Threshold[i] = m_Threshold_out;
        m_Ocuupancy[index].push_back(tree_pointer->m_rootBuffer.getData(Occupancy_axis_def)->at(index));
        m_Occuoancy_error[index].push_back(tree_pointer->m_rootBuffer.getData(Occupancy_error_axis_def)->at(index));
        m_total_hits[index].push_back(tree_pointer->m_rootBuffer.getData("NumOfEvents")->at(index));
        x_pos[index] = tree_pointer->m_rootBuffer.getData(x_axis_def)->at(index);


      }

    }
  }

  size_t size() const {
    return m_max_index;
  }
  Long_t set_pos(size_t index) {

    m_index = index;

    return 1;

  }
  double getX() const {
    return x_pos.at(m_index);
  }
  const std::vector<double> * get_Threshold() const {
    return &m_Threshold;
  }
  const std::vector<double> * get_Ocuupancy() const {
    return &m_Ocuupancy.at(m_index);
  }
  const std::vector<double> * get_Occuoancy_error() const {

    return &m_Occuoancy_error.at(m_index);
  }
  const std::vector<double> * get_total_hits() const {

    return &m_total_hits.at(m_index);
  }
private:
  double m_Threshold_out = 0;
  std::vector<double> x_pos, m_Threshold;
  size_t m_index, m_max_index;
  TTree* m_tree = nullptr;
  sct_corr::treeCollection* tree_pointer = nullptr;
  std::vector<std::vector<double>> m_Ocuupancy, m_Occuoancy_error,m_total_hits;
};
class process_collection {
public:
  process_collection(const char* outputFileName) : m_out(outputFileName) {

    setStartValues();
  }
  void setTree(TTree* tree) {
    m_tree = tree;
    m_tree_sc_curve = std::make_shared<getScurvesFromTTree>(tree);
  }
  void setBeamRuns(TTree* noise) {
    m_use_total_efficiency_fit_as_start = true;
    m_start_amp = 1;
    if (!m_use_total_efficiency_fit_as_start) {
      m_start_mean = 110;
    }
    m_start_gaus_sigma = 52;
    m_start_landau_sigma = 10;
    m_gaus_sigma_lower_boundary = 20;
    m_threshold_Cut = "Threshold>=25";
    setTree(noise);
    setStartValues();
  }
  void setNoiseRun(TTree* hits) {
    m_start_amp = 1;
    m_start_mean = 8;
    m_start_gaus_sigma = 4;
    m_start_landau_sigma = 0.4;
    m_gaus_sigma_lower_boundary = 1;
    m_use_total_efficiency_fit_as_start = false;

    m_threshold_Cut = "Threshold>=0";
    setTree(hits);
    setStartValues();
  }
  void setStartValues() {
    f.setFitRange(0, 600);
    f.setLimits_Amplitude(0.7, 1.0);
    f.setStartAmplitude(m_start_amp);
    f.setStartGaussSigma(m_start_gaus_sigma);
    f.setStartLandauMean(m_start_mean);
    f.setStartLandauSigma(m_start_landau_sigma);
    f.setLimits_GaussSigma(m_gaus_sigma_lower_boundary, 1000);

  }
  void check() {
    if (f.getLandauSigma() == m_gaus_sigma_lower_boundary) {
      std::cout << "fit hit boundary \n";
    }
  }
  void push_to_file() {
    m_out << m_x_pos << ";  " << f.getAmplitude() << ";  " << f.getLandauMostProbable() << ";   " << f.getLandauSigma() << ";   " << f.getGaussSigma() << ";  " << f.getChiSqare() << std::endl;

  }
  void push_to_file_emtpyEvent() {
    m_out << m_x_pos << ";  " << 0 << ";  " << 0 << ";   " << 0 << ";   " << 0 << ";   " << 0 << std::endl;

  }
  void setStartMPV(double mpv) {

    m_start_mean = mpv;
    m_use_total_efficiency_fit_as_start = false;
  }

  ////////////////PROCESS SCURVE EFFICIENCY VS THRESHOLD
  void processTotal(const char * total_name) {
    TCanvas *canv = new TCanvas("canv", "", 500, 500);
    setStartValues();
    SCT_helpers::DrawTTree(
      m_tree,
      S_DrawOption()
      .output_object(&g)
      .draw_axis(total_name)
      .opt_bar()
      .cut(m_threshold_Cut.c_str())
      );

    f(&g);
    f.DrawfitFunction();
    push_to_file();
    if (m_use_total_efficiency_fit_as_start) {
      m_start_amp = f.getAmplitude();

      m_start_mean = f.getLandauMostProbable();
    }
    m_start_gaus_sigma = f.getGaussSigma();
    m_start_landau_sigma = f.getLandauSigma();
  }
  double get_startPoint(const std::vector<double> & thr, const std::vector<double> & occ) {
    const double occ_begin = occ[0];
    for (size_t i = 0; i < occ.size(); ++i)
    {
      if (occ.at(i) < 0.5 * occ_begin)
      {
        return thr.at(i);
      }

    }
    return -1;
  }

  /////////PROCESS SINGLE STRIP EFF VS THRESHOLD
  void processStrip(const char* stripName, axis_def axis__, double min__, double max__) {
    m_tree_sc_curve->set_pos(m_tree_sc_curve->size() - 1);
    const double max_x = m_tree_sc_curve->getX();
    double dec = 0;
    const auto max_index = m_tree_sc_curve->size();
    for (size_t i = 0; i < max_index; ++i) {
      m_tree_sc_curve->set_pos(i);
      if (m_tree_sc_curve->getX() > dec) {
        std::cout << "process channel: " << m_tree_sc_curve->getX() << " of " << max_x << std::endl;
        dec += 10;
      }
      auto thr = m_tree_sc_curve->get_Threshold();
      auto occ = m_tree_sc_curve->get_Ocuupancy();
      m_x_pos = m_tree_sc_curve->getX();
      double N_event = sum(*m_tree_sc_curve->get_total_hits());
   //   std::cout << N_event << std::endl;
      if (N_event<1000)
      {
        push_to_file_emtpyEvent();
        continue;
      }
      auto d = std::max_element(occ->begin(), occ->end());
      if (*d < 0.5) {
        push_to_file_emtpyEvent();
        continue;
      }
      if (occ->empty()) {
        push_to_file_emtpyEvent();
        continue;
      }

      TGraph g(thr->size(), thr->data(), occ->data());


      const auto mpv_start= get_startPoint(*thr, *occ);
      if (mpv_start<280)
      {
        push_to_file_emtpyEvent();
        continue;
      }
      f.setStartLandauMean(mpv_start);
      f(&g);
    //  f.printResults();
    //  f.DrawfitFunction();

      push_to_file();

    }
  }

  TTree *m_tree = nullptr;
  double m_start_amp = 1,
    m_start_mean = 8.79884,
    m_start_gaus_sigma = 30,
    m_start_landau_sigma = 0.386907,
    m_gaus_sigma_lower_boundary = 10,
    m_x_pos=0;
  bool m_use_total_efficiency_fit_as_start = true;
  TGraphErrors g;
  TGraphErrors pluto;
  TCanvas m_c;
  landgausFit f;
  std::ofstream m_out;
  std::string m_threshold_Cut;
  std::vector<double> m_axis;
  std::shared_ptr<getScurvesFromTTree> m_tree_sc_curve;
};
using namespace std;
using namespace TCLAP;
int main(int argc, char **argv) {
  int argc_asdasddsa = 2;
  char **argv_sadsda = new char*[] {"adad", "sadas"};
#ifdef _DEBUG
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
    process_collection p(outfile.c_str());
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

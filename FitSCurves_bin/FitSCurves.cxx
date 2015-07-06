#include "TFile.h"
#include "TApplication.h"
#include "TBrowser.h"



#include "../sct_plot/inc/sct_plots.h"
#include <iostream>
#include "TCanvas.h"
#include "TMath.h"
#include "TH2.h"
#include <vector>
#include "TProfile.h"
#include "TFitResult.h"
#include "TF1.h"
#include "TBenchmark.h"
#include "../sct_plot/inc/s_plane.h"
#include "../sct_plot/inc/s_plot_collection.h"
#include "../sct_plot/inc/s_DrawOption.h"
#include "../sct_plot/inc/s_cuts.h"
#include "TGraph.h"
#include "../sct_plot/inc/s_process_files.h"
#include "../sct_plot/inc/sct_runs.h"
#include "landgausFit.h"
#include <algorithm>
#include <fstream>



class process_collection{
public:
  process_collection(const char* outputFileName) : m_out(outputFileName){

    setStartValues();
  }
  void setBeamRuns(TTree* noise){
    m_use_total_efficiency_fit_as_start = true;
    m_start_amp = 1;
    m_start_mean = 110;
    m_start_gaus_sigma =  30;
    m_start_landau_sigma = 2.3;
    m_gaus_sigma_lower_boundary = 10;
    m_tree = noise;
    m_threshold_Cut = "Threshold>=25";
    setStartValues();
  }
  void setNoiseRun(TTree* hits){
      m_start_amp = 1;
      m_start_mean = 8;
      m_start_gaus_sigma = 4;
      m_start_landau_sigma = 0.4;
      m_gaus_sigma_lower_boundary = 1;
      m_use_total_efficiency_fit_as_start = false;
      m_tree = hits;
      m_threshold_Cut = "Threshold>=0";
      setStartValues();
  }
  void setStartValues(){

    f.setStartAmplitude(m_start_amp);
    f.setStartGaussSigma(m_start_gaus_sigma);
    f.setStartLandauMean(m_start_mean);
    f.setStartLandauSigma(m_start_landau_sigma);
    f.setLimits_GaussSigma(m_gaus_sigma_lower_boundary, 1000);
    
  }
  void check(){
    if (f.getLandauSigma() == m_gaus_sigma_lower_boundary)
    {
      std::cout << "fit hit boundary \n";
    }
  }
  void push_to_file(){
    m_out << f.getAmplitude() << ";  " << f.getLandauMostProbable() << ";   " << f.getLandauSigma() << ";   " << f.getGaussSigma() << ";  " << f.getChiSqare() << std::endl;

  }
  void push_to_file_emtpyEvent(){
    m_out << 0 << ";  " << 0 << ";   " << 0 << ";   " << 0 << ";   " << 0 << std::endl;

  }
  void setStartMPV(double mpv){

    m_start_mean = mpv;
    m_use_total_efficiency_fit_as_start = false;
  }
  void processTotal(const char * total_name){
    SCT_helpers::DrawTTree(m_tree, S_DrawOption().output_object(&g).draw_axis(total_name).opt_star());
    f(&g);
    f.printResults();
    if (m_use_total_efficiency_fit_as_start)
    {
      m_start_amp = f.getAmplitude();
      m_start_gaus_sigma = f.getGaussSigma();
      m_start_landau_sigma = f.getLandauSigma();
      m_start_mean = f.getLandauMostProbable();
    }

    push_to_file();
    setStartValues();
  }
  void processStrip(const char* stripName, axis_def x, double min__, double max__){

    for (double i = min__; i < max__; ++i)
    {
      if ((int)i%10==0)
      {
        std::cout << "process channel: " << i << " of " << max__ << std::endl;
      }
      setStartValues();
      SCT_helpers::DrawTTree(m_tree, S_DrawOption().output_object(&g).draw_axis("Occupancy:Threshold").opt_star().cut(x, i - 0.5, i + 0.5));
      if (g.GetN() == 0)
      {
        push_to_file_emtpyEvent();
        continue;
      }
      auto d = std::max_element(g.GetY(), g.GetY() + g.GetN());

      if (*d < 0.5)
      {
        push_to_file_emtpyEvent();
        continue;
      }
      f(&g);
     // f.printResults();
      push_to_file();
    }

  }

  TTree *m_tree = nullptr;
  double m_start_amp = 1,
    m_start_mean = 8.79884,
    m_start_gaus_sigma = 30,
    m_start_landau_sigma = 0.386907,
    m_gaus_sigma_lower_boundary = 10;
  bool m_use_total_efficiency_fit_as_start = true;
  TGraph g;
  landgausFit f;
  std::ofstream m_out;
};

int main(int argc, char **argv) {

 // TApplication theApp("App", &argc, argv);
  if (argc!=2)
  {
    std::cout << "wrong input arguments  \n \n ";
    std::cout << "FitSCurves example.root" << std::endl;
    return -1;
  }
  std::string inFile(argv[1]);
  std::cout << "opening file: " << inFile << std::endl;
  TFile* _file0 = new TFile(argv[1]);

  auto outfile =inFile.substr(0, inFile.size() - 5);
  outfile += "_scurve.txt";
  std::cout << outfile << std::endl;
  landgausFit f;
  TTree* noise = (TTree*)_file0->Get("hitmap");
  TTree* Hits = (TTree*)_file0->Get("out");
  if (!noise&&!Hits)
  {
    std::cout << "TTree not found " << std::endl;
    return -2;
    
  }



  TCanvas c;
  
  process_collection p(outfile.c_str());


  if (noise)
  {
    p.setNoiseRun(noise);
  }
  else if (Hits)
  {
    p.setBeamRuns(Hits);
  }

  p.processTotal("total_efficiency:Threshold");
  p.processStrip("Occupancy:Threshold", x_axis_def, 0, 400);

  return 0;
}

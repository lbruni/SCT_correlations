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
#include "s_plane.h"
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


class process_collection{
public:
    process_collection(const char* outputFileName) : m_out(outputFileName){
        
        setStartValues();
    }
    void setBeamRuns(TTree* noise){
        m_use_total_efficiency_fit_as_start = true;
        m_start_amp = 1;
        if (!m_use_total_efficiency_fit_as_start) {
          m_start_mean = 110;
        }
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
        f.setFitRange(0, 600);
        f.setLimits_Amplitude(0.7, 1.0);
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
    
    ////////////////PROCESS SCURVE EFFICIENCY VS THRESHOLD
    void processTotal(const char * total_name){
        TCanvas *canv = new TCanvas("canv","",500,500);
        setStartValues();
        SCT_helpers::DrawTTree(m_tree, S_DrawOption().output_object(&g).draw_axis(total_name).opt_bar().cut(m_threshold_Cut.c_str()));
       
        f(&g);
        f.DrawfitFunction();
        push_to_file();
        if (m_use_total_efficiency_fit_as_start)
        {
            m_start_amp = f.getAmplitude();
            
            m_start_mean = f.getLandauMostProbable();
        }
        m_start_gaus_sigma = f.getGaussSigma();
        m_start_landau_sigma = f.getLandauSigma();
          
 

    
        

    }
    
    /////////PROCESS SINGLE STRIP EFF VS THRESHOLD
    void processStrip(const char* stripName, axis_def x, double min__, double max__){
        
        for (double i = min__; i < max__; ++i)
        {
            if ((int)i%10==0)
            {
                std::cout << "process channel: " << i << " of " << max__ << std::endl;
            }
            setStartValues();
            SCT_helpers::DrawTTree(m_tree, S_DrawOption().output_object(&g).draw_axis("Occupancy:Threshold:Occupancy_error").opt_star().cut(m_threshold_Cut.c_str()).cut(x, i - 0.5, i + 0.5));
            if (g.GetN() == 0)
            {
                push_to_file_emtpyEvent();
                continue;
            }
            auto d = std::max_element(g.GetY()+1, g.GetY() + g.GetN());
            
            if (*d < 0.5)
            {
                push_to_file_emtpyEvent();
                continue;
            }
            
            f(&g);
            f.printResults();
             f.DrawfitFunction();
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
    TGraphErrors g;
    TGraphErrors pluto;
    TCanvas m_c;
    landgausFit f;
    std::ofstream m_out;
    std::string m_threshold_Cut;
};
using namespace std;
using namespace TCLAP;
int main(int argc, char **argv) {
  int argc_asdasddsa = 2; 
  char **argv_sadsda = new char*[] {"adad", "sadas"};
  TApplication theApp("App", &argc_asdasddsa, argv_sadsda);
    
    try {
        
        CmdLine cmd("SCurve Fit", ' ', "0.1");
        ValueArg<std::string> FileNameArg("f", "inFile", "filename", false, "", "string");
        cmd.add(FileNameArg);
        ValueArg<std::string> outFilename("o", "outFile", "output filename", false, "", "string");
        cmd.add(outFilename);
        ValueArg<double> MPV_arg("m", "mpv", "start value for the landau gauss fit", false, 80, "double");
        cmd.add(MPV_arg);
        cmd.parse(argc, argv);
        
        
        
        std::string inFile = FileNameArg.getValue();
        
        std::string outfile;
        if (!outFilename.isSet()){
            outfile = inFile.substr(0, inFile.size() - 5);
            outfile += "_scurve.txt";
        }
        else{
            outfile = outFilename.getValue();
        }
        std::cout << outfile << std::endl;
        
        TFile _file0(inFile.c_str());
        if (!_file0.IsOpen())
        {
            std::cout << "Unable to Open the File    \n ";
            return -3;
        }
        
        TTree* noise = (TTree*)_file0.Get("hitmap");
        TTree* Hits = (TTree*)_file0.Get("out");
        
        if (!noise&&!Hits)
        {
            std::cout << "TTree not found " << std::endl;
            return -2;
            
        }
        process_collection p(outfile.c_str());
        p.setStartMPV(MPV_arg.getValue());
        if (noise)
        {
            p.setNoiseRun(noise);
        }
        else if (Hits)
        {
            p.setBeamRuns(Hits);
        }
        gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
       p.processTotal("total_efficiency:Threshold:error_efficiency");
       p.processStrip("Occupancy:Threshold:Occupancy_error", x_axis_def, 1, 400);
       theApp.Run();
        return 0;
    }
    catch (ArgException &e)  // catch any exceptions
    {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return -1;
    }

    return 0;
}

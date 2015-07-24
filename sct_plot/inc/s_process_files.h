#ifndef s_process_files_h__
#define s_process_files_h__
#include "Rtypes.h"
#include "internal/platform.hh"
#include "TH1.h"

#ifndef __CINT__
#include <memory>
#include "s_plot_collection.h"
#include "s_cuts.h"
#include "sct_events/rootEventRunOutput.hh"
#include "sct_event_buffer.h"
#include "s_plane.h"

namespace sct_corr{
  class treeCollection_ouput;
}
#endif
#include "TTree.h"
#include "TH2.h"

class DllExport s_process_files{
public:
  s_process_files();
  ~s_process_files();
  void setOutputName(const char* name);
  void push_files(TFile* _file, double Threshold, double runNumber);
  void push_files(const char* _fileName, double Threshold, double runNumber,double HV);
  void AddCut(const S_Cut& cut);
  void SetPitchSize(Double_t pitchSize);
  void SetRotation(Double_t rotation);
  void SetPosition(Double_t x_pos, Double_t y_pos);
  void setActiveArea(Double_t x_min, Double_t x_max);
  void SetNumberOfBins(Int_t bins);
  void setResidualCut(Double_t residualCut);

  bool process();
  Int_t DrawResidual(Double_t min_X,Double_t max_X);
  Int_t DrawResidual();
  Int_t DrawResidualVsMissingCordinate(Double_t min_X, Double_t max_X);
  Int_t Draw_Efficinecy_map();
  Int_t Draw_Hit_map();
  Int_t Draw_DUT_Hits_map();
  TH2D* getResidualVsMissingCordinate();
#ifndef __CINT__
  void extract_hitMap();
  bool process(TFile* file);
    void pushChannel(Double_t channel_x, Double_t channel_y, Double_t Effi, Double_t NumberOfEvents, Double_t Effi_error);
  Double_t m_rotation = 0,
    m_pitchSize = 0.0742,
    m_pos_x = 0,
    m_pos_y = 0,
    m_active_area_x_min=0,
    m_active_area_x_max=0,
    m_residual_cut= 1000;
  Int_t m_bins;
  S_CutCoollection m_cuts;
  s_plane_collection m_output_planes;
  std::shared_ptr<S_plot_collection> m_plotCollection;
  class FileProberties{
  public:
    TFile* getTfile() const{
      if (m_fileOwnd){
        return m_fileOwnd.get();
      }

      if (m_file)
      {
        return m_file;
      }
      return nullptr;
    }
    void setTFile(TFile* file){
      m_file = file;
    }
    void setTFile(std::shared_ptr<TFile> file){
      m_fileOwnd = file;
    }
    double m_Threshold = 0;
    double m_runNumber = 0;
    double m_HV = 0;
  private:
    std::shared_ptr<TFile> m_fileOwnd;
    TFile* m_file =nullptr;
  };
  std::vector<FileProberties> m_files;
  sct_corr::rootEventRunOutput m_outputl;
  std::shared_ptr<sct_corr::treeCollection_ouput> m_outputTree;
  std::shared_ptr<TH1D> m_Residual;
  std::shared_ptr<TH1D> m_Hits_total;
  std::shared_ptr<TH1D> m_Hits_with_DUT_Hits;
  std::shared_ptr<TH1D> m_Efficieny_map;
  std::shared_ptr<TH2D> m_resVSMissing;
  sct_corr::sct_event_buffer m_buffer;
  TFile* m_dummy = nullptr;
  TFile* m_outpuFile = nullptr;
  std::string m_outname;
#endif
  ClassDef(s_process_files, 0);
};


#ifdef __CINT__

#pragma link C++ class s_process_files;
#endif
#endif // s_process_files_h__

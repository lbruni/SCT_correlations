#ifndef s_process_files_h__
#define s_process_files_h__
#include "Rtypes.h"
#include "internal/platform.hh"
#include "TH1.h"


#include <memory>
#include "s_plot_collection.h"
#include "s_cuts.h"
#include "sct_events/rootEventRunOutput.hh"
#include "sct_event_buffer.h"
#include "s_plane_def.h"

namespace sct_corr{
  class treeCollection_ouput;
  struct Xgear;
}
namespace sct_files {
  class fitter_file;
}

namespace xmlImputFiles {
  class XML_imput_file;
}
class FileProberties {
public:
  TFile* getTfile() const;
  void setTFile(TFile* file);
  void setTFile(std::shared_ptr<TFile> file);
  double m_Threshold = 0;
  double m_runNumber = 0;
  double m_HV = 0;
private:
  std::shared_ptr<TFile> m_fileOwnd;
  TFile* m_file = nullptr;
};

#include "TTree.h"
#include "TH2.h"


class DllExport s_process_collection{
public:
  s_process_collection();
  virtual ~s_process_collection();
  void setOutputName(const char* name);
  void push_files(TFile* _file, double Threshold, double runNumber);
  void push_files(const char* _fileName, double Threshold, double runNumber,double HV);

  int Add_XML_RunList(const std::string& xmlInputFileName, std::string path__, std::string outputPath = ".", int element = -1);

  void setGearFile(const char* name);
  void setPrintout(bool print);
  bool process();



  const xmlImputFiles::XML_imput_file* get_xml_input() const;
  const sct_corr::Xgear* get_gear() const;
private:

  std::shared_ptr<xmlImputFiles::XML_imput_file> m_input_files_xml;
   


  std::shared_ptr<sct_corr::Xgear> m_gear;


  virtual void start_collection(TFile* file__) = 0;
  virtual  bool process_file(FileProberties* fileP) = 0;



  std::vector<FileProberties> m_files;



  TFile* m_outpuFile = nullptr;
  std::string m_outname;




};
class DllExport s_process_collection_standard :public s_process_collection {
public:
  s_process_collection_standard();
  virtual ~s_process_collection_standard();
  Long64_t DrawResidual(Double_t min_X, Double_t max_X);
  Long64_t DrawResidual();
  Long64_t DrawResidualVsEvent(Double_t min_X, Double_t max_X);
  Long64_t DrawResidualVsEvent();
  Long64_t DrawResidualVsMissingCordinate(Double_t min_X, Double_t max_X);
  Long64_t DrawResidualVsMissingCordinate();
  Long64_t Draw_Efficinecy_map();
  Long64_t Draw_Hit_map();
  Long64_t Draw_DUT_Hits_map();
  TH2D* getResidualVsMissingCordinate();

private:
  virtual void start_collection(TFile* file__) override;
  virtual  bool process_file(FileProberties* fileP) override;
  void process_set_run_prob(const FileProberties& fileP);
  void extract_efficiency();
  void extract_hitMap();
  void extract_residual();
  void extract_rotation();
  void process_reset();
  void pushChannel(Double_t channel_x, Double_t channel_y, Double_t Effi, Double_t NumberOfEvents, Double_t Effi_error);
  sct_corr::rootEventRunOutput m_outputl;
  std::shared_ptr<sct_corr::treeCollection_ouput> m_outputTree;
  sct_corr::sct_event_buffer m_buffer;
  
  
  std::shared_ptr<TH1D> m_Residual;
  std::shared_ptr<TH1D> m_Hits_total;
  std::shared_ptr<TH1D> m_Hits_with_DUT_Hits;
  std::shared_ptr<TH1D> m_Efficieny_map;
  std::shared_ptr<TH1D> m_Efficieny_trueHits;
  std::shared_ptr<TH2D> m_resVSMissing;
  std::shared_ptr<TH2D> m_ResidualVsEvent;

  s_plane_collection m_res_VS_event;
  s_plane_collection_correlations m_output_planes;

  std::shared_ptr<sct_corr::plot_collection> m_plotCollection;
  std::shared_ptr <sct_files::fitter_file> m_file_fitter;
  TFile* m_dummy = nullptr;


};

class DllExport s_process_collection_modulo : public s_process_collection {
public:
  s_process_collection_modulo();
  virtual ~s_process_collection_modulo() ;


private:
  virtual void start_collection(TFile* file__) override;
  virtual  bool process_file(FileProberties* fileP) override;
  std::shared_ptr<sct_corr::treeCollection_ouput> m_outputTree;
  sct_corr::sct_event_buffer m_buffer;
  sct_corr::rootEventRunOutput m_outputl;
  TFile* m_dummy = nullptr;

  std::shared_ptr<sct_corr::plot_collection> m_plotCollection;
  std::shared_ptr<sct_files::fitter_file> m_file_fitter;
  std::shared_ptr<sct_corr::inStripEfficiency> m_instripEfficiency ;
  std::shared_ptr<sct_corr::inStripClusterSize> m_instripClusterSize ;
  std::shared_ptr<sct_corr::residual_efficienct> m_residualEffieciency ;
};


#endif // s_process_files_h__

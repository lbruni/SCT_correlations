#include "s_process_collection.h"
#include "TFile.h"

#include "s_plane_def.h"
#include <string>
#include <iostream>
#include "treeCollection.h"
#include "TF1.h"
#include "TCanvas.h"
#include "geometry/setup_description.hh"
#include "xml_helpers/xml_fileList.hh"
#include "SCT_helpers.h"
#include "s_file_base.h"
#include "sct_processors.h"
#include "internal/hit_efficiency.hh"
#include "internal/inStripClusterSize.hh"
#include "internal/residual_efficienct.hh"
#include "internal/inStripEfficiency.hh"
#include "sct_types.h"


bool gDo_print = false;
int gPos = 0;
using namespace sct_type;
std::ostream* m_out = &std::cout;
class xml_print {

public:
  static void setOutPutStream(std::ostream& out) {
    m_out = &out;
  }
  xml_print(const char* name) :m_name(name), m_pos(gPos) {
    gPos += 2;
    if (!gDo_print) {
      return;
    }
    *m_out << std::string(m_pos, ' ') << "<" << m_name << ">" << std::endl;
  }

  template<typename T>
  xml_print(const char* name, const T& value) :xml_print(name) {
    print(value);
    close();
  }
  template<typename T>
  void print(const T& value) {
    if (!gDo_print) {
      return;
    }
    if (first_element) {
      auto s = std::string(m_pos + 2, ' ');
      *m_out << s << value << std::endl;
      first_element = false;
    } else {

      *m_out << std::string(m_pos + 2, ' ') << ", " << value << std::endl;
    }


  }
  virtual ~xml_print() {
    if (!end_printed) {
      close();
    }


  }
  void close() {
    gPos -= 2;
    if (!gDo_print) {
      return;
    }

    *m_out << std::string(m_pos, ' ') << "</" << m_name << ">" << std::endl;
    end_printed = true;
  }
private:

  std::string m_name;
  bool end_printed = false, first_element=true;
  int m_pos;
};
double BinNomialSigma(double totalHits, double DUTHits) {
  return sqrt((DUTHits / totalHits)*(1 - (DUTHits / totalHits))*(1 / totalHits));
}
s_process_collection::s_process_collection() {


 


}


s_process_collection::~s_process_collection() {


}

void s_process_collection::setOutputName(const char* name) {
  m_outname = name;
}

void s_process_collection::push_files(TFile* _file, double Threshold, double runNumber) {
  FileProberties p;
  p.setTFile(_file);
  p.m_Threshold = Threshold;
  p.m_runNumber = runNumber;
  m_files.push_back(p);
}

void s_process_collection::push_files(const char* _fileName, double Threshold, double runNumber, double HV) {
  FileProberties p;
  p.setTFile(std::shared_ptr<TFile>(new TFile(_fileName)));
  if (!p.getTfile()->IsOpen()) {

    return;
  }

  p.m_Threshold = Threshold;
  p.m_runNumber = runNumber;
  p.m_HV = HV;
  m_files.push_back(p);

}



int s_process_collection::Add_XML_RunList(const std::string& xmlInputFileName, std::string path__, std::string outputPath /*= "."*/, int element /* = -1*/) {
  path__ += "/";
  m_input_files_xml = std::make_shared<xmlImputFiles::XML_imput_file>(xmlInputFileName.c_str());

  if (m_input_files_xml->fileList().empty()) {
    return -1;
  }
  auto collname = m_input_files_xml->globalConfig().CollectionName();
  if (element != -1) {
    outputPath += "/" + collname + "_" + std::to_string(element) + ".root";
  } else {
    outputPath += "/" + collname + ".root";
  }


  setOutputName(outputPath.c_str());


  setGearFile(m_input_files_xml->globalConfig().gearFile().c_str());


  if (element != -1) {
    if (element >= (int)m_input_files_xml->fileList().size()) {
      return -1;
    }
    auto& e = m_input_files_xml->fileList()[element];
    push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());

  } else {
    for (auto& e : m_input_files_xml->fileList()) {
      push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());
    }

  }


  return 0;
}



void s_process_collection::setGearFile(const char* name) {

  rapidxml::file<> m_file(name);
  rapidxml::xml_document<> m_doc;
  m_doc.parse<0>(m_file.data());

  m_gear = std::make_shared<sct_corr::Xgear>(m_doc.first_node("gear"));
}

void s_process_collection::setPrintout(bool print) {
  gDo_print = print;
}

void s_process_collection_standard::extract_efficiency() {

  double totalHits = (double)m_plotCollection->Draw(m_output_planes.getTotalTrueHits(),
                                                    S_DrawOption()
                                                    .cut_x(get_xml_input()->globalConfig().AvtiveStrips().getMin(),
                                                    get_xml_input()->globalConfig().AvtiveStrips().getMax())
                                                    );

  xml_print("TotalNumOfEvents", totalHits);
  m_outputl.set_TotalNumOfEvents(totalHits);

  double DUTHits = (double)m_plotCollection->Draw(m_output_planes.getTrueHitsWithDUT(),
                                                  S_DrawOption()
                                                  .cut_x(get_xml_input()->globalConfig().AvtiveStrips().getMin() ,
                                                  get_xml_input()->globalConfig().AvtiveStrips().getMax())
                                                  );
  xml_print("DUTHits", DUTHits);

  xml_print("Efficiency", DUTHits / totalHits);
  m_outputl.set_Total_efficiency(DUTHits / totalHits);


  auto Error_efficiency = BinNomialSigma(totalHits,
                                         DUTHits);

  xml_print("Error_efficiency", Error_efficiency);
  m_outputl.set_Error_efficiency(Error_efficiency);


}

void s_process_collection_standard::extract_hitMap() {

  for (Int_t i = 0; i < m_Efficieny_map->GetNbinsX(); ++i) {
    pushChannel(m_Efficieny_map->GetBinCenter(i), //xPosition
                1,                                //yPosition   
                m_Efficieny_map->GetBinContent(i), // Efficiency
                m_Efficieny_trueHits->GetBinContent(i), //Total True Hits

                BinNomialSigma(m_Efficieny_trueHits->GetBinContent(i),
                m_Efficieny_map->GetBinContent(i))

                );
  }
}

void s_process_collection_standard::extract_residual() {
  DrawResidual(-3, 3);


  TF1 f("f1", "gaus");

  m_Residual->Fit(&f, "Q");
  {
    auto residual_sigma = f.GetParameter("Sigma");
    xml_print("residual_sigma", residual_sigma);
    m_outputl.set_residual(residual_sigma);
  }
  {
    auto residual_mean = f.GetParameter("Mean");
    xml_print("residual_mean", residual_mean);
    m_outputl.set_offset(residual_mean);
  }
}

void s_process_collection_standard::extract_rotation() {
  DrawResidualVsMissingCordinate(-10, 10);
  auto h = getResidualVsMissingCordinate();
  auto f1 = SCT_helpers::LinearFit_Of_Profile(h, 0.01);
  auto rot = TMath::ATan(f1.GetParameter("p1"));
  m_outputl.set_rotation(rot);
  xml_print("rotation", rot);
}

void s_process_collection_standard::process_reset() {
  m_plotCollection.reset();
  m_res_VS_event.clear();
  m_outputl.reset();
}
void s_process_collection_standard::process_set_run_prob(const FileProberties& fileP) {
  xml_print("fileName", fileP.getTfile()->GetName());


  xml_print("m_runNumber", fileP.m_runNumber);
  m_outputl.set_RunNumber(fileP.m_runNumber);


  xml_print("threshold", fileP.m_Threshold);
  m_outputl.set_Threshold(fileP.m_Threshold);

  xml_print("HV", fileP.m_HV);
  m_outputl.set_HV(fileP.m_HV);
}

void s_process_collection_standard::start_collection(TFile* file__) {
  m_outputTree = std::make_shared<sct_corr::treeCollection_ouput>(
    m_outputl,
    &m_buffer,
    true
    );

  m_outputTree->getTTree()->SetDirectory(file__->GetDirectory("/"));
}

bool s_process_collection_standard::process_file(FileProberties* fileP) {
  process_reset();
  auto file_PRINTOUT = xml_print("file");

  process_set_run_prob(*fileP);

  
  m_file_fitter.reset();
  
  


  m_plotCollection = std::make_shared<S_plot_collection>(fileP->getTfile());
  m_plotCollection->setOutputFile(m_dummy);
  m_file_fitter = std::make_shared<s_file_fitter>(m_plotCollection, get_gear());

  m_output_planes = m_file_fitter->get_correlations_channel(
    get_xml_input()->globalConfig().cut(),
    residualCut_t(get_xml_input()->globalConfig().residual_cut()),
    rot_angle_t(get_xml_input()->globalConfig().Rotation()),
    move_t(get_xml_input()->globalConfig().Position_value()),
    s_plot_prob("Hits_in_channels")
    .SaveToDisk()
    );

 

  auto res = sct_processor::residual(
    m_file_fitter->DUT_fitted_local_GBL().getX_def(),
    m_file_fitter->DUT_hit_local().getX_def(),
    s_plot_prob("residualVSEvent").SaveToDisk()
    );

  m_res_VS_event.push_back(res);

#ifdef _DEBUG
  m_plotCollection->loop(40000);
#else
  m_plotCollection->loop();
#endif // _DEBUG

  

  Draw_Efficinecy_map();

  extract_hitMap();
  extract_efficiency();
  extract_residual();
  extract_rotation();

  m_outputTree->fill();
  return true;
}

const xmlImputFiles::XML_imput_file* s_process_collection::get_xml_input() const {
  return m_input_files_xml.get();
}

const sct_corr::Xgear* s_process_collection::get_gear() const {
  return m_gear.get();
}

bool s_process_collection::process() {

  TCanvas c;

  auto files = xml_print("files");

  TFile* _file1 = new TFile(
    m_outname.c_str(), 
    "recreate"
    );
  start_collection(_file1);


 
  for (auto &e : m_files) {
     process_file(&e);

  }
  _file1->Write();
  return true;
}




s_process_collection_standard::s_process_collection_standard() :m_outputl("out") {
  m_dummy = new TFile("dummy1.root", "recreate");
}

s_process_collection_standard::~s_process_collection_standard() {
  delete m_dummy;
}

Long64_t s_process_collection_standard::DrawResidual(Double_t min_X, Double_t max_X) {
  m_Residual = std::make_shared<TH1D>(
    "residual", 
    "residual", 
    100, 
    min_X, 
    max_X
    );


  return m_plotCollection->Draw(
    m_output_planes.getResidual(), 
    S_DrawOption()
    .draw_x()
    .cut_x(min_X, max_X)
    .output_object(m_Residual.get())
    );
}



Long64_t s_process_collection_standard::DrawResidual() {

  auto ret = m_plotCollection->Draw(
    m_output_planes.getResidual(), 
    S_DrawOption().draw_x()
    );
  
  TH1* h = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
  h->SetTitle("residual");
  return ret;
}

Long64_t s_process_collection_standard::DrawResidualVsEvent(Double_t min_X, Double_t max_X) {
  m_ResidualVsEvent = std::make_shared<TH2D>(
    "ResidualVsEvent", 
    "Residual Vs Event", 
    100, 0, 0, 
    100, min_X, max_X
    );

  return m_plotCollection->Draw(
    m_res_VS_event(), 
    S_DrawOption()
    .draw_x_VS_y()
    .cut_x(min_X, max_X)
    .output_object(m_resVSMissing.get())
    .opt_colz()
    );
}

Long64_t s_process_collection_standard::DrawResidualVsEvent() {

  auto ret = m_plotCollection->Draw(
    m_res_VS_event(), 
    S_DrawOption().draw_x_VS_y()
    );

  TH2* h = dynamic_cast<TH2*>(gPad->GetPrimitive("htemp"));
  h->SetTitle("Residual Vs Event");

  return ret;

}

Long64_t s_process_collection_standard::DrawResidualVsMissingCordinate(Double_t min_X, Double_t max_X) {
  m_resVSMissing = std::make_shared<TH2D>(
    "ResidualVsMissingCordinate",
    "Residual Vs Missing Coordinate",
    100, 0, 0,
    100, min_X, max_X
    );




   auto ret = m_plotCollection->Draw(
    m_output_planes.getResidualVSmissing(),
    S_DrawOption()
    .draw_x_VS_y()
    .cut_x(min_X, max_X)
    .output_object(m_resVSMissing.get())
    .opt_colz()
    );
  auto f = new TF1(SCT_helpers::LinearFit_Of_Profile(m_resVSMissing.get(), 0));
//   std::cout << f->GetParameter("p1") << std::endl;
//   std::cout << f->GetParameter("p0") << std::endl;
  m_plotCollection->Draw(
    m_output_planes.getResidualVSmissing(),
    S_DrawOption()
    .draw_x_VS_y()
    .cut_x(min_X, max_X)
    .output_object(m_resVSMissing.get())
    .opt_colz()
    );

  f->Draw("same");

  return ret;
}

Long64_t s_process_collection_standard::DrawResidualVsMissingCordinate() {

  auto ret = m_plotCollection->Draw(
    m_output_planes.getResidualVSmissing(),
    S_DrawOption()
    .draw_x_VS_y()
    .opt_colz()
    );


  //TH2* h = dynamic_cast<TH2*>(gPad->GetPrimitive("htemp"));
  //auto f = new TF1(SCT_helpers::LinearFit_Of_Profile(h, 0.2));
  //f->Draw("same");
 // h->SetTitle("Residual Vs Missing Coordinate");

  return ret;
}

Long64_t s_process_collection_standard::Draw_Efficinecy_map() {

  m_Efficieny_trueHits = std::make_shared<TH1D>(
    "total", 
    "total", 
    get_xml_input()->globalConfig().NumberOfBins(), -0.5, get_xml_input()->globalConfig().NumberOfStrips() - 0.5
    );

  m_plotCollection->Draw(
    m_output_planes.getTotalTrueHits(),
    S_DrawOption()
    .draw_x()
    .output_object(m_Efficieny_trueHits.get())
    );

  m_Efficieny_map = std::make_shared<TH1D>(
    "Efficiency", 
    "Efficiency", 
    get_xml_input()->globalConfig().NumberOfBins(), -0.5, get_xml_input()->globalConfig().NumberOfStrips() - 0.5
    );
  
  auto n = m_plotCollection->Draw(
    m_output_planes.getTrueHitsWithDUT(), 
    S_DrawOption()
    .draw_x()
    .output_object(m_Efficieny_map.get())
    );
  auto e = SCT_helpers::calc_efficiency(m_Efficieny_trueHits.get(), m_Efficieny_map.get());
  auto eth2d = dynamic_cast<TH1D*>(e);

  m_Efficieny_map = std::shared_ptr<TH1D>(  eth2d);
 // m_Efficieny_map->Divide(m_Efficieny_trueHits.get());

  m_Efficieny_map->Draw();
  return n;
}

Long64_t s_process_collection_standard::Draw_Hit_map() {
  m_Hits_total = std::make_shared<TH1D>(
    "total", 
    "total",
    get_xml_input()->globalConfig().NumberOfBins(), -0.5, get_xml_input()->globalConfig().NumberOfStrips() - 0.5
    );

  return  m_plotCollection->Draw(
    m_output_planes.getTotalTrueHits(), 
    S_DrawOption()
    .draw_x()
    .output_object(m_Hits_total.get())
    );
}

Long64_t s_process_collection_standard::Draw_DUT_Hits_map() {
  m_Hits_with_DUT_Hits = std::make_shared<TH1D>(
    "DUT", 
    "DUT", 
    get_xml_input()->globalConfig().NumberOfBins(), -0.5, get_xml_input()->globalConfig().NumberOfStrips() - 0.5
    );

  return m_plotCollection->Draw(
    m_output_planes.getTrueHitsWithDUT(), 
    S_DrawOption()
    .draw_x()
    .output_object(m_Hits_with_DUT_Hits.get())
    );
}

TH2D* s_process_collection_standard::getResidualVsMissingCordinate() {
  return m_resVSMissing.get();
}

void s_process_collection_standard::pushChannel(
  Double_t channel_x, 
  Double_t channel_y, 
  Double_t Effi, 
  Double_t NumberOfEvents, 
  Double_t Error_Effi
  ) {

  m_outputl.getData(x_axis_def)->push_back(channel_x);
  m_outputl.getData(y_axis_def)->push_back(channel_y);
  m_outputl.getData(Occupancy_axis_def)->push_back(Effi);
  m_outputl.getData(Occupancy_error_axis_def)->push_back(Error_Effi);//LB
  m_outputl.getData(NumOfEvents_axis_def)->push_back(NumberOfEvents);
  m_outputl.getData(getIDString())->push_back(0);


}

TFile* FileProberties::getTfile() const {
  if (m_fileOwnd) {
    return m_fileOwnd.get();
  }

  if (m_file) {
    return m_file;
  }
  return nullptr;
}

void FileProberties::setTFile(std::shared_ptr<TFile> file) {
  m_fileOwnd = file;
}

void FileProberties::setTFile(TFile* file) {
  m_file = file;
}

s_process_collection_modulo::s_process_collection_modulo() :m_outputl("out") {
  m_dummy = new TFile("dummy1.root", "recreate");
}

s_process_collection_modulo::~s_process_collection_modulo() {

}

void s_process_collection_modulo::start_collection(TFile* file__) {
  m_outputTree = std::make_shared<sct_corr::treeCollection_ouput>(
    m_outputl,
    &m_buffer,
    true
    );

  m_outputTree->getTTree()->SetDirectory(file__->GetDirectory("/"));
}

bool s_process_collection_modulo::process_file(FileProberties* fileP) {

  m_plotCollection = std::make_shared<S_plot_collection>(fileP->getTfile());
  m_plotCollection->setOutputFile(m_dummy);
  m_file_fitter = std::make_shared<s_file_fitter>(m_plotCollection, get_gear());

  auto pl = m_file_fitter->get_collection();


  auto gbl_collection = m_file_fitter->get_correlations_channel(
    S_YCut(-42, -36),
    residualCut_t(1),
    rot_angle_t(0),
    move_t(-6.36702e-001),
    s_plot_prob("GBL").SaveToDisk()
    );


  sct_corr::inStripEfficiency instrip(
    gbl_collection.getTotalTrueHits(),
    gbl_collection.getTrueHitsWithDUT(),
    S_XCut(280, 360),
    x_axis_def,
    sct_type::modulo_t(3),
    s_plot_prob("inStripEffi")
    );


  sct_corr::hit_efficiency eff(
    gbl_collection.getTotalTrueHits(),
    gbl_collection.getTrueHitsWithDUT(),
    s_plot_prob("effi")
    );
  sct_corr::inStripClusterSize cl_instrip(
    gbl_collection.getTrueHitsWithDUT(),
    m_file_fitter->DUT_zs_data(), 10,
    x_axis_def,
    sct_type::modulo_t(3),
    s_plot_prob("cluster_size_instrip").SaveToDisk()
    );

  sct_corr::residual_efficienct res_eff(
    gbl_collection.getTotalTrueHits(),
    m_file_fitter->DUT_zs_data(),
    S_XCut(280, 360),
    400,
    x_axis_def,
    s_plot_prob("Res_efficiency")
    );

  pl->loop(4000);
  //pl->loop();
  

  res_eff.Draw();
//  SCT_helpers::saveTH1_as_txt(*res_eff.getEfficiency_map(), (path_ + name_ + "residual_efficiency" + "." + "txt").c_str());

  
  instrip.Draw(S_DrawOption());
//  SCT_helpers::saveTH1_as_txt(*instrip.getEfficiency_map(), (path_ + name_ + "instripEffi" + "." + "txt").c_str());

  

  eff.Draw();

  

  cl_instrip.Draw();

  return true;
}

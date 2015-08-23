#include "s_process_collection.h"
#include "TFile.h"
#include "sct_plots.h"
#include "s_plane.h"
#include <string>
#include <iostream>
#include "treeCollection.h"
#include "TF1.h"
#include "TCanvas.h"
#include "geometry/setup_description.hh"
#include "xml_helpers/xml_fileList.hh"


bool gDo_print = false;
int gPos = 0;

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
  bool end_printed = false, first_element;
  int m_pos;
};
double BinNomialSigma(double totalHits, double DUTHits) {
  return sqrt((DUTHits / totalHits)*(1 - (DUTHits / totalHits))*(1 / totalHits));
}
s_process_collection::s_process_collection() :m_outputl("out") {


  m_dummy = new TFile("dummy1.root", "recreate");


}


s_process_collection::~s_process_collection() {
  delete m_dummy;

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

void s_process_collection::AddCut(const S_Cut& cut) {
  m_cuts.add_Cut(cut);
}

int s_process_collection::Add_XML_RunList(const std::string& xmlInputFileName, std::string path__, std::string outputPath /*= "."*/, int element /* = -1*/) {
  path__ += "/";
  xmlImputFiles::XML_imput_file xml_imput(xmlInputFileName.c_str());

  if (xml_imput.fileList().empty()) {
    return -1;
  }
  auto collname = xml_imput.globalConfig().CollectionName();
  if (element != -1) {
    outputPath += "/" + collname + "_" + std::to_string(element) + ".root";
  } else {
    outputPath += "/" + collname + "_" + ".root";
  }


  setOutputName(outputPath.c_str());

  SetNumberOfBins(xml_imput.globalConfig().NumberOfBins());
  SetNumberOfStrips(xml_imput.globalConfig().NumberOfStrips());
  AddCut(xml_imput.globalConfig().cut());
  setActiveArea(xml_imput.globalConfig().AvtiveStrips().getMin(), xml_imput.globalConfig().AvtiveStrips().getMax());
  SetRotation(xml_imput.globalConfig().Rotation());
  SetPosition(xml_imput.globalConfig().Position_value(), 0);
  setResidualCut(xml_imput.globalConfig().residual_cut());
  setGearFile(xml_imput.globalConfig().gearFile().c_str());


  if (element != -1) {
    if (element >= (int)xml_imput.fileList().size()) {
      return -1;
    }
    auto& e = xml_imput.fileList()[element];
    push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());

  } else {
    for (auto& e : xml_imput.fileList()) {
      push_files((path__ + std::string(e.name())).c_str(), e.threshold(), e.runNumber(), e.HV());
    }

  }


  return 0;
}

void s_process_collection::SetPitchSize(Double_t pitchSize) {
  m_pitchSize = pitchSize;
}

void s_process_collection::SetRotation(Double_t rotation) {
  m_rotation = rotation;
}

void s_process_collection::SetPosition(Double_t x_pos, Double_t y_pos) {
  m_pos_x = x_pos;
  m_pos_y = y_pos;
}

void s_process_collection::setActiveArea(Double_t x_min, Double_t x_max) {
  m_active_area_x_min = x_min;
  m_active_area_x_max = x_max;
}

void s_process_collection::SetNumberOfBins(Int_t bins) {
  m_bins = bins;
}

void s_process_collection::SetNumberOfStrips(Int_t strips) {
  m_numOfStrips = strips;
}


void s_process_collection::setResidualCut(double residualCut) {
  m_residual_cut = residualCut;
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

void s_process_collection::extract_efficiency() {

  double totalHits = (double)m_plotCollection->Draw(m_output_planes.getTotalTrueHits(),
                                                    S_DrawOption()
                                                    .cut_x(m_active_area_x_min,
                                                    m_active_area_x_max)
                                                    );

  xml_print("TotalNumOfEvents", totalHits);
  m_outputl.set_TotalNumOfEvents(totalHits);

  double DUTHits = (double)m_plotCollection->Draw(m_output_planes.getTrueHitsWithDUT(),
                                                  S_DrawOption()
                                                  .cut_x(m_active_area_x_min,
                                                  m_active_area_x_max)
                                                  );
  xml_print("DUTHits", DUTHits);

  xml_print("TotalNumOfEvents", DUTHits / totalHits);
  m_outputl.set_Total_efficiency(DUTHits / totalHits);


  auto Error_efficiency = BinNomialSigma(totalHits,
                                         DUTHits);

  xml_print("Error_efficiency", Error_efficiency);
  m_outputl.set_Error_efficiency(Error_efficiency);


}

void s_process_collection::extract_hitMap() {

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

void s_process_collection::extract_residual() {
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

void s_process_collection::extract_rotation() {
  DrawResidualVsMissingCordinate(-10, 10);
  auto h = getResidualVsMissingCordinate();
  auto f1 = SCT_helpers::LinearFit_Of_Profile(h, 0.2);
  auto rot = f1.GetParameter("p1");
  m_outputl.set_rotation(rot);
  xml_print("rotation", rot);
}

void s_process_collection::process_reset() {
  m_plotCollection.reset();
  m_res_VS_event.clear();
}

bool s_process_collection::process(FileProberties* fileP) {
  auto file_PRINTOUT = xml_print("file");

  xml_print("fileName", fileP->getTfile()->GetName());
  xml_print("threshold", fileP->m_Threshold);
  xml_print("HV", fileP->m_HV);

  
  process_reset();
  m_plotCollection = std::make_shared<S_plot_collection>(fileP->getTfile());
  m_plotCollection->setOutputFile(m_dummy);

  m_output_planes = sct_plot::Create_Correlations_of_true_Fitted_hits_with_DUT_Hits_in_channels(
    *m_plotCollection,
    m_cuts,
    m_residual_cut,
    *m_gear,
    m_rotation,
    m_pos_x,
    s_plot_prob("Hits_in_channels").SaveToDisk()
    );



  auto res = sct_plot::residual(
    *m_plotCollection,
    sct_coll::DUT_fitted_local_GBL().getX_def(),
    sct_coll::DUT_hit_local().getX_def(),
    s_plot_prob("residualVSEvent")
    );

  m_res_VS_event.push_back(res);
  m_plotCollection->loop(4000);

  Draw_Efficinecy_map();

  extract_hitMap();
  extract_efficiency();
  extract_residual();
  extract_rotation();

  m_outputTree->fill();
  return true;
}

bool s_process_collection::process() {

  TCanvas c;

  auto files = xml_print("files");
  TFile* _file1 = new TFile(m_outname.c_str(), "recreate");

  m_outputTree = std::make_shared<sct_corr::treeCollection_ouput>(m_outputl, &m_buffer, true);
  m_outputTree->getTTree()->SetDirectory(_file1->GetDirectory("/"));
  for (auto &e : m_files) {
    //   std::cout << "processing file:  " << e.getTfile()->GetName() << std::endl;
    m_outputl.reset();
    m_outputl.set_RunNumber(e.m_runNumber);
    m_outputl.set_Threshold(e.m_Threshold);

    m_outputl.set_HV(e.m_HV);
    process(&e);

  }
  _file1->Write();
  return true;
}



Long64_t s_process_collection::DrawResidual(Double_t min_X, Double_t max_X) {
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



Long64_t s_process_collection::DrawResidual() {

  auto ret = m_plotCollection->Draw(
    m_output_planes.getResidual(), 
    S_DrawOption().draw_x()
    );
  
  TH1* h = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
  h->SetTitle("residual");
  return ret;
}

Long64_t s_process_collection::DrawResidualVsEvent(Double_t min_X, Double_t max_X) {
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

Long64_t s_process_collection::DrawResidualVsEvent() {

  auto ret = m_plotCollection->Draw(
    m_res_VS_event(), 
    S_DrawOption().draw_x_VS_y()
    );

  TH2* h = dynamic_cast<TH2*>(gPad->GetPrimitive("htemp"));
  h->SetTitle("Residual Vs Event");

  return ret;

}

Long64_t s_process_collection::DrawResidualVsMissingCordinate(Double_t min_X, Double_t max_X) {
  m_resVSMissing = std::make_shared<TH2D>(
    "ResidualVsMissingCordinate",
    "Residual Vs Missing Coordinate",
    100, 0, 0,
    100, min_X, max_X
    );




  return m_plotCollection->Draw(
    m_output_planes.getResidualVSmissing(),
    S_DrawOption()
    .draw_x_VS_y()
    .cut_x(min_X, max_X)
    .output_object(m_resVSMissing.get())
    .opt_colz()
    );

}

Long64_t s_process_collection::DrawResidualVsMissingCordinate() {

  auto ret = m_plotCollection->Draw(
    m_output_planes.getResidualVSmissing(),
    S_DrawOption()
    .draw_x_VS_y()
    .opt_colz()
    );


  TH2* h = dynamic_cast<TH2*>(gPad->GetPrimitive("htemp"));

  h->SetTitle("Residual Vs Missing Coordinate");

  return ret;
}

Long64_t s_process_collection::Draw_Efficinecy_map() {

  m_Efficieny_trueHits = std::make_shared<TH1D>("total", "total", m_bins, -0.5, m_numOfStrips - 0.5);

  m_plotCollection->Draw(m_output_planes.getTotalTrueHits(), S_DrawOption().draw_x().output_object(m_Efficieny_trueHits.get()));

  m_Efficieny_map = std::make_shared<TH1D>("Efficiency", "Efficiency", m_bins, -0.5, m_numOfStrips - 0.5);
  auto n = m_plotCollection->Draw(m_output_planes.getTrueHitsWithDUT(), S_DrawOption().draw_x().output_object(m_Efficieny_map.get()));

  m_Efficieny_map->Divide(m_Efficieny_trueHits.get());

  m_Efficieny_map->Draw();
  return n;
}

Long64_t s_process_collection::Draw_Hit_map() {
  m_Hits_total = std::make_shared<TH1D>("total", "total", m_bins, -0.5, m_numOfStrips - 0.5);

  return  m_plotCollection->Draw(m_output_planes.getTotalTrueHits(), S_DrawOption().draw_x().output_object(m_Hits_total.get()));
}

Long64_t s_process_collection::Draw_DUT_Hits_map() {
  m_Hits_with_DUT_Hits = std::make_shared<TH1D>("DUT", "DUT", m_bins, -0.5, m_numOfStrips - 0.5);
  return m_plotCollection->Draw(m_output_planes.getTrueHitsWithDUT(), S_DrawOption().draw_x().output_object(m_Hits_with_DUT_Hits.get()));
}

TH2D* s_process_collection::getResidualVsMissingCordinate() {
  return m_resVSMissing.get();
}

void s_process_collection::pushChannel(Double_t channel_x, Double_t channel_y, Double_t Effi, Double_t NumberOfEvents, Double_t Error_Effi) {

  //Fill the vectors Occupancy_axis_def and so on
  m_outputl.getData(x_axis_def)->push_back(channel_x);
  m_outputl.getData(y_axis_def)->push_back(channel_y);
  m_outputl.getData(Occupancy_axis_def)->push_back(Effi);
  m_outputl.getData(Occupancy_error_axis_def)->push_back(Error_Effi);//LB
  m_outputl.getData(NumOfEvents_axis_def)->push_back(NumberOfEvents);
  m_outputl.getData(getIDString())->push_back(0);


}

TFile* s_process_collection::FileProberties::getTfile() const {
  if (m_fileOwnd) {
    return m_fileOwnd.get();
  }

  if (m_file) {
    return m_file;
  }
  return nullptr;
}

void s_process_collection::FileProberties::setTFile(std::shared_ptr<TFile> file) {
  m_fileOwnd = file;
}

void s_process_collection::FileProberties::setTFile(TFile* file) {
  m_file = file;
}

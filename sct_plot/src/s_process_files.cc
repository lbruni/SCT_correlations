#include "s_process_files.h"
#include "TFile.h"
#include "sct_plots.h"
#include "s_plane.h"

#include "treeCollection.h"
#include "TF1.h"
#include "TCanvas.h"

s_process_files::s_process_files() :m_outputl("out")
{
 

 m_dummy = new TFile("dummy1.root", "recreate");


}


s_process_files::~s_process_files()
{
  
}

void s_process_files::setOutputName(const char* name)
{
  m_outname = name;
}

void s_process_files::push_files(TFile* _file, double Threshold, double runNumber)
{
  FileProberties p;
  p.m_file = _file;
  p.m_Threshold = Threshold;
  p.m_runNumber = runNumber;
  m_files.push_back(p);
}

void s_process_files::push_files(const char* _fileName, double Threshold, double runNumber, double HV)
{
  FileProberties p;
  p.m_file = new TFile(_fileName);
  p.m_Threshold = Threshold;
  p.m_runNumber = runNumber;
  p.m_HV = HV;
  m_files.push_back(p);

}

void s_process_files::AddCut(const S_Cut& cut)
{
  m_cuts.add_Cut(cut);
}

void s_process_files::SetPitchSize(Double_t pitchSize)
{
  m_pitchSize = pitchSize;
}

void s_process_files::SetRotation(Double_t rotation)
{
  m_rotation = rotation;
}

void s_process_files::SetPosition(Double_t x_pos, Double_t y_pos)
{
  m_pos_x = x_pos;
  m_pos_y = y_pos;
}

void s_process_files::setActiveArea(Double_t x_min, Double_t x_max)
{
  m_active_area_x_min = x_min;
  m_active_area_x_max = x_max;
}

void s_process_files::SetNumberOfBins(Int_t bins)
{
  m_bins = bins;
}

void s_process_files::extract_hitMap()
{
  for (Int_t i = 0; i < m_Efficieny_map->GetNbinsX(); ++i)
  {
    pushChannel(m_Efficieny_map->GetBinCenter(i), 1,
      m_Efficieny_map->GetBinContent(i),
      m_Hits_total->GetBinContent(i)
      );
  }
}

bool s_process_files::process(TFile* file)
{

    m_plotCollection.reset();
    m_plotCollection = std::make_shared<S_plot_collection>(file);
    m_plotCollection->setOutputFile(m_dummy);

    m_output_planes = s_plane_collection();
    auto truehits = sct_plot::Crate_True_Fitted_DUT_Hits_in_channels(*m_plotCollection, m_pitchSize, m_rotation, m_pos_x, m_pos_y, s_plot_prob().doNotSaveToDisk());

  auto trueHits_cut = m_plotCollection->addPlot(sct_plot::cut_x_y(m_cuts), truehits);
  auto cor = m_plotCollection->addPlot(sct_plot::find_nearest_strip(x_axis_def, 100, s_plot_prob()), sct_coll::DUT_zs_data(), trueHits_cut());
  s_plane_collection hitmap__rot = m_plotCollection->addPlot(sct_plot::hitmap(), cor.get("nearest_strip_distance")().getX_def(), cor.get("nearest_strip_plane2")().getY_def());
 
  m_output_planes = hitmap__rot + cor + trueHits_cut;
  m_plotCollection->loop();

  DrawResidualVsMissingCordinate(-3,3);

  Draw_Efficinecy_map();

  extract_hitMap();

  
  double totalHits = m_plotCollection->Draw(trueHits_cut(), S_DrawOption().cut_x(m_active_area_x_min, m_active_area_x_max));
  m_outputl.set_TotalNumOfEvents(totalHits);
  //std::cout << pl->Draw(cor.get("nearest_strip_plane2"), S_DrawOption().cut_x(280, 360)) << std::endl;
  double DUTHits = m_plotCollection->Draw(cor.get("nearest_strip_plane2"), S_DrawOption().cut_x(m_active_area_x_min, m_active_area_x_max));


  m_outputl.set_Total_efficiency(DUTHits/ totalHits);
  DrawResidual(-3, 3);
  

  TF1 f("f1","gaus");

  m_Residual->Fit(&f);

  m_outputl.set_residual( f.GetParameter("Sigma"));
  m_outputl.set_offset(f.GetParameter("Mean"));
  m_outputTree->fill();
  return true;
}

bool s_process_files::process()
{
  TCanvas c;
  
  TFile* _file1 = new TFile(m_outname.c_str(), "recreate");

  m_outputTree = std::make_shared<sct_corr::treeCollection_ouput>(m_outputl, &m_buffer, true);
  m_outputTree->getTTree()->SetDirectory(_file1->GetDirectory("/"));
  for (auto &e : m_files)
  {
    m_outputl.reset();
    m_outputl.set_RunNumber(e.m_runNumber);
    m_outputl.set_Threshold(e.m_Threshold);
    m_outputl.set_HV(e.m_HV);
    process(e.m_file);

  }
  _file1->Write();
  return true;
}



Int_t s_process_files::DrawResidual(Double_t min_X, Double_t max_X)
{
  m_Residual = std::make_shared<TH1D>("residual", "residual", 100, min_X, max_X);
 return m_plotCollection->Draw(m_output_planes.get("nearest_strip_distance"), S_DrawOption().draw_x().cut_x(min_X,max_X).output_object(m_Residual.get()));
}



Int_t s_process_files::DrawResidualVsMissingCordinate(Double_t min_X, Double_t max_X)
{
  m_resVSMissing = std::make_shared<TH2D>("h2", "adsad", 100, 0, 0, 100, min_X, max_X);
  return m_plotCollection->Draw(m_output_planes.get("hitmap"), S_DrawOption().draw_x_VS_y().cut_x(min_X, max_X).output_object(m_resVSMissing.get()).opt_colz());
  
}

Int_t s_process_files::Draw_Efficinecy_map()
{

   Draw_Hit_map();
   Int_t n = Draw_DUT_Hits_map();
  
  m_Efficieny_map = std::shared_ptr<TH1D>((TH1D*)m_Hits_with_DUT_Hits->Clone("Efficiency"));
  m_Efficieny_map->Divide(m_Hits_total.get());

  m_Efficieny_map->Draw();
  return n;
}

Int_t s_process_files::Draw_Hit_map()
{
  m_Hits_total = std::make_shared<TH1D>("total", "total", m_bins, 0 - 0.5, m_bins - 0.5);

  return  m_plotCollection->Draw(m_output_planes.get("cut_x_y"), S_DrawOption().draw_x().output_object(m_Hits_total.get()));
}

Int_t s_process_files::Draw_DUT_Hits_map()
{
  m_Hits_with_DUT_Hits = std::make_shared<TH1D>("DUT", "DUT", m_bins, 0 - 0.5, m_bins - 0.5);
  return m_plotCollection->Draw(m_output_planes.get("nearest_strip_plane2"), S_DrawOption().draw_x().output_object(m_Hits_with_DUT_Hits.get()));
}

void s_process_files::pushChannel(Double_t channel_x, Double_t channel_y, Double_t Effi, Double_t NumberOfEvents)
{
  m_outputl.getData(x_axis_def)->push_back(channel_x);
  m_outputl.getData(y_axis_def)->push_back(channel_y);
  m_outputl.getData(Occupancy_axis_def)->push_back(Effi);
  m_outputl.getData(NumOfEvents_axis_def)->push_back(NumberOfEvents);
  m_outputl.getData(getIDString())->push_back(0);
}


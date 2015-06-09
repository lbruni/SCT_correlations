#include "internal/PLaneVsPlane_plots.hh"
#include <iostream>
#include "TMath.h"
#include "TH2.h"
#include "TCanvas.h"
class efficieny_map :public plotPlaneVsPlane{
public:
  efficieny_map(const  S_plot_def& plot_def);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) override;
  virtual s_plane_collection getOutputcollection() override;
  virtual Long64_t Draw(const char* options, const char* cuts /* = "" */, const char* axis /* = "y:x" */) override;

  static const char* x_bin();
  static const char* y_bin();
  static const char* x_cut_name();
  static const char* y_cut_name();
  Double_t m_xbin_size=1,m_ybin_size=1;
  Double_t m_xCut = 1, m_yCut= 1;
  Double_t m_xMin = 10000 ,m_xMax=-10000;
  Double_t m_yMin = 10000, m_yMax = -10000;

  plane_hit make_bin_hit(const plane_hit& h) const;
  void set_min_max(const plane_hit& h);
};

efficieny_map::efficieny_map(const S_plot_def& plot_def) :plotPlaneVsPlane(plot_def)
{
  try{

    m_xbin_size = atof(m_plot_def.getParameter(x_bin(), ""));
    m_ybin_size = atof(m_plot_def.getParameter(y_bin(), ""));
    m_xCut = atof(m_plot_def.getParameter(x_cut_name(), ""));
    m_yCut= atof(m_plot_def.getParameter(y_cut_name(), ""));
  }
  catch (...){
    std::cout << "[efficieny_map]  unable to convert parameter" << std::endl;

  }

}



void efficieny_map::processHit(const plane_hit& p1, const plane_hit& p2)
{
  auto binp1 = make_bin_hit(p1);
  auto binp2 = make_bin_hit(p2);
  auto x = binp2.x;
  auto y = binp2.y;

  if (TMath::Abs(p1.x - p2.x) <m_xCut&& TMath::Abs(p1.y - p2.y)<m_yCut)
  {

    pushHit(binp1.x, binp1.y, 0);
    pushHit(binp2.x, binp2.y, 1);

    set_min_max(binp1);
    set_min_max(binp2);
  }


  pushHit(binp1.x, binp1.y, 2);
  pushHit(binp2.x, binp2.y, 3);
}

s_plane_collection efficieny_map::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("A_and_B_with_A_binning"), S_plane_def(getOutputName(), 0)));
  ret.m_planes.push_back(std::make_pair(std::string("A_and_B_with_B_binning"), S_plane_def(getOutputName(), 1)));
  ret.m_planes.push_back(std::make_pair(std::string("A_binned"), S_plane_def(getOutputName(), 2)));
  ret.m_planes.push_back(std::make_pair(std::string("B_binned"), S_plane_def(getOutputName(), 3)));
  ret.m_planes.push_back(std::make_pair(std::string("A_and_B_with_A_binning_normalised"), S_plane_def(getOutputName(), 4)));
  ret.m_planes.push_back(std::make_pair(std::string("A_and_B_with_B_binning_normalised"), S_plane_def(getOutputName(), 5)));
  return ret;
}

Long64_t efficieny_map::Draw(const char* options, const char* cuts /* = "" */, const char* axis /* = "y:x" */)
{
  
  auto x_bins = (m_xMax - m_xMin) / m_xbin_size;
  auto y_bins = (m_yMax - m_yMin) / m_ybin_size;
  TH2D* h1 = new TH2D("A_and_B", "A_and_B", x_bins, m_xMin, m_xMax, y_bins, m_yMin, m_yMax);
 // new TCanvas();
auto elements=  plotPlaneVsPlane::Draw(options, "ID==1", "y:x>>A_and_B");


    TH2D* h2 = new TH2D("B", "B", x_bins, m_xMin, m_xMax, y_bins, m_yMin, m_yMax);
//     new TCanvas();
   plotPlaneVsPlane::Draw(options, "ID==3", "y:x>>B");

   h1->Divide(h2);
//   new TCanvas();
   h1->Draw(options);
  return elements;
}

const char* efficieny_map::x_bin()
{
  return "x_bin____";
}

const char* efficieny_map::y_bin()
{
  return "y_bin____";
}

const char* efficieny_map::x_cut_name()
{
  return "x_cut_name___";
}

const char* efficieny_map::y_cut_name()
{
  return "y_cut_name___";
}

plane_hit efficieny_map::make_bin_hit(const plane_hit& h) const
{
  return plane_hit(static_cast<int>((h.x + 10000) / m_xbin_size)*m_xbin_size - 10000, static_cast<int>((h.y + 10000) / m_ybin_size)*m_ybin_size - 10000);
}

void efficieny_map::set_min_max(const plane_hit& h)
{
  m_xMin = TMath::Min(m_xMin, h.x);
  m_xMax= TMath::Max(m_xMax, h.x);

  m_yMin = TMath::Min(m_yMin, h.y);
  m_yMax = TMath::Max(m_yMax, h.y);


}

registerPlot(efficieny_map, sct::plot_efficieny_map());

S_plot_def sct_plot::s_efficiency_map(const char* name, Double_t x_bin_, Double_t y_bin_,Double_t x_cut,Double_t y_cut, bool save2disk /*= true*/)
{
  S_plot_def ret(sct::plot_efficieny_map(), name, save2disk);
  ret.setParameter(efficieny_map::x_bin(), std::to_string(x_bin_));
  ret.setParameter(efficieny_map::y_bin(), std::to_string(y_bin_));
  

  ret.setParameter(efficieny_map::x_cut_name(), std::to_string(x_cut));
  ret.setParameter(efficieny_map::y_cut_name(), std::to_string(y_cut));


  return ret;
}

S_plot_def sct_plot::s_efficiency_map(const char* name, Double_t x_bin_, Double_t y_bin_, bool save2disk /*= true*/)
{

  return s_efficiency_map(name, x_bin_, y_bin_, x_bin_, y_bin_, save2disk);
}

const char* sct::plot_efficieny_map()
{
  return "efficieny_map";
}

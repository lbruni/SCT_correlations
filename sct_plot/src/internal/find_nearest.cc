#include "internal/PLaneVsPlane_plots.hh"
#include <iostream>
#include "TMath.h"



class find_nearest :public plotPlaneVsPlane{
public:

  static const char* x_cutOff_name();
  static const char* y_cutOff_name();
  find_nearest(const  S_plot_def& plot_def);
  virtual void processEventStart();
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);

  virtual void processEventEnd();
  const double m_c_noHit = 100000000000;
  double m_x_cutOff, m_y_cutOff;
  std::vector<plane_hit> m_dist, m_hit1, m_hit2;

  virtual s_plane_collection getOutputcollection();
};

const char* find_nearest::x_cutOff_name()
{
  return "x_cutOff___";
}

const char* find_nearest::y_cutOff_name()
{
  return "y_cutOff___";
}

find_nearest::find_nearest(const S_plot_def& plot_def) : plotPlaneVsPlane(plot_def)
{
  try{

    m_x_cutOff = atof(m_plot_def.getParameter(x_cutOff_name(), "100000"));
    m_y_cutOff = atof(m_plot_def.getParameter(y_cutOff_name(), "100000"));
  }
  catch (...){
    std::cout << "[find_nearest]  unable to convert parameter" << std::endl;

  }
}

void find_nearest::processEventStart()
{
  m_hit1.clear();
  m_hit2.clear();
  m_dist.clear();
}

void find_nearest::processHit(const plane_hit& p1, const plane_hit& p2)
{
  m_dist.emplace_back((p1.x - p2.x), (p1.y - p2.y));
  m_hit1.emplace_back(p1.x, p1.y);
  m_hit2.emplace_back(p2.x, p2.y);
}

void find_nearest::processEventEnd()
{
  double r = m_c_noHit;
  double x_diff = m_x_cutOff;


  plane_hit dist(0, 0), h1(0, 0), h2(0, 0);

  for (size_t i = 0; i < m_dist.size(); ++i)
  {
    auto e = m_dist.at(i);
    auto r1 = TMath::Sqrt((e.x)*(e.x) + (e.y)*(e.y));

    if (r1 > 0
      &&
      r1 < r
      &&
      TMath::Abs(e.x) < m_x_cutOff
      &&
      TMath::Abs(e.y) < m_y_cutOff)
    {
      r = r1;
      dist = e;
      h1 = m_hit1.at(i);
      h2 = m_hit2.at(i);
    }

  }

  if (r < m_c_noHit)
  {
    pushHit(dist.x, dist.y, 0);
    pushHit(h1.x, h1.y, 1);
    pushHit(h2.x, h1.y, 2);
  }
}

s_plane_collection find_nearest::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("nearest_distance"), S_plane(getOutputName(), 0)));
  ret.m_planes.push_back(std::make_pair(std::string("nearest_plane1"), S_plane(getOutputName(), 1)));
  ret.m_planes.push_back(std::make_pair(std::string("nearest_plane2"), S_plane(getOutputName(), 2)));
  return ret;
}

registerPlot(find_nearest, sct::plot_find_nearest());


S_plot_def sct_plot::s_find_nearest(const char* name, Double_t x_cutoff, Double_t y_cutoff, bool save2disk /*=true*/)
{
  auto ret = S_plot_def(sct::plot_find_nearest(), name, save2disk);

  ret.setParameter(find_nearest::x_cutOff_name(), std::to_string(x_cutoff));
  ret.setParameter(find_nearest::y_cutOff_name(), std::to_string(y_cutoff));
  return ret;
}

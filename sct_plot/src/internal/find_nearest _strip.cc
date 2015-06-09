#include "internal/PLaneVsPlane_plots.hh"
#include <iostream>
#include "TMath.h"




class find_nearest_strip :public plotPlaneVsPlane{
public:
  find_nearest_strip(const  S_plot_def& plot_def);
  static  const char* axis_name();
  static  const char* cutOff_name();
  virtual void processEventStart();
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);

  virtual void processEventEnd();
  double m_cutOff;
  plane_hit dist = plane_hit(0, 0), h1 = plane_hit(0, 0), h2 = plane_hit(0, 0);
  double r = 10000000;
  axis_def m_axis;
  virtual s_plane_collection getOutputcollection();

};

registerPlot(find_nearest_strip, sct::plot_find_nearest_strip());

find_nearest_strip::find_nearest_strip(const S_plot_def& plot_def) : plotPlaneVsPlane(plot_def)
{
  try {
    auto ax = atoi(m_plot_def.getParameter(axis_name(), "0"));
    if (ax == x_axis_def)
    {
      m_axis = x_axis_def;
    }
    else if (ax == y_axis_def){
      m_axis = y_axis_def;

    }
    else
    {
      std::cout << "[find_nearest_strip] unable to convert to axis def" << std::endl;
    }


    m_cutOff = atof(m_plot_def.getParameter(cutOff_name(), "1000000000"));
  }
  catch (...){
    std::cout << "[find_nearest_strip] unable to convert" << std::endl;
  }
}

const char* find_nearest_strip::axis_name()
{
  return "axis____";
}

const char* find_nearest_strip::cutOff_name()
{
  return "cutOff____";
}

void find_nearest_strip::processEventStart()
{
  
  dist = plane_hit(0, 0);
  h1 = plane_hit(0, 0);
  h2 = plane_hit(0, 0);
  r = m_cutOff;
}

void find_nearest_strip::processHit(const plane_hit& p1, const plane_hit& p2)
{
  plane_hit e(p1.x - p2.x, p1.y - p2.y);
  double r1 = 0;
  if (m_axis == x_axis_def)
  {
    r1 = TMath::Abs(e.x);
  }
  else if (m_axis == y_axis_def) {
    r1 = TMath::Abs(e.y);
  }
  else{
    std::cout << "unknown axis" << std::endl;
  }
  if (r1 > 0 && r1 < r)
  {
    r = r1;
    dist = e;
    h1 = p1;
    h2 = p2;
  }
  if (r1==0)
  {
    std::cout << "[find_nearest_strip] error r1== 0" << std::endl;
  }
}

void find_nearest_strip::processEventEnd()
{
  if (r < m_cutOff)
  {

    pushHit(dist.x, dist.y, 0);
    pushHit(h1.x, h1.y, 1);
    pushHit(h2.x, h2.y, 2);
  }
}

s_plane_collection find_nearest_strip::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_distance"), S_plane_def(getOutputName(), 0)));
  ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_plane1"), S_plane_def(getOutputName(), 1)));
  ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_plane2"), S_plane_def(getOutputName(), 2)));
  return ret;
}


S_plot_def sct_plot::s_find_nearest_strip(const char* name, axis_def search_axis, Double_t cutOfff /*=100000*/, bool save2disk /*= true*/)
{
  auto ret = S_plot_def(sct::plot_find_nearest_strip(), name, save2disk);
  ret.setParameter(find_nearest_strip::cutOff_name(), std::to_string(cutOfff));
  ret.setParameter(find_nearest_strip::axis_name(), std::to_string(search_axis));
  return ret;
}

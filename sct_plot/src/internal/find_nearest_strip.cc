#include "internal/PLaneVsPlane_plots.hh"
#include <iostream>
#include "TMath.h"



namespace sct_corr{
  class find_nearest_strip :public plotPlaneVsPlane{
  public:
    find_nearest_strip(axis_def search_axis, Double_t cutOfff, const s_plot_prob& = "");
    virtual void processEventStart();
    virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);

    virtual void processEventEnd();
    double m_cutOff;
    plane_hit dist = plane_hit(0, 0), h1 = plane_hit(0, 0), h2 = plane_hit(0, 0);
    double r = 10000000;
    axis_def m_axis;
    virtual s_plane_collection getOutputcollection();
    virtual const char* getType() const override;
  };




  find_nearest_strip::find_nearest_strip(axis_def search_axis, Double_t cutOfff, const s_plot_prob& plot_prob) :plotPlaneVsPlane(plot_prob), m_axis(search_axis), m_cutOff(cutOfff)
  {

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
    if (r1 == 0)
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


  const char* find_nearest_strip::getType() const
  {
    return sct::plot_find_nearest_strip();
  }
}
S_plot sct_plot::find_nearest_strip(axis_def search_axis, Double_t cutOfff /*=100000*/, const s_plot_prob& plot_prob)
{
  return S_plot(new sct_corr::find_nearest_strip( search_axis, cutOfff,plot_prob));
}

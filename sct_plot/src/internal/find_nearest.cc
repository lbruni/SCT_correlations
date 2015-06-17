#include "internal/PLaneVsPlane_plots.hh"
#include <iostream>
#include "TMath.h"

namespace sct_corr{
  class find_nearest :public plotPlaneVsPlane{
  public:


    find_nearest(const char* name, bool save2disk, double x_cutOff, double y_cutOff);
    virtual void processEventStart();
    virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);

    virtual void processEventEnd();
    const double m_c_noHit = 100000000000;
    double m_x_cutOff, m_y_cutOff;
    double r;
    plane_hit  dist = plane_hit(0, 0), h1 = plane_hit(0, 0), h2 = plane_hit(0, 0);
    virtual s_plane_collection getOutputcollection();

    virtual const char* getType() const override;
  };

#define nearest_distance_pos 0
#define nearest_plane1_pos 1
#define nearest_plane2_pos 2

  find_nearest::find_nearest(const char* name, bool save2disk, double x_cutOff, double y_cutOff) :plotPlaneVsPlane(name, save2disk), m_x_cutOff(x_cutOff), m_y_cutOff(y_cutOff)
  {

  }

  void find_nearest::processEventStart()
  {
    dist = plane_hit(0, 0);
    h1 = plane_hit(0, 0);
    h2 = plane_hit(0, 0);
    r = m_c_noHit;
  }

  void find_nearest::processHit(const plane_hit& p1, const plane_hit& p2)
  {


    auto e = plane_hit((p1.x - p2.x), (p1.y - p2.y));
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
      h1 = p1;
      h2 = p2;
    }
    if (r1 == 0)
    {
      std::cout << "[find_nearest_strip] error r1== 0" << std::endl;
    }
  }

  void find_nearest::processEventEnd()
  {
    if (r < m_c_noHit)
    {
      pushHit(dist.x, dist.y, nearest_distance_pos);
      pushHit(h1.x, h1.y, nearest_plane1_pos);
      pushHit(h2.x, h1.y, nearest_plane2_pos);
    }
    else{
      auto i = "noHIt";
    }
  }

  s_plane_collection find_nearest::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("nearest_distance"), S_plane_def(getOutputName(), nearest_distance_pos)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_plane1"), S_plane_def(getOutputName(), nearest_plane1_pos)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_plane2"), S_plane_def(getOutputName(), nearest_plane2_pos)));
    return ret;
  }




  const char* find_nearest::getType() const
  {
    return sct::plot_find_nearest();
  }
}
S_plot sct_plot::find_nearest(const char* name, Double_t x_cutoff, Double_t y_cutoff, plot_save_option_def save_option  /*=  save_to_disk*/)
{
      return S_plot(new sct_corr::find_nearest(name, save_option==save_to_disk, x_cutoff, y_cutoff));
}

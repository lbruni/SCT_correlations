
#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"
#include "internal/plane_hit_healpers.hh"
#include <iostream>

namespace sct_corr{
  class rotated_plane : public plot_hit2d {
  public:
    rotated_plane(double angle,const s_plot_prob& = "");
    virtual void processHit(double x, double y) override;
    double m_angele = 0;

    virtual s_plane_collection getOutputcollection();
    virtual const char* getType() const override;
  };


  rotated_plane::rotated_plane(double angle_, const s_plot_prob& plot_prob) :plot_hit2d(plot_prob), m_angele(angle_)
  {

  }

  void rotated_plane::processHit(double x, double y)
  {
    auto h = rotate(plane_hit(x, y), m_angele);
    pushHit(h.x, h.y);
  }

  s_plane_collection rotated_plane::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("rotated"), S_plane_def(getOutputName(), 0)));
    return ret;
  }

  const char* rotated_plane::getType() const
  {
    return sct::plot_rotated();
  }
}
S_plot sct_plot::rotated(Double_t angle, const s_plot_prob& plot_prob)
{

  return S_plot(new sct_corr::rotated_plane(angle,plot_prob));
}

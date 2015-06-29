#include "internal/PLaneVsPlane_plots.hh"
#include "s_plane.h"
namespace sct_corr{
  class plot_Event_size :public plotPlaneVsPlane{
  public:
    plot_Event_size(const s_plot_prob& = "");
    virtual void processEventEnd();
    virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
    virtual const char* getType() const override;
    virtual s_plane_collection getOutputcollection();
  };



  plot_Event_size::plot_Event_size(const s_plot_prob& plot_prob) : plotPlaneVsPlane(plot_prob)
  {

  }

  void plot_Event_size::processEventEnd()
  {
    pushHit(m_size_x, m_size_y);
  }

  void plot_Event_size::processHit(const plane_hit& p1, const plane_hit& p2)
  {

  }

  const char* plot_Event_size::getType() const
  {
    return sct::plot_Event_size();
  }

  s_plane_collection plot_Event_size::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("Event_size"), S_plane_def(getOutputName(), 0)));
    return ret;
  }
}

S_plot sct_plot::Event_size(const s_plot_prob& plot_prob)
{
  return S_plot(new sct_corr::plot_Event_size(plot_prob));
}

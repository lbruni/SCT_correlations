#include "internal/PLaneVsPlane_plots.hh"

class plot_Event_size :public plotPlaneVsPlane{
public:
  plot_Event_size(const  S_plot_def& plot_def);
  virtual void processEventEnd();
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);

  virtual s_plane_collection getOutputcollection();
};



registerPlot(plot_Event_size, sct::plot_Event_size());


plot_Event_size::plot_Event_size(const S_plot_def& plot_def) : plotPlaneVsPlane(plot_def)
{

}

void plot_Event_size::processEventEnd()
{
  pushHit(m_size_x, m_size_y);
}

void plot_Event_size::processHit(const plane_hit& p1, const plane_hit& p2)
{

}

s_plane_collection plot_Event_size::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("Event_size"), S_plane(getOutputName(), 0)));
  return ret;
}

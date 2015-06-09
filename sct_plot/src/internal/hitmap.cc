#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"


class hitmap :public plot_hit2d{
public:
  hitmap(const S_plot_def& plot_def);
  virtual void processHit(double x, double y) override;
  virtual s_plane_collection getOutputcollection();
};
registerPlot(hitmap, sct::plot_hitmap());
hitmap::hitmap(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{

}

void hitmap::processHit(double x, double y)
{
  pushHit(m_x->get(), m_y->get());
}

s_plane_collection hitmap::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("hitmap"), S_plane_def(getOutputName(), 0)));
  return ret;
}

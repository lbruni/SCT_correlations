#include "internal/PLaneVsPlane_plots.hh"


class plot_a_if_b_has_a_hit :public plotPlaneVsPlane{
public:
  plot_a_if_b_has_a_hit(const  S_plot_def& plot_def);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) override;
  virtual s_plane_collection getOutputcollection();
};

registerPlot(plot_a_if_b_has_a_hit, sct::plot_A_if_B());

plot_a_if_b_has_a_hit::plot_a_if_b_has_a_hit(const S_plot_def& plot_def) : plotPlaneVsPlane(plot_def)
{

}

void plot_a_if_b_has_a_hit::processHit(const plane_hit& p1, const plane_hit& p2)
{
  pushHit(p1.x, p1.y, 0);
  pushHit(p2.x, p2.y, 1);
}

s_plane_collection plot_a_if_b_has_a_hit::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_A"), S_plane(getOutputName(), 0)));
  ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_B"), S_plane(getOutputName(), 1)));
  return ret;
}


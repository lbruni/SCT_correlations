#include "internal/PLaneVsPlane_plots.hh"
#include "sct_plots.h"

namespace sct_corr{
  class plot_a_if_b_has_a_hit :public plotPlaneVsPlane{
  public:
    plot_a_if_b_has_a_hit(const char* name, bool save2disk);
    virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) override;
    virtual s_plane_collection getOutputcollection();
    virtual const char* getType() const override;

  };


  plot_a_if_b_has_a_hit::plot_a_if_b_has_a_hit(const char* name, bool save2disk) : plotPlaneVsPlane(name, save2disk)
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
    ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_A"), S_plane_def(getOutputName(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_B"), S_plane_def(getOutputName(), 1)));
    return ret;
  }


  const char* plot_a_if_b_has_a_hit::getType() const
  {
    return sct::plot_A_if_B();
  }

}
  S_plot sct_plot::s_A_if_B(const char* name, bool save2disk)
  {
    return S_plot(new sct_corr::plot_a_if_b_has_a_hit(name, save2disk));
  }
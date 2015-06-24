#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"

namespace sct_corr{
  class hitmap :public plot_hit2d{
  public:
    hitmap(const s_plot_prob& = "");
    virtual void processHit(double x, double y) override;
    virtual s_plane_collection getOutputcollection();
    virtual const char* getType() const override;
  };

  hitmap::hitmap(const s_plot_prob& plot_prob) :plot_hit2d(plot_prob)
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

  const char* hitmap::getType() const
  {
    return sct::plot_hitmap();
  }
}
S_plot sct_plot::hitmap(const s_plot_prob& plot_prob)
{
  return S_plot(new sct_corr::hitmap(plot_prob));
}

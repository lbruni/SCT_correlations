#include "internal/plot_corr2d.hh"

namespace sct_corr{
  class correlations :public plot_corr2d{
  public:
    correlations(const s_plot_prob& = "");



    virtual void processHit(double x, double y) override;
    virtual s_plane_collection getOutputcollection();
    virtual const char* getType() const override;
  };


  correlations::correlations(const s_plot_prob& plot_prob) :plot_corr2d(plot_prob)
  {

  }

  void correlations::processHit(double x, double y)
  {
    pushHit(x, y);
  }

  s_plane_collection correlations::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("correlations"), S_plane_def(getOutputName(), 0)));
    return ret;
  }


  const char* correlations::getType() const
  {
    return sct::plot_correlation();
  }

}
S_plot sct_plot::correlation(const s_plot_prob& plot_prob)
{

  return S_plot(new sct_corr::correlations(plot_prob));
}

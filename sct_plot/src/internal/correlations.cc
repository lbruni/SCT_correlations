#include "internal/plot_corr2d.hh"


class correlations :public plot_corr2d{
public:
  correlations(const S_plot_def& plot_def);



  virtual void processHit(double x, double y) override;
  virtual s_plane_collection getOutputcollection();
};
registerPlot(correlations, sct::plot_correlation());


correlations::correlations(const S_plot_def& plot_def) :plot_corr2d(plot_def)
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

#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"


class ProjectOnPixel :public plot_hit2d{
public:
  ProjectOnPixel(const S_plot_def& plot_def);

  virtual void processHit(double x, double y) override;

  double m_x_pixelsize = 0.074, m_ypixelsize = 100000;
  virtual s_plane_collection getOutputcollection();
};
registerPlot(ProjectOnPixel, sct::plot_projectOnPixel());


ProjectOnPixel::ProjectOnPixel(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{

}

void ProjectOnPixel::processHit(double x, double y)
{
  pushHit((int)(m_x->get() / m_x_pixelsize * 100) % 100, (int)(m_y->get() / m_ypixelsize * 100) % 100);
}

s_plane_collection ProjectOnPixel::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("ProjectOnPixel"), S_plane(getOutputName(), 0)));
  return ret;
}

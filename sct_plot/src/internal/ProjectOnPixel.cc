#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"

namespace sct_corr{
  class ProjectOnPixel :public plot_hit2d{
  public:
    ProjectOnPixel(const char* name, bool save2disk);

    virtual void processHit(double x, double y) override;

    double m_x_pixelsize = 0.074, m_ypixelsize = 100000;
    virtual s_plane_collection getOutputcollection() override;
    virtual const char* getType() const override;
  };


  ProjectOnPixel::ProjectOnPixel(const char* name, bool save2disk) : plot_hit2d(name, save2disk)
  {

  }

  void ProjectOnPixel::processHit(double x, double y)
  {
    pushHit((int)(m_x->get() / m_x_pixelsize * 100) % 100, (int)(m_y->get() / m_ypixelsize * 100) % 100);
  }

  s_plane_collection ProjectOnPixel::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("ProjectOnPixel"), S_plane_def(getOutputName(), 0)));
    return ret;
  }

  const char* ProjectOnPixel::getType() const
  {
    return sct::plot_projectOnPixel();
  }
}
S_plot sct_plot::s_projectOnPixel(const char* name, bool save2disk)
{
  return S_plot(new sct_corr::ProjectOnPixel(name, save2disk));
}
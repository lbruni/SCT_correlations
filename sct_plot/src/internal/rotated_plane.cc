
#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"
#include "internal/plane_hit_healpers.hh"
#include <iostream>

class rotated_plane : public plot_hit2d {
public:
  static const char* Angle_name();
  rotated_plane(const S_plot_def& plot_def);
  virtual void processHit(double x, double y) override;
  double angele = 0;

  virtual s_plane_collection getOutputcollection();
};
registerPlot(rotated_plane, sct::plot_rotated());


const char* rotated_plane::Angle_name()
{
  return "ANGLE___";
}

rotated_plane::rotated_plane(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{
  std::string a;
  try {

    a = m_plot_def.getParameter(std::string(Angle_name()), std::string("0"));

    angele = atof(a.c_str());
  }
  catch (...){
    std::cout << "unable to convert \"" << a << "\"" << std::endl;
  }
}

void rotated_plane::processHit(double x, double y)
{
  auto h = rotate(plane_hit(x, y), angele);
  pushHit(h.x, h.y);
}

s_plane_collection rotated_plane::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("rotated"), S_plane(getOutputName(), 0)));
  return ret;
}

S_plot_def sct_plot::s_rotated(const char* name, Double_t angle, bool save2disk)
{
  auto ret = S_plot_def(sct::plot_rotated(), name, save2disk);
  ret.setParameter(rotated_plane::Angle_name(), std::to_string(angle));

  return ret;
}

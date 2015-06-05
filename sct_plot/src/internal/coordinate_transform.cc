#include "internal/plot_hit2d.hh"
#include <iostream>

class coordinate_transform : public plot_hit2d {
public:
  static const char* x_slope_name();
  static const char* y_slope_name();
  static const char* x_offset_name();
  static const char* y_offset_name();
  coordinate_transform(const S_plot_def& plot_def);
  virtual void processHit(double x, double y) override;
  double m_x_slope = 0;
  double m_y_slope = 0;
  double m_x_offset = 0;
  double m_y_offset = 0;

  virtual s_plane_collection getOutputcollection();
};

registerPlot(coordinate_transform, sct::plot_coordinate_transform() );

const char* coordinate_transform::x_slope_name()
{
  return "x_slope___";
}

const char* coordinate_transform::y_slope_name()
{
  return "y_slope___";
}

const char* coordinate_transform::x_offset_name()
{
  return "x_offset_";
}

const char* coordinate_transform::y_offset_name()
{
  return "y_offset_";
}

coordinate_transform::coordinate_transform(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{
  try {

    m_x_offset = atof(m_plot_def.getParameter(x_offset_name(), "0"));
    m_y_offset = atof(m_plot_def.getParameter(y_offset_name(), "0"));
    m_x_slope = atof(m_plot_def.getParameter(x_slope_name(), "1"));
    m_y_slope = atof(m_plot_def.getParameter(y_slope_name(), "1"));
  }
  catch (...){
    std::cout << "[coordinate_transform] unable to convert" << std::endl;
  }
}

void coordinate_transform::processHit(double x, double y)
{
  pushHit(x*m_x_slope + m_x_offset, y*m_y_slope + m_y_offset);
}

s_plane_collection coordinate_transform::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("coordinate_transform"), S_plane(getOutputName(), 0)));
  return ret;
}

S_plot_def sct_plot::s_coordinate_transform(const char* name, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset, bool save2disk /*= true*/)
{
  auto ret = S_plot_def(sct::plot_coordinate_transform(), name, save2disk);
  ret.setParameter(coordinate_transform::x_offset_name(), std::to_string(x_offset));
  ret.setParameter(coordinate_transform::y_offset_name(), std::to_string(y_offset));
  ret.setParameter(coordinate_transform::x_slope_name(), std::to_string(x_slope));
  ret.setParameter(coordinate_transform::y_slope_name(), std::to_string(y_slope));
  return ret;
}

S_plot_def sct_plot::s_coordinate_transform_move(const char* name, Double_t x_offset, Double_t y_offset, bool save2disk /*= true*/)
{
  return   sct_plot::s_coordinate_transform(name, 1, x_offset, 1, y_offset, save2disk);
}
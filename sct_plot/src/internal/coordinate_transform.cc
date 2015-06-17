#include "internal/plot_hit2d.hh"
#include <iostream>

namespace sct_corr{
  class coordinate_transform : public plot_hit2d {
  public:
    coordinate_transform(const char* name, bool save2disk, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset);
    virtual void processHit(double x, double y) override;
    double m_x_slope = 0;
    double m_y_slope = 0;
    double m_x_offset = 0;
    double m_y_offset = 0;

    virtual s_plane_collection getOutputcollection();

    virtual const char* getType() const override;
  };





  coordinate_transform::coordinate_transform(const char* name, bool save2disk, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset)
    :plot_hit2d(name, save2disk),
    m_x_slope(x_slope),
    m_y_slope(y_slope),
    m_x_offset(x_offset),
    m_y_offset(y_offset)

  {

  }

  void coordinate_transform::processHit(double x, double y)
  {
    pushHit(x*m_x_slope + m_x_offset, y*m_y_slope + m_y_offset);
  }

  s_plane_collection coordinate_transform::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("coordinate_transform"), S_plane_def(getOutputName(), 0)));
    return ret;
  }

  const char* coordinate_transform::getType() const
  {
    return sct::plot_coordinate_transform();
  }
}
S_plot sct_plot::coordinate_transform(const char* name, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset, plot_save_option_def save_option)
{

  return S_plot(new sct_corr::coordinate_transform(name, save_option == save_to_disk, x_slope, x_offset, y_slope, y_offset));
}

S_plot sct_plot::coordinate_transform_move(const char* name, Double_t x_offset, Double_t y_offset, plot_save_option_def save_option )
{

  return   sct_plot::coordinate_transform(name, 1, x_offset, 1, y_offset, save_option);
}
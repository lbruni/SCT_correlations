#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"
#include <iostream>


class cut_x_y :public plot_hit2d{
public:
  cut_x_y(const char* name, bool save2disk, const S_XCut& xcut, const S_YCut& ycut);
  virtual void processHit(double x, double y) override;
  virtual s_plane_collection getOutputcollection();

  S_XCut m_xcut;
  S_YCut m_ycut;

  virtual const char* getType() const override;
};



cut_x_y::cut_x_y(const char* name, bool save2disk, const S_XCut& xcut, const S_YCut& ycut) :plot_hit2d(name, save2disk), m_xcut(xcut), m_ycut(ycut)
{

}

void cut_x_y::processHit(double x, double y)
{

  if (m_xcut.isOutOfRange(x))
  {
    return;
  }

  if (m_ycut.isOutOfRange(y))
  {
    return;
  }
  pushHit(x,y);
}

s_plane_collection cut_x_y::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("cut_x_y"), S_plane_def(getOutputName(), 0)));
  return ret;
}



const char* cut_x_y::getType() const
{
  return sct::plot_cut_x_y();
}

S_plot sct_plot::s_cut_x_y(const char* name, const S_XCut& x_cut, const S_YCut& y_cut, bool save2disk /*= true*/)
{
  return S_plot(new cut_x_y(name, save2disk, x_cut, y_cut));
}

const char* sct::plot_cut_x_y()
{
  return "cut_x_y___";
}


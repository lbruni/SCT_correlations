#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"
#include <iostream>


class cut_x_y :public plot_hit2d{
public:
  cut_x_y(const S_plot_def& plot_def);
  virtual void processHit(double x, double y) override;
  virtual s_plane_collection getOutputcollection();

  static const char* xmin_name(){ return "x_min___"; }
  static const char* xmax_name(){ return "x_max___"; }
  static const char* ymin_name(){ return "y_min___"; }
  static const char* ymax_name(){ return "y_max___"; }
  S_XCut xcut;
  S_YCut ycut;
};
registerPlot(cut_x_y, sct::plot_cut_x_y());
cut_x_y::cut_x_y(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{
  try{

    xcut = S_XCut(atof(m_plot_def.getParameter(xmin_name(), "-100000")), atof(m_plot_def.getParameter(xmax_name(), "100000")));
    ycut = S_YCut(atof(m_plot_def.getParameter(ymin_name(), "-100000")), atof(m_plot_def.getParameter(ymax_name(), "100000")));
    
  }
  catch (...){
    std::cout << "[cut_x_y]  unable to convert parameter" << std::endl;

  }
}

void cut_x_y::processHit(double x, double y)
{

  if (xcut.isOutOfRange(x))
  {
    return;
  }

  if (ycut.isOutOfRange(y))
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



S_plot_def sct_plot::s_cut_x_y(const char* name, const S_XCut& x_cut, const S_YCut& y_cut, bool save2disk /*= true*/)
{
  S_plot_def ret(sct::plot_cut_x_y(), name, save2disk);



  if (x_cut.m_cut_min){
    ret.setParameter(cut_x_y::xmin_name(), std::to_string(x_cut.m_min));
  }
  else{
    ret.setParameter(cut_x_y::xmin_name(), std::to_string(-100000));
  }

  if (x_cut.m_cut_max){
    ret.setParameter(cut_x_y::xmax_name(), std::to_string(x_cut.m_max));
  }
  else{
    ret.setParameter(cut_x_y::xmax_name(), std::to_string(100000));
  }


  if (y_cut.m_cut_min){
    ret.setParameter(cut_x_y::ymin_name(), std::to_string(y_cut.m_min));
  }
  else{
    ret.setParameter(cut_x_y::ymin_name(), std::to_string(-100000));
  }

  if (y_cut.m_cut_max){
    ret.setParameter(cut_x_y::ymax_name(), std::to_string(y_cut.m_max));
  }
  else{
    ret.setParameter(cut_x_y::ymax_name(), std::to_string(100000));
  }
  return ret;
}

const char* sct::plot_cut_x_y()
{
  return "cut_x_y___";
}


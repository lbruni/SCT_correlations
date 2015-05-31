#include "sct_plots.h"
#include <iostream>
#include "sct_plots_internal.h"



s_plane_collection S_plot::getOutputcollection()
{
  if (m_plot)
  {
    return m_plot->getOutputcollection();
  }
  return s_plane_collection();
}

S_plot::S_plot(const char* type, const char* name, axis_ref* x, axis_ref* y) :m_plotDef(type, name)
{
  m_plot = std::shared_ptr<plot>(create_plot(type, name, x, y));
}

S_plot::S_plot() : m_plot(nullptr), m_plotDef("error","error")
{
  std::cout << "[S_plot] unsupported default constructor" << std::endl;
}



S_plot::S_plot(const char* type, const char* name, S_plane* x, S_plane* y) :m_plotDef(type,name)
{
  m_plot = std::shared_ptr<plot>(create_plot(type, name, x, y));
}

S_plot::S_plot(const S_plot& pl) : m_plotDef(pl.m_plotDef)
{
  m_plot = pl.m_plot;
}



S_plot::S_plot(const S_plot_def& plotdef, S_plane* x, S_plane* y) :m_plotDef(plotdef)
{
  m_plot = std::shared_ptr<plot>(create_plot(plotdef, x, y));
}

S_plot::S_plot(const S_plot_def& plotdef, axis_ref* x, axis_ref* y) : m_plotDef(plotdef)
{
  m_plot = std::shared_ptr<plot>(create_plot(plotdef, x, y));
}

void S_plot::fill()
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return;
  }
  m_plot->fill();
}

Long64_t S_plot::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return -1;
  }
  return m_plot->Draw(options, cuts, axis);
}
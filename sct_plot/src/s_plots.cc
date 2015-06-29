#include "sct_plots.h"
#include <iostream>
#include "sct_plots_internal.h"
#include "internal/plotsBase.hh"
#include "s_DrawOption.h"
#include "s_plane.h"


s_plane_collection S_plot::getOutputcollection()
{
  if (m_plot)
  {
    return m_plot->getOutputcollection();
  }
  return s_plane_collection();
}


S_plot::S_plot() : m_plot(nullptr)
{
  std::cout << "[S_plot] unsupported default constructor" << std::endl;
}


const char* S_plot::getName() const
{
  return m_plot->getName();
}

const char* S_plot::getType() const 
{
  return m_plot->getType();
}
S_plot::S_plot(const S_plot& pl) 
{
  m_plot = pl.m_plot;
}


bool S_plot::fill()
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return false;
  }
 return m_plot->fill();
}

Long64_t S_plot::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  return Draw(S_DrawOption(options, cuts, axis));
}

Long64_t S_plot::Draw(const S_DrawOption& opt)
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return -1;
  }
  return m_plot->Draw(opt);
}

Long64_t S_plot::Draw(const char* options, const TCut& cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  return Draw(S_DrawOption(options, cuts, axis));
}

S_plot::S_plot(sct_corr::plot* plot_)
{
  m_plot = std::shared_ptr<sct_corr::plot>(plot_);
}
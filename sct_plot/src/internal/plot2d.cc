#include <memory>
#include "treeCollection.h"
#include "sct_plots.h"
#include "internal/plot2d.hh"
#include "internal/plane.hh"
#include "sct_plots.h"
#include <iostream>


plot2d::plot2d(const S_plot_def& plot_def) :plot(plot_def)
{
  if (m_plot_def.m_axis.size()==2)
  {
    m_x = m_plot_def.m_axis[0];
    m_y = m_plot_def.m_axis[1];
  }else if(m_plot_def.m_planes.size()==1)
  {
    m_x = m_plot_def.m_planes[0]->getX();
    m_y = m_plot_def.m_planes[0]->getY();
  }
  else {

    std::cout << "[plot2d] unable to get Axis references" << std::endl;
    return;

  }
  m_outTree = std::make_shared<treeCollection_ouput>(plot_def.m_name.c_str(), &m_x_points, &m_y_points, &m_id, &m_current, plot_def.m_save2disk);
}

Long64_t plot2d::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  return m_outTree->Draw(axis, cuts, options);
}

void plot2d::fill()
{
  m_x_points.clear();
  m_y_points.clear();
  m_id.clear();


  ProcessEvent();

  m_outTree->fill();
  ++m_current;
}

void plot2d::pushHit(Double_t x, Double_t y)
{
  m_x_points.push_back(x);
  m_y_points.push_back(y);
  m_id.push_back(0);
}

void plot2d::pushHit(Double_t x, Double_t y, Double_t ID)
{
  m_x_points.push_back(x);
  m_y_points.push_back(y);
  m_id.push_back(ID);
}

const char* plot2d::getOutputName() const
{
  return m_outTree->m_name.c_str();
}


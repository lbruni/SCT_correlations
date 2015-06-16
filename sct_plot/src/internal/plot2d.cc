#include <memory>
#include "treeCollection.h"
#include "sct_plots.h"
#include "internal/plot2d.hh"
#include "internal/plane.hh"
#include "sct_plots.h"
#include <iostream>

namespace sct_corr{

  plot2d::plot2d(const char* name, bool save2disk) :plot(name, save2disk)
  {

  }

  Long64_t plot2d::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
  {
    return Draw(S_DrawOption(options, cuts, axis));
  }

  Long64_t plot2d::Draw(const S_DrawOption& opt)
  {
    return opt.Draw(m_outTree->getTTree());
    
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

  bool plot2d::isReady()
  {
    if (m_x &&m_y)
    {
      m_outTree = std::make_shared<treeCollection_ouput>(getName(), &m_x_points, &m_y_points, &m_id, &m_current, getSave2disk());
      return true;
    }

    return false;
  }

  void plot2d::pushAxis(axis_ref* axis)
  {
    if (!m_x)
    {
      m_x = axis;
      return;
    }

    if (!m_y)
    {
      m_y = axis;
      return;
    }


    std::cout << "[plot2d::pushAxis(axis_ref* axis)] only two axis are supported\n";
  }

  void plot2d::pushPlane(S_plane* plane_)
  {
    if (!m_x&&!m_y)
    {
      m_x = plane_->getX();
      m_y = plane_->getY();
      return;
    }
    std::cout << "[plot2d::pushPlane(S_plane* plane)] only one plane is supported \n";
  }

  const char* plot2d::getOutputName() const
  {
    return m_outTree->m_name.c_str();
  }

}
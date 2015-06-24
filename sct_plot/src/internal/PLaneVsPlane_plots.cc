#include "internal/PLaneVsPlane_plots.hh"
#include "treeCollection.h"

#include <memory>
#include <iostream>
namespace sct_corr{
  plotPlaneVsPlane::plotPlaneVsPlane(const s_plot_prob& plot_prob) :plot(plot_prob)
  {

  }

  bool plotPlaneVsPlane::isReady()
  {
    if (!m_x)
    {
      return false;
      std::cout << "[plotPlaneVsPlane] first plane not set \n";
    }

    if (!m_y)
    {
      return false;
      std::cout << "[plotPlaneVsPlane] second plane not set \n";
    }
    m_outPutEvent = rootEvent_X_Y_hits(getName());
    m_outTree = std::make_shared<treeCollection_ouput>( m_outPutEvent, getSave2disk());
    return true;
  }

  void plotPlaneVsPlane::pushAxis(axis_ref* axis)
  {
    std::cout << "[plotPlaneVsPlane] axis not supported as input " << std::endl;

  }

  void plotPlaneVsPlane::pushPlane(S_plane* plane_)
  {
    if (!m_x)
    {
      m_x = plane_;
      return;
    }

    if (!m_y)
    {
      m_y = plane_;
      return;
    }


    std::cout << "[plotPlaneVsPlane] this class only supports two input planes \n";
  }

  void plotPlaneVsPlane::processEventStart()
  {

  }

  void plotPlaneVsPlane::processEventEnd()
  {

  }

  void plotPlaneVsPlane::fill()
  {
    m_size_x = 0, m_size_y = 0;
    m_outPutEvent.reset();

    bool first = true;
    processEventStart();
    while (m_y->next()){
      ++m_size_x;
      while (m_x->next()){
        if (first)
        {
          ++m_size_y;
        }
        processHit(m_x->get(), m_y->get());
      }
      first = false;
    }
    processEventEnd();
    m_outTree->fill();
    ++m_current;
  }

  void plotPlaneVsPlane::pushHit(Double_t x, Double_t y)
  {
    pushHit(x, y, 0);
  }

  void plotPlaneVsPlane::pushHit(Double_t x, Double_t y, Double_t ID)
  {
    m_outPutEvent.push_Hit(x, y, ID);
  }

  Long64_t plotPlaneVsPlane::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
  {
    return Draw(S_DrawOption(options, cuts, axis));
  }

  Long64_t plotPlaneVsPlane::Draw(const S_DrawOption& opt)
  {
    return opt.Draw(m_outTree->getTTree());
   
  }

  const char* plotPlaneVsPlane::getOutputName() const
  {
    if (m_outTree)
    {
      return m_outTree->m_name.c_str();
    }
    return getName();
  }


  plane_distance::plane_distance(const s_plot_prob& plot_prob) : plotPlaneVsPlane(plot_prob)
  {

  }

  void plane_distance::processEventStart()
  {
    m__id = 0;
    m_status = 0;
  }

  void plane_distance::processHit(const plane_hit& p1, const plane_hit& p2)
  {
    if (m__id == 0)
    {

      if (hit_abs(p2) < 0.1){
        if (TMath::Abs(p1.y) < 0.2)
        {
          m_status = 0;
        }
        else
        {
          m_status = 1;
        }
      }
      else{
        if (TMath::Abs(p1.y) < 0.2)
        {
          m_status = 10;
        }
        else
        {
          m_status = 11;

        }
      }
    }
    pushHit(p1.x, p1.y, m__id * 100 + m_status);
    ++m__id;
    //pushHit(hit_abs(p1), hit_abs(p2));
  }

  double plane_distance::hit_abs(const plane_hit& h)
  {
    return sqrt(h.x*h.x + h.y * h.y);
  }

  const char* plane_distance::getType() const
  {
    return sct::plot_plane_distance();
  }

  s_plane_collection plane_distance::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("A_and_b"), S_plane_def(getOutputName(), 0)));
    return ret;
  }

}

S_plot sct_plot::plane_distance( const s_plot_prob& plot_prob)
  {
    return S_plot(new sct_corr::plane_distance(plot_prob));
  }
#include "internal/PLaneVsPlane_plots.hh"
#include "treeCollection.h"

#include <memory>
#include <iostream>

plotPlaneVsPlane::plotPlaneVsPlane(const S_plot_def& plot_def) :plot(plot_def)
{
  if (m_plot_def.m_planes.size()==2)
  {
    m_x = m_plot_def.m_planes[0];
    m_y = m_plot_def.m_planes[1];
  }
  else{
    std::cout << "[plotPlaneVsPlane] unable to extract planes" << std::endl;
    return;
  }
  m_outTree = std::make_shared<treeCollection_ouput>(plot_def.m_name.c_str(), &m_x_points, &m_y_points, &m_id, &m_current, plot_def.m_save2disk);
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
  m_x_points.clear();
  m_y_points.clear();
  m_id.clear();

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
  m_x_points.push_back(x);
  m_y_points.push_back(y);
  m_id.push_back(0);
}

void plotPlaneVsPlane::pushHit(Double_t x, Double_t y, Double_t ID)
{
  m_x_points.push_back(x);
  m_y_points.push_back(y);
  m_id.push_back(ID);
}

Long64_t plotPlaneVsPlane::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  return m_outTree->Draw(axis, cuts, options);
}

const char* plotPlaneVsPlane::getOutputName() const
{
  return m_outTree->m_name.c_str();
}

plane_distance::plane_distance(const S_plot_def& plot_def) : plotPlaneVsPlane(plot_def)
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

s_plane_collection plane_distance::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("A_and_b"), S_plane_def(getOutputName(), 0)));
  return ret;
}
registerPlot(plane_distance, sct::plot_plane_distance());
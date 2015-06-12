#include "sct_plots.h"
#include <iostream>
#include "sct_plots_internal.h"
#include "internal/plane.hh"


  S_plane::S_plane() : m_plane(nullptr), m_plane_def("error", 0)
  {
    std::cout << "[s_plane] unsupported default constructor do not use" << std::endl;
  }


  S_plane::S_plane(const S_plane_def& plane_def, S_treeCollection* hits) : S_plane(plane_def, hits->m_tree.get())
  {


  }

  S_plane::S_plane(const S_plane_def& plane_def, sct_corr::treeCollection* hits) : m_plane_def(plane_def)
  {

    m_plane = std::make_shared<sct_corr::plane>(m_plane_def.getID(), hits);


  }





  const char * S_plane::getName() const
  {
    return m_plane_def.getName();
  }
  Double_t S_plane::getID() const
  {
    return m_plane_def.getID();
  }

  bool S_plane::next()
  {

    return m_plane->next();
  }

  plane_hit S_plane::get() const
  {
    return m_plane->get();
  }

  S_Axis S_plane::getX_def() const
  {
    return m_plane_def.getX_def();
  }

  S_Axis S_plane::getY_def() const
  {
    return m_plane_def.getY_def();
  }

  sct_corr::axis_ref* S_plane::getX() const
  {
    if (!m_plane)
    {
      std::cout << "[s_plane] plane not set " << std::endl;
      return nullptr;
    }
    return m_plane->getX();
  }

  sct_corr::axis_ref* S_plane::getY() const
  {
    if (!m_plane)
    {
      std::cout << "[s_plane] plane not set " << std::endl;
      return nullptr;
    }
    return m_plane->getY();
  }



  S_plane_def::S_plane_def(const char* name, Double_t ID) :m_name(name), m_ID(ID)
  {

  }
  Double_t S_plane_def::getID() const
  {
    return m_ID;
  }

  const char* S_plane_def::getName() const
  {
    return m_name.c_str();
  }


  S_Axis S_plane_def::getX_def() const
  {
    return S_Axis(getName(), getID(), x_axis_def);
  }
  S_Axis S_plane_def::getY_def() const
  {
    return S_Axis(getName(), getID(), y_axis_def);
  }

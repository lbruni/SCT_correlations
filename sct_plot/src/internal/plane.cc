#include "internal/plane.hh"

#include "treeCollection.h"
#include <iostream>
#include "internal/plane_hit.hh"
namespace sct_corr{


  plane::plane(double ID, rootEventBase* buffer) : m_axis(*buffer, ID), m_ID(ID)
  {

  }






  const axis_ref* plane::getAxis(const char* name) const
  {
    return m_axis.getAxis(name);
  }

 const axis_ref* plane::getAxis(axis_def ax) const
  {
    return m_axis.getAxis(ax);
  }




}
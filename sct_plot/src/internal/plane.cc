#include "internal/plane.hh"

#include "treeCollection.h"
#include <iostream>
#include "internal/plane_hit.hh"
namespace sct_corr{


  plane::plane(double ID, rootEventBase* buffer) : m_axis(*buffer, ID), m_ID(ID)
  {
    m_x = m_axis.getAxis(x_axis_def);
    m_y = m_axis.getAxis(y_axis_def);
  }

  axis_ref* plane::getX()
  {
    return m_axis.getAxis(x_axis_def);
  }

  axis_ref* plane::getY()
  {
    return m_axis.getAxis(y_axis_def);
  }

  bool plane::next()
  {
    if (m_x->next()){
      if (!m_y->next())
      {
        std::cout << "vector have different length" << std::endl;
        return false;
      }

      return true;
    }

    if (m_y->next())
    {
      std::cout << "vector have different length" << std::endl;
      return false;
    }
    return false;
  }

  plane_hit plane::get() const
  {
    return plane_hit(m_x->get(), m_y->get());
  }


}
#include "internal/plane.hh"

#include "treeCollection.h"

plane::plane(double ID, treeCollection* hits) :m_hits(hits), m_ID(ID), m_x(hits->m_buffer.m_x, hits->m_buffer.m_ID, ID), m_y(hits->m_buffer.m_y, hits->m_buffer.m_ID, ID)
{

}

axis_ref* plane::getX()
{
  return dynamic_cast<axis_ref*>(&m_x);
}

axis_ref* plane::getY()
{
  return dynamic_cast<axis_ref*>(&m_y);
}

plane::axis_vector::axis_vector(std::vector<double>* axis, std::vector<double>* ID, double planeID) :m_axis(axis), m_ID(ID), m_planeID(planeID)
{

}

bool plane::axis_vector::next()
{
  do{
    if (++m_curr >= static_cast<int>(m_ID->size()))
    {
      m_curr = -1;
      return false;
    }

  } while ((m_ID->at(m_curr) != m_planeID));

  return true;
}

double plane::axis_vector::get() const
{
  return m_axis->at(m_curr);
}

#include "internal/planeX_Y_hits.hh"
#include "internal/axis_ref.hh"
#include <iostream>
namespace sct_corr{

  planeX_Y::planeX_Y(double ID, rootEventBase* buffer) :plane(ID,buffer)
  {
    m_x = getAxis(x_axis_def);
    m_y = getAxis(y_axis_def);
  }

  const axis_ref* planeX_Y::getX() const
  {
    return getAxis(x_axis_def);
  }

  const axis_ref* planeX_Y::getY() const
  {
    return getAxis(y_axis_def);
  }




  bool planeX_Y::next() const
  {
    bool ret = m_axis.next();
    if (ret)
    {
      m_hit->x = m_x->get();
      m_hit->y = m_y->get();
    }
    return ret;
  }

  const plane_hit * planeX_Y::getHit() const
  {
    return m_hit.get();
  }

}
#include "internal/planeTrack_hits.hh"
#include <iostream>
namespace sct_corr{


  
  planeTrack_hits::planeTrack_hits(double ID, rootEventBase* buffer) :plane(ID,buffer)
  {
    m_x     = getAxis(x_axis_def);
    m_y     = getAxis(y_axis_def);
    m_chi2  = getAxis(chi2_axis_def);
    m_phi   = getAxis(phi_axis_def);
    m_theta = getAxis(theta_axis_def);
    m_hit = std::make_shared<track_hits>(0, 0, 0, 0, 0);
  }




  bool planeTrack_hits::next() const
  {
    bool ret = m_axis.next();
    if (ret)
    {
      m_hit->x = m_x->get();
      m_hit->y = m_y->get();
      m_hit->chi2 = m_chi2->get();
      m_hit->phi = m_phi->get();
      m_hit->theta = m_theta->get();
    }
    return ret;
  }

  const plane_hit * planeTrack_hits::getHit() const
  {
    return dynamic_cast<const plane_hit *>(m_hit.get());
  }

}
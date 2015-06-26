#include "sct_events/rootEvent_Track_hits.hh"

#include "internal/planeTrack_hits.hh"

namespace sct_corr{

  std::vector<std::string> getrootEvent_Track_hitsList(){
    std::vector<std::string> ret;
    ret.emplace_back(getIDString());
    ret.emplace_back(axis2String(x_axis_def));
    ret.emplace_back(axis2String(y_axis_def));
    ret.emplace_back(axis2String(chi2_axis_def));
    ret.emplace_back(axis2String(phi_axis_def));
    ret.emplace_back(axis2String(theta_axis_def));
    return ret;
  }


  rootEvent_Track_hits::rootEvent_Track_hits(const char* name) :rootEventBase(name, getrootEvent_Track_hitsList()), 
    m_x(getData(x_axis_def)), 
    m_y(getData(y_axis_def)), 
    m_id(getData(getIDString())),
    m_chi2(getData(chi2_axis_def)),
    m_phi(getData(phi_axis_def)),
    m_theta(getData(theta_axis_def))
  {

  }

  rootEvent_Track_hits::rootEvent_Track_hits()
  {

  }

  rootEvent_Track_hits::~rootEvent_Track_hits()
  {

  }

  std::shared_ptr<plane> rootEvent_Track_hits::createPlane(double ID) const
  {
    return std::dynamic_pointer_cast<plane>(std::make_shared<planeTrack_hits>(ID, this));
  }

  void rootEvent_Track_hits::push_Hit(const plane_hit& h)
  {
    
    push_Hit(h, 0);
  }

  void rootEvent_Track_hits::push_Hit(const plane_hit& h, double ID)
  {
    track_hits h1(h.x, h.y, 0, 0, 0);
    push_Hit(h1, ID);
  }

  void rootEvent_Track_hits::push_Hit(const track_hits& h, double ID)
  {
    m_x->push_back(h.x);
    m_y->push_back(h.y);
    m_id->push_back(ID);
    m_chi2->push_back(h.chi2);
    m_phi->push_back(h.phi);
    m_theta->push_back(h.theta);
    get_event_nr()++;
  }

}
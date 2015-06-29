#include "sct_events/rootEvent_X_Y_hits.hh"
#include "S_Axis.h"
#include "internal/planeX_Y_hits.hh"
#include "internal/plane_hit.hh"


namespace sct_corr{
  std::vector<std::string> get_rootEvent_X_Y_hitsList(){
    std::vector<std::string> ret;
    ret.emplace_back(getIDString());
    ret.emplace_back(axis2String(x_axis_def));
    ret.emplace_back(axis2String(y_axis_def));
    return ret;
  }
  rootEvent_X_Y_hits::rootEvent_X_Y_hits(const char* name) :rootEventBase(name, get_rootEvent_X_Y_hitsList()),
    m_x(getData(x_axis_def)),
    m_y(getData(y_axis_def)), 
    m_id(getData(getIDString()))
  {

  }



  rootEvent_X_Y_hits::rootEvent_X_Y_hits()
  {

  }

  rootEvent_X_Y_hits::~rootEvent_X_Y_hits()
  {

  }

  std::shared_ptr<plane> rootEvent_X_Y_hits::createPlane(double ID) 
  {
    return std::dynamic_pointer_cast<plane>(std::make_shared<planeX_Y>(ID, this));
  }

  void rootEvent_X_Y_hits::push_Hit(double x, double y)
  {
    push_Hit(x, y, 0);
  }

  void rootEvent_X_Y_hits::push_Hit(double x, double y, double ID)
  {
    m_x->push_back(x);
    m_y->push_back(y);
    m_id->push_back(ID);
    get_event_nr()++;

  }

  void rootEvent_X_Y_hits::push_Hit(const plane_hit& h, double ID/*=0*/)
  {
    m_x->push_back(h.x);
    m_y->push_back(h.y);
    m_id->push_back(ID);
    get_event_nr()++;
  }

}
#include "sct_events/rootEvent_X_Y_hits.hh"
#include "S_Axis.h"


std::vector<std::string> getList(){
  std::vector<std::string> ret;
  ret.emplace_back(getIDString());
  ret.emplace_back( axis2String(x_axis_def));
  ret.emplace_back(axis2String(y_axis_def));
  return ret;
}
sct_corr::rootEvent_X_Y_hits::rootEvent_X_Y_hits(const char* name) :rootEventBase(name, getList()), m_x(getData(axis2String(x_axis_def))), m_y(getData(axis2String(y_axis_def))), m_id(getData(getIDString()))
{

}



sct_corr::rootEvent_X_Y_hits::rootEvent_X_Y_hits()
{

}

sct_corr::rootEvent_X_Y_hits::~rootEvent_X_Y_hits()
{

}

void sct_corr::rootEvent_X_Y_hits::push_Hit(double x, double y)
{
  push_Hit(x, y, 0);
}

void sct_corr::rootEvent_X_Y_hits::push_Hit(double x, double y, double ID)
{
  m_x->push_back(x);
  m_y->push_back(y);
  m_id->push_back(ID);
  get_event_nr()++;
  
}

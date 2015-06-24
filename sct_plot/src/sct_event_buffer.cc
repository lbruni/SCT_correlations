#include "sct_event_buffer.h"
#include "treeCollection.h"



namespace sct_corr{


  void sct_event_buffer::set(const char * name, rootEventBase* ev)
  {
    m_events[name] = *ev;
  }

  bool sct_event_buffer::get(const char* name, rootEventBase* ev)
  {
    if (!IsCollection(name))
    {
      return false;
    }
    *ev = m_events[name];

    return true;
  }

  bool sct_event_buffer::IsCollection(const char* name)
  {
    auto it = m_events.find(name);
    if (it == m_events.end())
    {
      return false;
    }
    return true;
  }

  void sct_event_buffer::reset()
  {
    m_events.clear();
  }

  root_event::root_event(std::vector<double> *ID, std::vector<double> *x, std::vector<double> *y, Int_t* event_nr) :m_ID(ID), m_x(x), m_y(y), m_event_nr(event_nr)
  {

  }

  root_event::root_event()
  {

  }

  std::unique_ptr<axis_ref> root_event::getAxis(axis_def ax, double ID) const
  {
    return nullptr;
  }

}
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



}
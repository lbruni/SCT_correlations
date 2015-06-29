#ifndef sct_event_buffer_h__
#define sct_event_buffer_h__

#include <vector>
#include <map>
#include "Rtypes.h"
#include <memory>
#include "internal/axis_ref.hh"
#include "sct_plots.h"

#include "sct_events/rootEventBase.hh"


namespace sct_corr{



  class sct_event_buffer{
  public:
    void set(const char* name, rootEventBase* ev);
    bool get(const char* name, rootEventBase* ev);
    bool IsCollection(const char* name);
    void reset();

    std::map<std::string, rootEventBase> m_events;
  };
}
#endif // sct_event_buffer_h__

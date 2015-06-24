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



  class root_event{
  public:
    root_event(std::vector<double>  *ID,
      std::vector<double>  *x,
      std::vector<double>  *y,
      Int_t*                event_nr);
    root_event();
    std::vector<double>  *m_ID = nullptr;
    std::vector<double>  *m_x = nullptr;
    std::vector<double>  *m_y = nullptr;
    Int_t*                m_event_nr = nullptr;
  };


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

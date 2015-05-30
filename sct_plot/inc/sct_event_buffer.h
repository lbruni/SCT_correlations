#ifndef sct_event_buffer_h__
#define sct_event_buffer_h__

#include <vector>
#include <map>
#include "Rtypes.h"


class treeCollection;
class treeCollection_ouput;
class root_event{
public:
  root_event(std::vector<double>  *ID,
    std::vector<double>  *x,
    std::vector<double>  *y,
    Int_t*                event_nr);
  root_event();
  std::vector<double>  *m_ID=nullptr;
  std::vector<double>  *m_x=nullptr;
  std::vector<double>  *m_y =nullptr;
  Int_t*                m_event_nr=nullptr;
};


class sct_event_buffer{
public:
  void set(const char * name, root_event* ev);
  bool get(const char* name, root_event* ev);
  bool IsCollection(const char* name);
  void reset();

  std::map<std::string, root_event> m_events;
};
#endif // sct_event_buffer_h__

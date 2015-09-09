#ifndef rootEvent_Alibava_hits_h__
#define rootEvent_Alibava_hits_h__
#include <memory>
#include <vector>

#include "sct_events/rootEventBase.hh"
#include "internal/plane_hit.hh"
namespace sct_corr{
  class DllExport rootEvent_Alibava :public rootEventBase{
  public:
    rootEvent_Alibava(const char* name);
    rootEvent_Alibava();

    ~rootEvent_Alibava();
    virtual std::shared_ptr<plane> createPlane(double ID) ;
    void push_Hit(double x, double y);
    void push_Hit(double x, double y,double ID);
    void push_Hit(const plane_hit& h, double ID=0);
    void push_Hit(const Alibava_hit& h, double ID = 0);
  private:
    std::vector<double> *m_x=nullptr, // these are just shortcuts 
                        *m_y =nullptr,
                        *m_charge = nullptr,
                        *m_id =nullptr;
  };

}
#endif // rootEvent_X_Y_hits_h__
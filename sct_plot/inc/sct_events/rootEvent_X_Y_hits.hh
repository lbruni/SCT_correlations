#ifndef rootEvent_X_Y_hits_h__
#define rootEvent_X_Y_hits_h__
#include "sct_events/rootEventBase.hh"

namespace sct_corr{
  class rootEvent_X_Y_hits :public rootEventBase{
  public:
    rootEvent_X_Y_hits(const char* name);
    rootEvent_X_Y_hits();

    ~rootEvent_X_Y_hits();
    virtual std::shared_ptr<plane> createPlane(double ID);
    void push_Hit(double x, double y);
    void push_Hit(double x, double y,double ID);
  private:
    std::vector<double> *m_x=nullptr, // these are just shortcuts 
                        *m_y =nullptr,
                        *m_id =nullptr;
  };

}
#endif // rootEvent_X_Y_hits_h__
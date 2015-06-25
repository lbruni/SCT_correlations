#ifndef rootEvent_Track_hits_h__
#define rootEvent_Track_hits_h__
#include "rootEventBase.hh"
#include "internal\plane_hit.hh"
#include <memory>
namespace sct_corr{
  class rootEvent_Track_hits :public rootEventBase{
  public:
    rootEvent_Track_hits(const char* name);
    rootEvent_Track_hits();

    ~rootEvent_Track_hits();
    virtual std::shared_ptr<plane> createPlane(double ID);
    void push_Hit(const plane_hit& h);
    void push_Hit(const plane_hit& h, double ID);

    void push_Hit(const track_hits& h, double ID);
  private:
    std::vector<double> *m_x     = nullptr, // these are just shortcuts 
                        *m_y     = nullptr,
                        *m_id    = nullptr,
                        *m_chi2  = nullptr,
                        *m_phi   = nullptr,
                        *m_theta = nullptr;
  }; 
}
#endif // rootEvent_Track_hits_h__

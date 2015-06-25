#ifndef plane_h__
#define plane_h__

#include <vector>

#include "internal/axis_ref.hh"
#include "sct_plots.h"
#include "plane_hit.hh"
#include "sct_events/rootEventBase.hh"


namespace sct_corr{

  class plane{
  public:

    plane(double ID, rootEventBase* buffer);

   
    virtual bool next() const =0;
    
    virtual const plane_hit * getHit() const =0;
    
    
   const axis_ref* getAxis(const char* name) const;
   const axis_ref* getAxis(axis_def ax) const;
  protected:
    rootEventBaseAxisCollection m_axis;
  private:

    

    const double m_ID = 0;


  };

}
#endif // plane_h__

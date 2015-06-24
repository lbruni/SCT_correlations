#ifndef plane_h__
#define plane_h__

#include <vector>

#include "internal/axis_ref.hh"
#include "sct_plots.h"
#include "plane_hit.hh"
#include "sct_events/rootEventBase.hh"


namespace sct_corr{
  class treeCollection;
  class root_event;
  class plane{
  public:

    plane(double ID, rootEventBase* buffer);

    axis_ref* getX();
    axis_ref* getY();
    bool next();
    plane_hit get() const;

  private:

    rootEventBaseAxisCollection m_axis;
    axis_ref* m_x;
    axis_ref* m_y;
    const double m_ID = 0;


  };

}
#endif // plane_h__

#ifndef find_coressponding_h__
#define find_coressponding_h__


#include "internal/PLaneVsPlane_plots.hh"

class plot_find_correspondingX :public plotPlaneVsPlane{
public:
  plot_find_correspondingX(const  S_plot_def& plot_def);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
  virtual s_plane_collection getOutputcollection();
};

class plot_find_correspondingXY :public plotPlaneVsPlane{
public:
  plot_find_correspondingXY(const  S_plot_def& plot_def);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
  virtual s_plane_collection getOutputcollection();
};

#endif // find_coressponding_h__

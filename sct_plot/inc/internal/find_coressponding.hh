#ifndef find_coressponding_h__
#define find_coressponding_h__


#include "internal/PLaneVsPlane_plots.hh"

class plot_find_correspondingX :public plotPlaneVsPlane{
public:
  plot_find_correspondingX(const char* name, bool save2disk);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
  virtual s_plane_collection getOutputcollection();
  virtual const char* getType() const override;
};

class plot_find_correspondingXY :public plotPlaneVsPlane{
public:
  plot_find_correspondingXY(const char* name, bool save2disk);
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
  virtual s_plane_collection getOutputcollection();
  virtual const char* getType() const override;
};

#endif // find_coressponding_h__

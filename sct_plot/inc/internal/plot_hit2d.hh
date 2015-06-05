#ifndef plot_hit2d_h__
#define plot_hit2d_h__

#include "internal/plot2d.hh"

class plot_hit2d :public plot2d{
public:
  plot_hit2d(const S_plot_def& plot_def);
  virtual void processEventStart();
  virtual void processHit(double x, double y) = 0;
  virtual void processEventEnd();
  virtual void ProcessEvent() override final;
};

#endif // plot_hit2d_h__

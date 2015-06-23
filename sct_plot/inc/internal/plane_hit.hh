#ifndef plane_hit_h__
#define plane_hit_h__
#include "Rtypes.h"

struct plane_hit
{
  plane_hit(Double_t x_, Double_t y_) :x(x_), y(y_){}
  Double_t x, y;
};

#endif // plane_hit_h__

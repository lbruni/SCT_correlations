#ifndef plane_hit_h__
#define plane_hit_h__
#include "Rtypes.h"


namespace sct_corr{
  class plane_hit
  {
  public:
    plane_hit(Double_t x_, Double_t y_) :x(x_), y(y_){}
    Double_t x, y;
  };

  class track_hits :public plane_hit{
  public:
    track_hits(Double_t x_, Double_t y_, Double_t chi2_, Double_t ndf_, Double_t phi_, Double_t theta_) :plane_hit(x_, y_), chi2(chi2_), phi(phi_), theta(theta_), ndf(ndf_) {}


    Double_t chi2, phi,theta,ndf;
  };
}


#endif // plane_hit_h__

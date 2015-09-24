#ifndef s_plane_h__
#define s_plane_h__

#include "internal/platform.hh"
#include "internal/plane_hit.hh"
#include "Rtypes.h"
#include <memory>
#include "S_Axis.h"

class S_Axis;
class S_treeCollection;


namespace sct_corr{
  class treeCollection;
  class axis_ref;
  class plane;
}



class DllExport S_plane{
public:
  S_plane();


  S_plane(const S_plane_def& plane_def, sct_corr::treeCollection* hits);
  void setTreeCollection(sct_corr::treeCollection* hits);
  sct_corr::plane* getPlane() const;





  const char * getName() const;
  Double_t getID() const;
  bool next();



  const sct_corr::axis_ref* getX() const;
  const sct_corr::axis_ref* getY() const;
  const sct_corr::axis_ref* getAxis(axis_def ax) const;

  std::shared_ptr<S_plane_def> m_plane_def;
  std::shared_ptr<sct_corr::plane> m_plane;


};






#endif // s_plane_h__

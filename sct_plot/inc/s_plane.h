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

class DllExport S_plane_def{

public:
  S_plane_def(const char* name, Double_t ID);

  Double_t getID() const;
  const char* getName() const;

  S_Axis getX_def() const;
  S_Axis getY_def() const;
  S_Axis get_Axis(axis_def) const;
  const std::vector<S_Axis>& get_axis_defs() const;
#ifndef __CINT__
protected:
  std::vector<S_Axis> m_axis;

private:
  Double_t m_ID = 0;
  std::string m_name;
  
#endif
};

class DllExport S_plane_def_GBL :public S_plane_def {
public:
  S_plane_def_GBL(const char* name, Double_t ID);
  S_Axis getChi2_def() const;
  S_Axis getNdf_def() const;
  S_Axis getPhi_def() const;
  S_Axis getTheta_def() const;
};
class DllExport S_plane{
public:
  S_plane();
#ifndef __CINT__

  S_plane(const S_plane_def& plane_def, sct_corr::treeCollection* hits);
  void setTreeCollection(sct_corr::treeCollection* hits);
  sct_corr::plane* getPlane() const;
#endif

  S_plane(const S_plane_def& plane_def, S_treeCollection* hits);


  const char * getName() const;
  Double_t getID() const;
  bool next();



  const sct_corr::axis_ref* getX() const;
  const sct_corr::axis_ref* getY() const;
#ifndef __CINT__
  S_plane_def m_plane_def;
  std::shared_ptr<sct_corr::plane> m_plane;
#endif
  ClassDef(S_plane, 0);
};
class DllExport s_plane_collection{
public:
  s_plane_collection(const S_plane_def& plane_);
  s_plane_collection(){}
  S_plane_def get(Int_t i) const;
  s_plane_collection getByName(const char* name) const;
  s_plane_collection getByType(const char* type) const;
  S_plane_def get(const char* name, const char* type) const;
  s_plane_collection get(const char* nameOrType) const;
  S_plane_def operator()() const;
  const char* getName(Int_t i) const;
  void showNames() const;
  Int_t size() const;
  void push_back(const S_plane_def& pl);
  void push_back(const char* name, const S_plane_def& pl);
#ifndef __CINT__
  std::vector<std::pair<std::string, S_plane_def>> m_planes;
#endif
  ClassDef(s_plane_collection, 0);
};

class DllExport s_plane_collection_correlations :public s_plane_collection {
public:
  s_plane_collection_correlations();

  void setResidual(const S_plane_def& plane_);
  void setResidualVsMissing(const S_plane_def& plane_);
  void setTotalTrueHits(const S_plane_def& plane_);
  void setTrueHitsWithDUT(const S_plane_def& plane_);
  S_plane_def getResidual() const;
  S_plane_def getResidualVSmissing() const;
  S_plane_def getTotalTrueHits() const;
  S_plane_def getTrueHitsWithDUT() const;
  ClassDef(s_plane_collection_correlations, 0);
};

DllExport s_plane_collection operator+(s_plane_collection pl1, const s_plane_collection& pl2);
DllExport s_plane_collection operator+(s_plane_collection pl1, const S_plane_def& pl2);
DllExport s_plane_collection operator+(const S_plane_def& pl1, const S_plane_def& pl2);

#ifdef __CINT__
#pragma link C++ class S_plane;
#pragma link C++ class S_plane_def;
#pragma link C++ class S_plane_def_GBL;
#pragma link C++ class s_plane_collection;
#pragma link C++ class s_plane_collection_correlations;
#pragma link C++ function  operator+(s_plane_collection , const s_plane_collection& );
#pragma link C++ function  operator+(s_plane_collection , const S_plane_def& );
#pragma link C++ function  operator+(const S_plane_def& , const S_plane_def& );
#endif

#endif // s_plane_h__

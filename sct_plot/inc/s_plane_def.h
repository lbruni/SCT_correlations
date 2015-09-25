#ifndef s_plane_def_h__
#define s_plane_def_h__



#include "internal/platform.hh"
#include "internal/plane_hit.hh"
#include "Rtypes.h"
#include <memory>
#include "S_Axis.h"
#include "sct_types.h"

class S_Axis;
class S_treeCollection;

class s_plane_collection;
class S_DrawOption;
namespace sct_corr {
class treeCollection;
class axis_ref;
class plane;
struct Xlayer;
}

class DllExport S_plane_def {

public:
  S_plane_def(const sct_type::collectionName_t& name, const sct_type::ID_t& ID, const sct_corr::Xlayer* layer_ = nullptr);
  Long64_t Draw(const S_DrawOption& ) const;
  const sct_type::ID_t& getID() const;
  const sct_type::collectionName_t& getName() const;

  S_Axis getX_def() const;
  S_Axis getY_def() const;
  S_Axis get_Axis(axis_def) const;
  const std::vector<S_Axis>& get_axis_defs() const;
  const sct_corr::Xlayer* getLayer() const;

  void set_plot_collection(std::weak_ptr<sct_corr::plot_collection> plot_collection);
  virtual std::shared_ptr<S_plane_def> copy() const;
  std::shared_ptr<sct_corr::plot_collection> get_plot() const;
protected:
  std::vector<S_Axis> m_axis;

private:
  std::weak_ptr<sct_corr::plot_collection> m_plot;

  sct_type::ID_t m_ID = sct_type::ID_t(0);
  sct_type::collectionName_t m_name;
  std::shared_ptr<sct_corr::Xlayer> m_layer;


};

DllExport S_plane_def s_error_plane_def();




class DllExport S_plane_def_GBL :public S_plane_def {
public:
  S_plane_def_GBL(const sct_type::collectionName_t& name, const sct_type::ID_t& ID, const sct_corr::Xlayer* layer_ = nullptr);
  S_Axis getChi2_def() const;
  S_Axis getNdf_def() const;
  S_Axis getPhi_def() const;
  S_Axis getTheta_def() const;
#ifndef __CINT__
  virtual std::shared_ptr<S_plane_def> copy() const override;
#endif
 
};



class DllExport S_plane_def_Alibava :public S_plane_def {
public:
  S_plane_def_Alibava(const sct_type::collectionName_t& name, const sct_type::ID_t& ID, const sct_corr::Xlayer* layer_ = nullptr);
  S_Axis getCharge_def() const;
#ifndef __CINT__
  virtual std::shared_ptr<S_plane_def> copy() const override;
#endif
 
};
class DllExport s_plane_collection {
public:
#ifndef __CINT__
  void set_plot_collection(std::weak_ptr<sct_corr::plot_collection> plot_collection__);
#endif
  s_plane_collection(const S_plane_def& plane_);
  s_plane_collection() {}
  S_plane_def get(Int_t i) const;
  s_plane_collection getByName(const sct_type::collectionName_t& name) const;
  s_plane_collection getByType(const char* type) const;
  S_plane_def get(const sct_type::collectionName_t&  name, const char* type) const;
  s_plane_collection get(const char* nameOrType) const;
  S_plane_def operator()() const;
  const char* getName(Int_t i) const;
  void showNames() const;
  Int_t size() const;
  void push_back(const S_plane_def& pl);
  void push_back(const char* name, const S_plane_def& pl);
  void clear();
#ifndef __CINT__
  std::vector<std::pair<std::string, S_plane_def>> m_planes;
#endif
  
};



class DllExport s_plane_collection_correlations :public s_plane_collection {
public:
  s_plane_collection_correlations();

  void setResidual(const S_plane_def& plane_);
  void setResidualVsMissing(const S_plane_def& plane_);
  void setTotalTrueHits(const S_plane_def& plane_);
  void setTrueHitsWithDUT(const S_plane_def& plane_);
  void setDUT_Hits(const S_plane_def& plane_);
  S_plane_def getResidual() const;
  S_plane_def getResidualVSmissing() const;
  S_plane_def getTotalTrueHits() const;
  S_plane_def getTrueHitsWithDUT() const;
  S_plane_def getDUT_Hits() const;
  
};
class DllExport s_plane_collection_find_closest :public s_plane_collection {
public:
  s_plane_collection_find_closest();

  void setResidual(const S_plane_def& plane_);
  void setHitOnPlaneA(const S_plane_def& plane_);
  void setHitOnPlaneB(const S_plane_def& plane_);
  S_plane_def getResidual() const;
  S_plane_def getHitOnPlaneA() const;
  S_plane_def getHitOnPlaneB() const;
 
};

DllExport S_plane_def s_error_plane_def();

DllExport s_plane_collection operator+(s_plane_collection pl1, const s_plane_collection& pl2);
DllExport s_plane_collection operator+(s_plane_collection pl1, const S_plane_def& pl2);
DllExport s_plane_collection operator+(const S_plane_def& pl1, const S_plane_def& pl2);





#endif // s_plane_def_h__

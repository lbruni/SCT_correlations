#ifndef s_file_base_h__
#define s_file_base_h__
#include "s_plot_collection.h"
#include <memory>
#include "TFile.h"
#include "s_plane_def.h"
#include "s_cuts.h"
#include "s_plot_prob.h"
#include "internal/platform.hh"
#include "geometry/setup_description.hh"

using ID_t = double;
using residualCut_t = double;
using rot_angle_t = double;
using move_t = double;

class DllExport s_file {
public:
  s_file(std::shared_ptr<S_plot_collection> plot_collection, const sct_corr::Xgear* gear_ = nullptr);
  virtual ~s_file() {}


  S_plane_def get_plane(const char* collection_name, ID_t plane_ID) const;
  
  S_plot_collection* get_collection();
  
protected:
   S_plot_collection* get_plot_collection() const;
   sct_corr::Xgear* get_gear() const;
  std::shared_ptr<S_plot_collection> m_plot_collection;
private:
  const sct_corr::Xlayer* get_layer(ID_t ID) const;
  std::unique_ptr<sct_corr::Xgear> m_gear;
};



class DllExport s_file_fitter : public s_file{
public:
  s_file_fitter(std::shared_ptr<S_plot_collection> plot_collection, const sct_corr::Xgear* gear_ = nullptr);
  s_file_fitter(const char* Fitter_File, const char* gear_file);
  virtual ~s_file_fitter() {}

  s_plane_collection_correlations get_correlations(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& plot_prob_ = ""
    ) const ;

  s_plane_collection_correlations get_correlations_channel(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& = ""
    ) const ;


  S_plane_def apix_hit_local() const;
  S_plane_def apix_hit() const;
  S_plane_def apix_zs_data() const;
  S_plane_def apix_fitted() const;
  S_plane_def apix_fitted_local() const;

  S_plane_def DUT_hit_local() const;
  S_plane_def DUT_hit() const;
  S_plane_def DUT_zs_data() const;
  S_plane_def DUT_fitted() const;
  S_plane_def DUT_fitted_local() const;
  S_plane_def DUT_TTC_data() const;
  S_plane_def DUT_Timestamp() const;

  S_plane_def DUT_Timestamp_L0ID() const;

  S_plane_def     DUT_TDC_L0ID() const;
  S_plane_def     DUT_TLU_TLUID() const;
  S_plane_def     tel_hit_local(double ID) const;
  S_plane_def     tel_hit(double ID) const;
  S_plane_def     tel_zs_data(double ID) const;
  S_plane_def     tel_fitted(double ID) const;
  S_plane_def     tel_fitted_local(double ID) const;

  S_plane_def_GBL DUT_fitted_local_GBL() const;
  S_plane_def_GBL tel_fitted_local_GBL(double ID) const;


private:
  s_plane_collection_correlations get_Daff_correlations(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& plot_prob_ = ""
    ) const;

  s_plane_collection_correlations get_GBL_correlations(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& plot_prob_ = ""
    ) const;
  s_plane_collection_correlations get_Daff_correlations_channel(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& = ""
    ) const;
  s_plane_collection_correlations get_GBL_correlations_channel(
    const S_Cut& fiducial_cut_,
    residualCut_t residualCut,
    rot_angle_t rotate_angle,
    move_t move_x,
    const s_plot_prob& plot_prob_ = ""
    ) const;
};


class DllExport s_alibava_file :public s_file {
public:
  s_alibava_file(std::shared_ptr<S_plot_collection> plot_collection, const sct_corr::Xgear* gear_ = nullptr);
  virtual ~s_alibava_file() {}
  S_plane_def_Alibava Alibava_sz_data() const;
  S_plane_def_GBL DUT_fitted_local_GBL() const;
};



#endif // s_file_base_h__

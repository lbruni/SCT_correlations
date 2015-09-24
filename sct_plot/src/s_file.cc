#include "s_file_base.h"
#include "geometry/setup_description.hh"
#include "sct_plots.h"
#include "s_plot_collection.h"
#include "s_plane_def.h"
#include "s_cuts.h"
#include "s_plot_collection.h"
#include "s_plane_def.h"
#include "s_DrawOption.h"


#include "s_process_collection.h"
#include "sct_plots_internal.h"

#include "TF1.h"
#include "TProfile.h"
#include "geometry/setup_description.hh"
#include "s_plane_def.h"
#include "sct_processors.h"
#include "sct_default_names.h"
#include "sct_types.h"



namespace sct_files {

base_file::base_file(std::shared_ptr<sct_corr::plot_collection> plot_collection___, const sct_corr::Xgear* gear_ /*= nullptr*/) :m_plot_collection(plot_collection___) {

  if (gear_) {
    m_gear = std::unique_ptr<sct_corr::Xgear>(new sct_corr::Xgear(*gear_));
  }
}



S_plane_def base_file::get_plane(const sct_type::collectionName_t& collection_name, sct_type::ID_t ID) const {

  S_plane_def ret(collection_name, ID, get_layer(ID));

  ret.set_plot_collection(m_plot_collection);
  return ret;
}

sct_corr::plot_collection* base_file::get_collection() {
  return m_plot_collection.get();
}
sct_corr::plot_collection* base_file::get_plot_collection() const {
  return m_plot_collection.get();
}
sct_corr::Xgear* base_file::get_gear() const {

  if (!m_gear) {
    return nullptr;
  }

  return m_gear.get();
}


const sct_corr::Xlayer*  base_file::get_layer(sct_type::ID_t ID) const {

  if (!m_gear) {
    return nullptr;
  }

  return m_gear->detector.layer_by_ID(Un_necessary_CONVERSION(ID));
}

fitter_file::fitter_file(const char* Fitter_File_name, const char* gear_file) :fitter_file(sct_corr::create_plot_collection(), &(sct_corr::load_gear(gear_file))) {

  m_main_file = std::make_shared<TFile>(Fitter_File_name);
  m_plot_collection->addFile(m_main_file.get());

}

fitter_file::fitter_file(std::shared_ptr<sct_corr::plot_collection> plot_collection___, const sct_corr::Xgear* gear_ /*= nullptr*/) :base_file(plot_collection___, gear_) {

}
S_plane_def fitter_file::apix_hit_local() const {
  return get_plane(sct::col_local_hit(), sct_type::ID_t(20));
}

S_plane_def fitter_file::apix_hit() const {
  return get_plane(sct::col_hit(), sct_type::ID_t(20));
}

S_plane_def fitter_file::apix_zs_data() const {
  return get_plane(sct::col_zsdata_apix(), sct_type::ID_t(20));
}


S_plane_def fitter_file::apix_fitted() const {
  return get_plane(sct::col_fitpoints(), sct_type::ID_t(20));
}

S_plane_def fitter_file::apix_fitted_local() const {
  return get_plane(sct::col_fitpoints_local(), sct_type::ID_t(20));
}


S_plane_def fitter_file::DUT_hit_local() const {
  return get_plane(sct::col_local_hit(), sct_type::ID_t(8));
}


S_plane_def fitter_file::DUT_hit() const {
  return get_plane(sct::col_hit(), sct_type::ID_t(8));
}


S_plane_def fitter_file::DUT_zs_data() const {
  return   get_plane(sct::col_zsdata_strip(), sct_type::ID_t(8));
}

S_plane_def fitter_file::DUT_fitted() const {
  return   get_plane(sct::col_fitpoints(), sct_type::ID_t(8));
}

S_plane_def fitter_file::DUT_fitted_local() const {
  return   get_plane(sct::col_fitpoints_local(), sct_type::ID_t(8));
}

S_plane_def fitter_file::DUT_TTC_data() const {
  return get_plane(sct::col_DUT_TTC(), sct_type::ID_t(9));
}

S_plane_def fitter_file::DUT_Timestamp() const {
  return get_plane(sct::col_DUT_TTC(), sct_type::ID_t(10));
}

S_plane_def fitter_file::DUT_Timestamp_L0ID() const {
  return get_plane(sct::col_DUT_TTC(), sct_type::ID_t(11));
}

S_plane_def fitter_file::DUT_TDC_L0ID() const {
  return get_plane(sct::col_DUT_TTC(), sct_type::ID_t(12));
}

S_plane_def fitter_file::DUT_TLU_TLUID() const {
  return get_plane(sct::col_DUT_TTC(), sct_type::ID_t(13));
}

S_plane_def fitter_file::tel_hit_local(const sct_type::ID_t& ID) const {
  return get_plane(sct::col_local_hit(), ID);
}

S_plane_def fitter_file::tel_hit(const sct_type::ID_t& ID) const {
  return get_plane(sct::col_hit(), ID);
}

S_plane_def fitter_file::tel_zs_data(const sct_type::ID_t& ID) const {
  return get_plane(sct::col_zsdata_m26(), ID);
}

S_plane_def fitter_file::tel_fitted(const sct_type::ID_t& ID) const {
  return get_plane(sct::col_fitpoints(), ID);
}

S_plane_def fitter_file::tel_fitted_local(const sct_type::ID_t& ID) const {
  return get_plane(sct::col_fitpoints_local(), ID);
}


S_plane_def_GBL fitter_file::DUT_fitted_local_GBL() const {
  S_plane_def_GBL ret(sct::col_GBL_fitted_points(), sct_type::ID_t(8));

  ret.set_plot_collection(m_plot_collection);
  return ret;
}


S_plane_def_GBL fitter_file::tel_fitted_local_GBL(const sct_type::ID_t& ID) const {
  S_plane_def_GBL ret(sct::col_GBL_fitted_points(), ID);

  ret.set_plot_collection(m_plot_collection);
  return ret;
}


s_plane_collection_correlations fitter_file::get_correlations(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_ /*= "" */
  ) const {


  if (get_plot_collection()->collectionExist(sct::col_GBL_fitted_points())) {

    return get_GBL_correlations(fiducial_cut_, residualCut, rotate_angle, move_x, plot_prob_);
  }

  if (get_plot_collection()->collectionExist(sct::col_fitpoints_local())) {

    return get_Daff_correlations(fiducial_cut_, residualCut, rotate_angle, move_x, plot_prob_);
  }
  std::cout << "collection not found " << std::endl;
  return s_plane_collection_correlations();
}


s_plane_collection_correlations fitter_file::get_GBL_correlations(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_
  ) const {


  auto loc = sct_processor::convert_local_to_global(
    apix_hit_local(),
    *get_gear()->detector.layer_by_ID(20),
    s_plot_prob().doNotSaveToDisk()
    );


  auto loc11 = sct_processor::convert_global_to_local(
    loc,
    *get_gear()->detector.layer_by_ID(8),
    s_plot_prob().doNotSaveToDisk()
    );




  auto trueHits = sct_processor::find_nearest(
    loc11,
    DUT_fitted_local_GBL(),
    1,  //x cut in mm
    1, //y cut in mm
    s_plot_prob().doNotSaveToDisk()
    ).getHitOnPlaneB();

  auto dut_rotated_17 = sct_processor::rotate(
    trueHits,
    rotate_angle.value,
    s_plot_prob().doNotSaveToDisk()
    );

  auto dut_rotated_17_move = sct_processor::coordinate_transform_move(
    dut_rotated_17,
    move_x.value,
    0,
    s_plot_prob().doNotSaveToDisk()
    );

  auto trueHits_cut = sct_processor::cut_x_y(
    dut_rotated_17_move,
    fiducial_cut_,
    s_plot_prob().SaveToDisk()
    );

  std::string find_closest_name = plot_prob_.getName().value + "_closest";

  auto find_closest = sct_processor::find_nearest_strip(
    trueHits_cut,
    DUT_hit_local(),
    x_axis_def,
    residualCut.value,
    s_plot_prob(find_closest_name.c_str())
    .setSaveOptione(plot_prob_.getPlotSaveOption())
    );

  std::string res_vs_missing_name = plot_prob_.getName().value + "_res_vs_missing";

  auto res_vs_missing = sct_processor::hitmap(
    find_closest.getResidual().getX_def(),
    find_closest.getHitOnPlaneA().getY_def(),
    s_plot_prob(res_vs_missing_name.c_str())
    .setSaveOptione(plot_prob_.getPlotSaveOption())
    );

  s_plane_collection_correlations ret;
  ret.setResidual(find_closest.getResidual());
  ret.setResidualVsMissing(res_vs_missing);
  ret.setTotalTrueHits(trueHits_cut);
  ret.setTrueHitsWithDUT(find_closest.getHitOnPlaneA());
  ret.setDUT_Hits(find_closest.getHitOnPlaneB());
  return ret;
}


s_plane_collection_correlations fitter_file::get_Daff_correlations(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_ /*= "" */
  ) const {


  std::cout << "[fitter_file::get_Daff_correlations]: not implemented \n";
  s_plane_collection_correlations ret;
  return ret;
}


s_plane_collection_correlations fitter_file::get_Daff_correlations_channel(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_) const {

  std::cout << "[fitter_file::get_Daff_correlations_channel]: not implemented \n";
  s_plane_collection_correlations ret;
  return ret;
}

s_plane_collection_correlations fitter_file::get_GBL_correlations_channel(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_
  ) const {




  auto apix_global = sct_processor::convert_local_to_global(
    apix_hit_local(),
    *get_gear()->detector.layer_by_ID(20),
    s_plot_prob().doNotSaveToDisk()
    );


  auto apix_on_DUT = sct_processor::convert_global_to_local(
    apix_global,
    *get_gear()->detector.layer_by_ID(8),
    s_plot_prob().doNotSaveToDisk()
    );


  auto trueHits = sct_processor::find_nearest(
    apix_on_DUT,
    DUT_fitted_local_GBL(),
    1, // residual cut x
    1,  // residual cut y
    s_plot_prob().SaveToDisk()
    ).getHitOnPlaneB();

  auto trueHits_cut = sct_processor::cut_x_y(
    trueHits,
    fiducial_cut_,
    s_plot_prob().doNotSaveToDisk()
    );

  std::string trueHitsInStrips_name = plot_prob_.getName().value + "_true";

  auto trueHitsInStrips = sct_processor::convert_hits_to_zs_data_GBL(
    trueHits_cut,
    *get_gear()->detector.layer_by_ID(8),
    s_plot_prob().doNotSaveToDisk()
    );


  auto dut_rotated_17 = sct_processor::rotate(
    trueHitsInStrips,
    rotate_angle.value,
    s_plot_prob().doNotSaveToDisk()
    );

  auto dut_rotated_17_move = sct_processor::coordinate_transform_move(
    dut_rotated_17,
    move_x.value,
    0,
    s_plot_prob(trueHitsInStrips_name.c_str())
    .setSaveOptione(plot_prob_.getPlotSaveOption())
    );


  std::string find_closest_name = plot_prob_.getName().value + "_closest";


  auto find_closest = sct_processor::find_nearest_strip(
    dut_rotated_17_move,
    DUT_zs_data(),
    x_axis_def,
    residualCut.value,
    s_plot_prob(find_closest_name.c_str())
    .setSaveOptione(plot_prob_.getPlotSaveOption())
    );

  std::string res_vs_missing_name = plot_prob_.getName().value + "_res_vs_missing";
  auto res_vs_missing = sct_processor::hitmap(
    find_closest.getResidual().getX_def(),
    find_closest.getHitOnPlaneA().getY_def(),
    s_plot_prob(res_vs_missing_name.c_str())
    .setSaveOptione(plot_prob_.getPlotSaveOption())
    );

  s_plane_collection_correlations ret;
  ret.setResidual(find_closest.getResidual());
  ret.setResidualVsMissing(res_vs_missing);
  ret.setTotalTrueHits(dut_rotated_17_move);
  ret.setTrueHitsWithDUT(find_closest.getHitOnPlaneA());
  ret.setDUT_Hits(find_closest.getHitOnPlaneB());
  return ret;
}

s_plane_collection_correlations fitter_file::get_correlations_channel(
  const S_Cut& fiducial_cut_,
  sct_type::residualCut_t residualCut,
  sct_type::rot_angle_t rotate_angle,
  sct_type::move_t move_x,
  const s_plot_prob& plot_prob_/*= "" */
  ) const {

  if (get_plot_collection()->collectionExist(sct::col_GBL_fitted_points())) {

    return get_GBL_correlations_channel(fiducial_cut_, residualCut, rotate_angle, move_x, plot_prob_);
  }

  if (get_plot_collection()->collectionExist(sct::col_fitpoints_local())) {

    return get_Daff_correlations_channel(fiducial_cut_, residualCut, rotate_angle, move_x, plot_prob_);
  }
  std::cout << "collection not found " << std::endl;
  return s_plane_collection_correlations();
}


alibava_file::alibava_file(
  std::shared_ptr<sct_corr::plot_collection> plot_collection__,
  const sct_corr::Xgear* gear_ /*= nullptr*/
  ) :base_file(plot_collection__, gear_) {

}

S_plane_def_Alibava alibava_file::Alibava_sz_data() const {
  S_plane_def_Alibava ret(sct_type::collectionName_t("alibava"), sct_type::ID_t(6));

  ret.set_plot_collection(m_plot_collection);
  return ret;
}

S_plane_def_GBL alibava_file::DUT_fitted_local_GBL() const {
  S_plane_def_GBL ret(sct_type::collectionName_t("telescope"), sct_type::ID_t(0));

  ret.set_plot_collection(m_plot_collection);
  return ret;
}
}
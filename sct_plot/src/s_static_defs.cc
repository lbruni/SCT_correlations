#include "sct_plots.h"
#include "s_plane.h"


const char* sct::col_hit()
{
  return "hit";
}

const char* sct::col_zsdata_strip()
{
  return "zsdata_strip";
}

const char* sct::col_zsdata_m26()
{
  return "zsdata_m26";
}

const char* sct::col_zsdata_apix()
{
  return "zsdata_apix";
}

const char* sct::col_fitpoints()
{
  return "fitpoints";
}

const char* sct::col_fitpoints_local()
{
  return "fitpoints_local";
}

const char* sct::col_local_hit()
{
  return "local_hit";
}
const char* sct::col_DUT_TTC()
{
  return "zsdata_strip_TTC";
}
const char* sct::col_GBL_fitted_points() {
  return "GBL_tracks";
}

const char* sct::plot_hitmap()
{
  return "hitmap";
}
const char* sct::plot_modulo() {
  return "modulo";
}
const char* sct::plot_correlation()
{
  return "correlation";
}

const char* sct::plot_residual()
{
  return "residual";
}

const char* sct::plot_clusterSize()
{
  return "clusterSize";
}

const char* sct::plot_projectOnPixel()
{
  return "ProjectOnPixel";
}

const char* sct::plot_find_correspondingX()
{
  return  "plot_find_correspondingX";
}

const char* sct::plot_find_correspondingXY()
{
  return  "plot_find_correspondingXY";
}

const char* sct::plot_Event_size()
{
  return "plot_Event_size_____";
}

const char* sct::plot_find_nearest()
{
  return "plot_find_nearest";
}

const char* sct::plot_plane_distance()
{
  return "plot_plane_distance____";
}

const char* sct::plot_find_nearest_strip()
{
  return "find_nearest_strip___";
}

const char* sct::plot_A_if_B()
{
  return "plot_A_If_b";
}

const char* sct::plot_rotated()
{
  return "plot_rotated___";
}



const char* sct::plot_coordinate_transform()
{
  return "coordinate_transform____";
}

const char* sct::plot_hitMultiplizity()
{
  return "hitMultiplizity___";
}




















S_plane_def sct_coll::apix_hit_local()
{
  return S_plane_def(sct::col_local_hit(), 20);
}

S_plane_def sct_coll::apix_hit()
{
  return S_plane_def(sct::col_hit(), 20);
}

S_plane_def sct_coll::apix_zs_data()
{
  return S_plane_def(sct::col_zsdata_apix(), 20);
}



S_plane_def sct_coll::apix_fitted()
{
  return S_plane_def(sct::col_fitpoints(), 20);
}

S_plane_def sct_coll::apix_fitted_local()
{
  return S_plane_def(sct::col_fitpoints_local(), 20);
}

S_plane_def sct_coll::DUT_hit_local()
{
  return S_plane_def(sct::col_local_hit(), 8);
}

S_plane_def sct_coll::DUT_hit()
{
  return S_plane_def(sct::col_hit(), 8);
}

S_plane_def sct_coll::DUT_zs_data()
{
  return S_plane_def(sct::col_zsdata_strip(), 8);
}



S_plane_def sct_coll::DUT_fitted()
{
  return S_plane_def(sct::col_fitpoints(), 8);
}

S_plane_def sct_coll::DUT_fitted_local()
{
  return S_plane_def(sct::col_fitpoints_local(), 8);
}

S_plane_def sct_coll::DUT_TTC_data()
{
  return S_plane_def(sct::col_DUT_TTC(), 9);
}
S_plane_def sct_coll::DUT_Timestamp()
{
  return S_plane_def(sct::col_DUT_TTC(), 10);
}
S_plane_def sct_coll::DUT_Timestamp_L0ID()
{
  return S_plane_def(sct::col_DUT_TTC(), 11);
}
S_plane_def sct_coll::DUT_TDC_L0ID()
{
  return S_plane_def(sct::col_DUT_TTC(), 12);
}
S_plane_def sct_coll::DUT_TLU_TLUID()
{
  return S_plane_def(sct::col_DUT_TTC(), 13);
}


S_plane_def sct_coll::tel_hit_local(double ID)
{
  return S_plane_def(sct::col_local_hit(), ID);
}

S_plane_def sct_coll::tel_hit(double ID)
{
  return S_plane_def(sct::col_hit(), ID);
}

S_plane_def sct_coll::tel_zs_data(double ID)
{
  return S_plane_def(sct::col_zsdata_m26(), ID);
}



S_plane_def sct_coll::tel_fitted(double ID)
{
  return S_plane_def(sct::col_fitpoints(), ID);
}

S_plane_def sct_coll::tel_fitted_local(double ID)
{
  return S_plane_def(sct::col_fitpoints_local(), ID);
}


S_plane_def_GBL sct_coll::DUT_fitted_local_GBL() {
  return S_plane_def_GBL(sct::col_GBL_fitted_points(), 8);
}
S_plane_def_GBL sct_coll::tel_fitted_local_GBL(double ID) {
  return S_plane_def_GBL(sct::col_GBL_fitted_points(), ID);
}

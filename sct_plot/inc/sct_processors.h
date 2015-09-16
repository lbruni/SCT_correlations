#ifndef sct_processors_h__
#define sct_processors_h__
#include "s_plane_def.h"
#include "internal/platform.hh"
#include "s_cuts.h"
#include "s_plot_prob.h"




class DllExport sct_processor {
public:
  static  S_plane_def cut_x_y(
    const S_plane_def& planeA,
    const  S_Cut& cut_,
    const s_plot_prob& = ""
    );


  static  S_plane_def cluster_strip(
    const S_plane_def& hits,
    axis_def search_axis,
    double cluster_distance,
    const s_plot_prob& = ""
    );


  static  S_plane_def coordinate_transform(
    const S_plane_def& planeA,
    Double_t x_slope,
    Double_t x_offset,
    Double_t y_slope,
    Double_t y_offset,
    const s_plot_prob& = ""
    );


  static  S_plane_def coordinate_transform_move(
    const S_plane_def& planeA,
    Double_t x_offset,
    Double_t y_offset,
    const s_plot_prob& = ""
    );

  static s_plane_collection_find_closest find_nearest(
    const S_plane_def& planeA,
    const S_plane_def& planeB,
    Double_t x_cutoff,
    Double_t y_cutoff,
    const s_plot_prob& = ""
    );


  static s_plane_collection_find_closest find_nearest_strip(
    const S_plane_def& planeA,
    const S_plane_def& planeB,
    axis_def search_axis,
    Double_t cutOfff,
    const s_plot_prob& plot_prob_ = ""
    );

  static s_plane_collection_find_closest modulo_find_nearest_strip(
    const S_plane_def& planeA,
    const S_plane_def& planeB,
    axis_def search_axis,
    Double_t cutOfff,
    Double_t modulo_parameter,
    const s_plot_prob& plot_prob_ = ""
    );




  static  S_plane_def correlation(
    const S_Axis& axisA,
    const S_Axis& axisB,
    const s_plot_prob& = ""
    );


  static  S_plane_def correlation(
    const S_plane_def& planeA,
    const s_plot_prob& = ""
    );

  static  S_plane_def rotate(
    const S_plane_def& planeA,
    Double_t angle,
    const s_plot_prob& = ""
    );

  static  S_plane_def linear_trans(
    const S_plane_def& planeA,
    Double_t a11, Double_t a21,
    Double_t a12, Double_t a22,
    const s_plot_prob& = ""
    );

  static S_plane_def hitmap(
    const S_plane_def& hits,
    const s_plot_prob& = ""
    );


  static  S_plane_def hitmap(
    const S_Axis& axisA,
    const S_Axis& axisB,
    const s_plot_prob& = ""
    );

  static  S_plane_def moduloHitMap(
    const S_Axis& axisA,
    const S_Axis& axisB,
    double mod_x, double mod_y,
    const s_plot_prob& = ""
    );


  static  S_plane_def moduloHitMap(
    const S_plane_def& hits,
    double mod_x, double mod_y,
    const s_plot_prob& = ""
    );




  static  S_plane_def residual(
    const S_Axis& axisA,
    const S_Axis& axisB,
    const s_plot_prob& = ""
    );

  static  S_plane_def_Alibava residual_with_charge(
    const S_plane_def_Alibava& hits_A,
    const S_plane_def& hits_B, 
    bool full_cluster=true,
    const s_plot_prob& = ""
    );
  static S_plane_def convert_local_to_global(
    const S_plane_def& local_hits, 
    const sct_corr::Xlayer&, 
    const s_plot_prob& = ""
    );
  static S_plane_def convert_global_to_local(
    const S_plane_def& local_hits, 
    const sct_corr::Xlayer&, 
    const s_plot_prob& = ""
    );
  static S_plane_def convert_hits_to_zs_data_GBL(
    const S_plane_def& hits, 
    const sct_corr::Xlayer& layer, 
    const s_plot_prob& plot_prob_ /*= ""*/
    );


  static  S_plane_def convert_zs_data_to_hits_GBL(
    const S_plane_def& sz_data, 
    const sct_corr::Xlayer& layer, 
    const s_plot_prob& plot_prob_/*= ""*/
    );

  static S_plane_def convert_zs_data_to_hits_DAF(
    const S_plane_def& sz_data, 
    const sct_corr::Xlayer& layer, 
    const s_plot_prob& plot_prob_/*= ""*/
    );

  static  S_plane_def convert_hits_to_zs_data_DAF(
    const S_plane_def& hits, 
    const sct_corr::Xlayer& layer, 
    const s_plot_prob& plot_prob_
    );
};

#ifdef __CINT__

#pragma link C++ class sct_processor;

#endif
#endif // sct_processors_h__

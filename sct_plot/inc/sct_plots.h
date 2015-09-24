#ifndef sct_plots_h__
#define sct_plots_h__

#ifndef __CINT__
#include <memory>
#include <vector>
#endif


#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TF1.h"
#include "Rtypes.h"
#include "TCut.h"



#include "internal/platform.hh"
#include "s_plot.h"
#include "S_Axis.h"
#include "s_plot_prob.h"
#include "s_plot_collection.h"
#include "s_DrawOption.h"
#include "s_plane_def.h"



class TH2D;

class S_plane;
class S_DrawOption;
class S_Axis;
class S_treeCollection;
class S_Cut;
class s_plane_collection;
class s_process_collection;
class S_plane_def;

namespace sct_corr {
  class treeCollection;
  
  class plot_collection;
  class axis_ref;
  class plot;
  class plane;
  class sct_event_buffer;
  struct Xgear;
  struct Xlayer;
}

class TH2;







class S_plot_def;
class S_plot;
class DllExport sct_plot {
public:
  static  S_plot hitmap(const s_plot_prob& = "");
  static  S_plot moduloHitMap(double modulo_parameter_x, double modulo_parameter_y, const s_plot_prob& = "");
  static  S_plot correlation(const s_plot_prob& = "");
  static  S_plot residual(const s_plot_prob& = "");
  static  S_plot clustering(Double_t Pixel_distance = 2, const s_plot_prob& = "");
  static  S_plot clustering_strip(axis_def searchAxis, Double_t Pixel_distance = 2, const s_plot_prob& = "");
  static  S_plot projectOnPixel(const s_plot_prob& = "");
  static  S_plot find_correspondingX(const s_plot_prob& = "");
  static  S_plot find_correspondingXY(const s_plot_prob& = "");
  static  S_plot Event_size(const s_plot_prob& = "");
  static  S_plot find_nearest(Double_t x_cutoff, Double_t y_cutoff, const s_plot_prob& = "");
  static  S_plot find_nearest_strip(axis_def search_axis, Double_t cutOfff = 100000, const s_plot_prob& = "");
  static  S_plot modulo_find_nearest_strip(axis_def search_axis, double modulo_param, Double_t cutOfff = 100000, const s_plot_prob& = "");

  static  S_plot plane_distance(const s_plot_prob& = "");
  static  S_plot A_if_B(const s_plot_prob& = "");
  static  S_plot rotated(Double_t angle, const s_plot_prob& = "");
  static  S_plot coordinate_transform(Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset, const s_plot_prob& = "");
  static  S_plot coordinate_transform_move(Double_t x_offset, Double_t y_offset, const s_plot_prob& = "");

  static  S_plot efficiency_map(Double_t x_bin, Double_t y_bin, Double_t x_cut, Double_t y_cut, const s_plot_prob& = "");
  static  S_plot efficiency_map(Double_t x_bin, Double_t y_bin, const s_plot_prob& = "");
  static  S_plot hitMultiplizity(const s_plot_prob& = "");

  static  S_plot cut_x_y(const  S_Cut& cut_, const s_plot_prob& = "");


  static  S_plot save2LCIO(const char* filename, unsigned runnum, const s_plot_prob& = "");

  static  S_plot linear_trans(Double_t a11, Double_t a21 ,Double_t a12,Double_t a22, const s_plot_prob& = "");


};













#endif // sct_plots_h__

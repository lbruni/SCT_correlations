#ifndef sct_plots_h__
#define sct_plots_h__




#ifdef WIN32 
#ifndef __CINT__
#define DllExport   __declspec( dllexport )
#else
#define DllExport   
#endif // __CINT__
#else 
#define DllExport   
#endif // WIN32



#ifndef __CINT__
#include <memory>
#endif

#include "TTree.h"
#include "TFile.h"


#include "TH1.h"
#include "TString.h"

#include "Rtypes.h"


class S_plane;

class DllExport sct_coll{
public:
static  S_plane apix_hit_local();
static  S_plane apix_hit();
static  S_plane apix_zs_data();
static  S_plane apix_fitted();
static  S_plane apix_fitted_local();
static  S_plane DUT_hit_local();
static  S_plane DUT_hit();
static  S_plane DUT_zs_data();
static  S_plane DUT_fitted();
static  S_plane DUT_fitted_local();
static  S_plane tel_hit_local(double ID);
static  S_plane tel_hit(double ID);
static  S_plane tel_zs_data(double ID);
static  S_plane tel_fitted(double ID);
static  S_plane tel_fitted_local(double ID);
};

class DllExport sct{
public:
  static const char* col_hit();
  static const char*  col_zsdata_strip();
  static const char*  col_zsdata_m26();
  static const char*  col_zsdata_apix();
  static const char* col_fitpoints();
  static const char* col_fitpoints_local();
  static const char* col_local_hit();
  static const char* plot_hitmap();
  static const char* plot_correlation();
  static const char* plot_residual();
  static const char* plot_clusterSize();
  static const char* plot_projectOnPixel();
  static const char* plot_find_correspondingX();
  static const char* plot_find_correspondingXY();
  static const char* plot_Event_size();
  static const char* plot_find_nearest();
  static const char* plot_plane_distance();
  static const char* plot_find_nearest_strip();
  static const char* plot_A_if_B();
  static const char* plot_rotated();
};
class S_plot_def;
class DllExport sct_plot{
public:
static  S_plot_def s_hitmap(const char* name);
static  S_plot_def s_correlation(const char* name);
static  S_plot_def s_residual(const char* name);
static  S_plot_def s_clusterSize(const char* name);
static  S_plot_def s_projectOnPixel(const char* name);
static  S_plot_def s_find_correspondingX(const char* name);
static  S_plot_def s_find_correspondingXY(const char* name);
static  S_plot_def s_Event_size(const char* name);
static  S_plot_def s_find_nearest(const char* name);
static  S_plot_def s_plane_distance(const char* name);
static  S_plot_def s_find_nearest_strip(const char* name);
static  S_plot_def s_A_if_B(const char* name);
static  S_plot_def s_rotated(const char* name ,Double_t angle);
};
class treeCollection;

class axis_ref;
class plot;
class plane;
class S_treeCollection;
class S_Axis;
struct plane_hit
{
  plane_hit(Double_t x_, Double_t y_) :x(x_), y(y_){}
  Double_t x, y;
};


class DllExport S_plane{
public:
  S_plane();
#ifndef __CINT__
  S_plane(double ID, treeCollection* hits);
  void setTreeCollection(treeCollection* hits);
#endif
  S_plane(double ID, S_treeCollection* hits);
  S_plane(const char* name,Double_t ID);
  bool isSetTreeCollectionSet() const;
  void setTreeCollection(S_treeCollection* hits);
  const char * getName() const;
  bool next() ;
  plane_hit get() const;

  S_Axis getX_def() const;
  S_Axis getY_def() const;

  axis_ref* getX() const;
  axis_ref* getY() const;
#ifndef __CINT__
private:
  Double_t m_ID =0 ;
  std::string m_name;
  std::shared_ptr<plane> m_plane;
#endif
  ClassDef(S_plane, 0);
};


class DllExport S_plot_def{
public:
  S_plot_def();
  S_plot_def(const char* type, const char* name);
  void setParameter(const char* tag, const char* value);
  const char * getParameter(const char* tag, const char* default_value);
#ifndef __CINT__

  void setParameter(const std::string & tag, const std::string& value);
  std::string getParameter(const std::string &  tag, const std::string &  default_value);


  std::map<std::string, std::string> m_tags;
  std::string m_name, m_type;
#endif
  ClassDef(S_plot_def, 0);
};


class DllExport S_plot{
public:
  S_plot();
  S_plot(const S_plot& );
  S_plot(const char* type, const char* name, axis_ref* x, axis_ref* y);
  S_plot(const char* type, const char* name, S_plane* x, S_plane* y);
  S_plot(S_plot_def plotdef, S_plane* x, S_plane* y);
  S_plot(S_plot_def plotdef, axis_ref* x, axis_ref* y);
  void fill();
  Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x");
#ifndef __CINT__
private:
  std::shared_ptr<plot> m_plot;
#endif
  ClassDef(S_plot, 0);
};



enum  axis_def{
  x_axis_def,
  y_axis_def
};
class DllExport S_Axis{
public:
  S_Axis(const char* collctionName, double planeID, axis_def axis);

  TString m_collectionName;
  double m_planeID;
  axis_def m_axis;
  ClassDef(S_Axis, 0);
};

class DllExport S_DrawOption{
public:

  S_DrawOption(const char* options = "colz", const char* cuts = "", const char * axis = "y:x");
  TString m_options = "colz", m_cuts = "", m_axis = "x:y";
  ClassDef(S_DrawOption, 0);
};
class S_treeCollection{
public:
  S_treeCollection(TTree *tree);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Int_t    GetEntries() const;
#ifndef __CINT__
  std::shared_ptr<treeCollection> m_tree;
#endif // !__CINT__
  ClassDef(S_treeCollection, 0);
};
class  DllExport S_plot_collection{
public:
  S_plot_collection(TFile* file);
  void addFile(TFile* file);
  void reset();
  void addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis);

  void addPlot(S_plot_def plot_def, S_Axis x_axis, S_Axis y_axis);
  void addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis, S_DrawOption option);
  void addPlot(const char* name,S_plot pl );
  void addPlot(const char* PlotType, const char* name, S_plane p1 ,S_plane  p2);
  void addPlot(S_plot_def plot_def, S_plane p1, S_plane  p2);
  void Draw();
  Long64_t Draw(const char* name);
  Long64_t Draw(const char* name, const S_DrawOption& option);

  void loop(Int_t last = -1, Int_t start = 0);
#ifndef __CINT__
private:
  axis_ref* getAxis_ref(const S_Axis & axis);
  treeCollection* getCollection(const char* name);

  S_plane* getPlane(double ID, treeCollection* coll);
  S_plane* pushPlane(S_plane pl);
  std::vector<std::shared_ptr<S_plane>> m_planes;
  std::vector<std::pair<std::string, S_plot>> m_plots;
  std::map<std::string, S_DrawOption> m_drawOption;
 std::vector< std::pair<std::string, treeCollection*>> m_trees;
  std::vector<TFile*> m_file;
#endif
  ClassDef(S_plot_collection, 0);
};





#endif // sct_plots_h__

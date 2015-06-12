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
#include <vector>
#endif

#include "TTree.h"
#include "TFile.h"


#include "TH1.h"
#include "TString.h"

#include "Rtypes.h"
#include "TCut.h"
enum  axis_def
#ifndef __CINT__
  :int
#endif // !__CINT__
{
  x_axis_def,
  y_axis_def
};
class S_plane;
class S_DrawOption;
 class S_Axis;
  class S_treeCollection;

namespace sct_corr{
  class treeCollection;

  class axis_ref;
  class plot;
  class plane;
  class sct_event_buffer;

}
class DllExport S_Cut
{
public:

  virtual ~S_Cut(){}
  
#ifndef __CINT__
  virtual std::shared_ptr<S_Cut> copy()const { return nullptr; }
#endif // __CINT__

  virtual bool isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const { return false; }
  virtual bool isOutOfRange(Double_t BinContent, Double_t x)  const { return false; }
  virtual bool isOutOfRange(Double_t BinContent)  const { return false; }
  ClassDef(S_Cut, 0);
};
class DllExport S_Cut_min_max: public S_Cut
{
public:
  virtual ~S_Cut_min_max(){}
  S_Cut_min_max(Double_t min_, Double_t max_);
  S_Cut_min_max(Double_t min_);

protected:
#ifndef __CINT__

  Double_t m_min, m_max;
  Bool_t m_cut_min, m_cut_max;
  bool isOutOfRange_intern(Double_t val)  const;
#endif // !__CINT__

  ClassDef(S_Cut, 0);
};

class DllExport  S_XCut :public S_Cut_min_max
{
public:
  S_XCut(Double_t min_, Double_t max_);
  S_XCut(Double_t min_);
#ifndef __CINT__
  virtual std::shared_ptr<S_Cut> copy()const {
    return  std::shared_ptr<S_Cut>(new S_XCut(*this));
  }
#endif // __CINT__
  virtual bool isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const;
  virtual bool isOutOfRange(Double_t BinContent, Double_t x) const;
  virtual bool isOutOfRange(Double_t BinContent)  const;

  ClassDef(S_XCut, 0);
};
class DllExport S_YCut :public S_Cut_min_max
{
public:
  S_YCut(Double_t min_, Double_t max_);
  S_YCut(Double_t min_);
#ifndef __CINT__
  virtual std::shared_ptr<S_Cut> copy()const {
    return  std::shared_ptr<S_Cut>(new S_YCut(*this));
  }
#endif // __CINT__
  virtual bool isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const;
  virtual bool isOutOfRange(Double_t BinContent, Double_t x) const;
  virtual bool isOutOfRange(Double_t BinContent) const;
  ClassDef(S_YCut, 0);
};
class  DllExport S_Cut_BinContent :public S_Cut_min_max
{
public:
  S_Cut_BinContent(Double_t min_, Double_t max_);
  S_Cut_BinContent(Double_t min_);
#ifndef __CINT__
  virtual std::shared_ptr<S_Cut> copy()const {
    return  std::shared_ptr<S_Cut>(new S_Cut_BinContent(*this));
  }
#endif // __CINT__
  virtual bool isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const;
  virtual bool isOutOfRange(Double_t BinContent, Double_t x) const;
  virtual bool isOutOfRange(Double_t BinContent) const;

  ClassDef(S_Cut_BinContent, 0);
};

class  DllExport S_CutCoollection :public S_Cut
{
public:

#ifndef __CINT__
  virtual std::shared_ptr<S_Cut> copy()const {
    return  std::shared_ptr<S_Cut>(new S_CutCoollection(*this));
  }
#endif // __CINT__
  S_CutCoollection();
  virtual bool isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const;
  virtual bool isOutOfRange(Double_t BinContent, Double_t x) const;
  virtual bool isOutOfRange(Double_t BinContent) const;
  void add_Cut(const S_Cut& cut_);
  void add_Cut(const S_CutCoollection& cut_);
#ifndef __CINT__
  std::vector<std::shared_ptr<S_Cut> > m_cuts;
#endif // !__CINT__
  ClassDef(S_CutCoollection, 0);
};
DllExport S_CutCoollection operator+(const S_Cut& cut_a, const S_Cut& cut_b);

class TH2;
class DllExport SCT_helpers{
public:
  static void CutTH2(TH2* h2, const S_Cut& cut_);
  static void CutTH1(TH1* h1, const S_Cut& cut_);
  static TH1* HistogrammSilhouette(TH2* h2, axis_def ax);
};
class S_plane_def;
class DllExport sct_coll{
public:
static  S_plane_def apix_hit_local();
static  S_plane_def apix_hit();
static  S_plane_def apix_zs_data();
static  S_plane_def apix_fitted();
static  S_plane_def apix_fitted_local();
static  S_plane_def DUT_hit_local();
static  S_plane_def DUT_hit();
static  S_plane_def DUT_zs_data();
static  S_plane_def DUT_fitted();
static  S_plane_def DUT_fitted_local();
static  S_plane_def tel_hit_local(double ID);
static  S_plane_def tel_hit(double ID);
static  S_plane_def tel_zs_data(double ID);
static  S_plane_def tel_fitted(double ID);
static  S_plane_def tel_fitted_local(double ID);
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
  static const char* plot_coordinate_transform();
  static const char* plot_hitMultiplizity();
  static const char* plot_efficieny_map();
  static const char* plot_cut_x_y();
};
class S_plot_def;
class S_plot;
class DllExport sct_plot{
public:
  static  S_plot s_hitmap(const char* name, bool save2disk = true);
  static  S_plot s_correlation(const char* name, bool save2disk = true);
static  S_plot s_residual(const char* name, bool save2disk = true);
static  S_plot s_clustering(const char* name, Double_t Pixel_distance = 2, bool save2disk = true);
static  S_plot s_projectOnPixel(const char* name, bool save2disk = true);
static  S_plot s_find_correspondingX(const char* name, bool save2disk = true);
static  S_plot s_find_correspondingXY(const char* name, bool save2disk = true);
static  S_plot s_Event_size(const char* name, bool save2disk = true);
static  S_plot s_find_nearest(const char* name, Double_t x_cutoff, Double_t y_cutoff, bool save2disk = true);
static  S_plot s_find_nearest_strip(const char* name, axis_def search_axis, Double_t cutOfff = 100000, bool save2disk = true);
static  S_plot s_plane_distance(const char* name, bool save2disk = true);
static  S_plot s_A_if_B(const char* name, bool save2disk = true);
static  S_plot s_rotated(const char* name, Double_t angle, bool save2disk = true);
static  S_plot s_coordinate_transform(const char* name, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset, bool save2disk = true);
static  S_plot s_coordinate_transform_move(const char* name, Double_t x_offset, Double_t y_offset, bool save2disk = true);

static  S_plot s_efficiency_map(const char* name, Double_t x_bin, Double_t y_bin, Double_t x_cut, Double_t y_cut, bool save2disk = true);
static  S_plot s_efficiency_map(const char* name, Double_t x_bin, Double_t y_bin, bool save2disk = true);
static  S_plot s_hitMultiplizity(const char* name, bool save2disk = true);

static  S_plot s_cut_x_y(const char* name, const  S_XCut& x_cut, const  S_YCut& y_cut, bool save2disk = true);


static  S_plot s_save2LCIO(const char* name,const char* filename,unsigned runnum );

};

struct plane_hit
{
  plane_hit(Double_t x_, Double_t y_) :x(x_), y(y_){}
  Double_t x, y;
};

class DllExport S_plane_def{

public:
  S_plane_def(const char* name, Double_t ID);

  Double_t getID() const;
  const char* getName() const;

  S_Axis getX_def() const;
  S_Axis getY_def() const;
#ifndef __CINT__

private:
  Double_t m_ID = 0;
  std::string m_name;

#endif
};
class DllExport S_plane{
public:
  S_plane();
#ifndef __CINT__

  S_plane(const S_plane_def& plane_def, sct_corr::treeCollection* hits);
  void setTreeCollection(sct_corr::treeCollection* hits);
#endif

  S_plane(const S_plane_def& plane_def, S_treeCollection* hits);


  const char * getName() const;
  Double_t getID() const;
  bool next() ;
  plane_hit get() const;

  S_Axis getX_def() const;
  S_Axis getY_def() const;

  sct_corr::axis_ref* getX() const;
  sct_corr::axis_ref* getY() const;
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
  S_plane_def get(const char* name,const char* type) const;
  s_plane_collection get(const char* nameOrType) const;
  S_plane_def operator()() const;
  const char* getName(Int_t i) const;
  void showNames() const;
  Int_t size() const;
  void push_back(const S_plane_def& pl);
  void push_back(const char* name,const S_plane_def& pl);
#ifndef __CINT__
  std::vector<std::pair<std::string, S_plane_def>> m_planes;
#endif
  ClassDef(s_plane_collection, 0);
};
DllExport s_plane_collection operator+(s_plane_collection pl1, const s_plane_collection& pl2);
DllExport s_plane_collection operator+(s_plane_collection pl1, const S_plane_def& pl2);
DllExport s_plane_collection operator+(const S_plane_def& pl1, const S_plane_def& pl2);
class DllExport S_plot_def{
public:
  
  S_plot_def(const char* type, const char* name,bool save2disk=true);
  void setParameter(const char* tag, const char* value);
  const char * getParameter(const char* tag, const char* default_value);

 

#ifndef __CINT__

  void setParameter(const std::string & tag, const std::string& value);
  std::string getParameter(const std::string &  tag, const std::string &  default_value);

  std::vector<S_plane*> m_planes;
  std::vector<sct_corr::axis_ref*> m_axis;
  std::map<std::string, std::string> m_tags;
  std::string m_name, m_type;
  bool m_save2disk;
#endif
  ClassDef(S_plot_def, 0);
private:
  S_plot_def(){}
};


class DllExport S_plot{
public:
  S_plot();
  S_plot(const S_plot& );
  void fill();
  Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x");
  Long64_t Draw(const char* options, const TCut& cuts , const char* axis = "y:x");
  Long64_t Draw(const S_DrawOption& opt);
  s_plane_collection getOutputcollection();
  const char* getName() const;
  const char* getType() const;
#ifndef __CINT__
  S_plot(sct_corr::plot* plot_);
  
  std::shared_ptr<sct_corr::plot> m_plot;
#endif
  ClassDef(S_plot, 0);
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

  S_DrawOption(const char* options, const char* cuts ="", const char * axis = "y:x");
  S_DrawOption(const char* options, TCut cuts , const char * axis = "y:x");
  S_DrawOption();
  S_DrawOption& options(const char* option);
  S_DrawOption& opt_colz();
  S_DrawOption& cut(const char* cut_);
  S_DrawOption& cut_add(const TCut& cut_);
  S_DrawOption& cut_x(Double_t min_,Double_t max_);
  S_DrawOption& cut_x_min(Double_t min_);
  S_DrawOption& cut_x_max(Double_t max_);
  S_DrawOption& cut_y(Double_t min_, Double_t max_);
  S_DrawOption& cut_y_min(Double_t min_);
  S_DrawOption& cut_y_max(Double_t max_);
  S_DrawOption& draw_axis(const char* axis_);
  S_DrawOption& draw_x();
  S_DrawOption& draw_y();
  S_DrawOption& draw_x_VS_y();
  S_DrawOption& draw_y_VS_x();
  S_DrawOption& output_object(TObject* out_);

  const char* getOptions() const;
  const char* getAxis() const;
  TCut getCut() const;
private:
#ifndef __CINT__

  std::string m_options = "colz", m_axis = "x:y";
  TCut m_cut;
  TObject* m_output_object = nullptr;
  mutable std::string m_axis_dummy;
#endif // !__CINT__

  ClassDef(S_DrawOption, 0);
};

class S_treeCollection{
public:
  S_treeCollection(TTree *tree);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Int_t    GetEntries() const;
#ifndef __CINT__
  std::shared_ptr<sct_corr::treeCollection> m_tree;
#endif // !__CINT__
  ClassDef(S_treeCollection, 0);
};




class  DllExport S_plot_collection{
public:
  S_plot_collection(TFile* file);
  void addFile(TFile* file);
  void reset();
  s_plane_collection addPlot( S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis);

  s_plane_collection addPlot(S_plot plot_def, const S_plane_def& p1, const S_plane_def & p2);
  s_plane_collection addPlot(S_plot plot_def, const  S_plane_def& p1);
  s_plane_collection addPlot(S_plot plot_def, const  s_plane_collection& p1);
  void Draw();
  Long64_t Draw(const char* name);
  Long64_t Draw(const char* name, const S_DrawOption& option);
  Long64_t Draw(const S_plane_def& name, const S_DrawOption& option);
  Long64_t Draw(const s_plane_collection& name, const S_DrawOption& option);
  Long64_t DrawAll(const s_plane_collection& name, const S_DrawOption& option);
  void loop(Int_t last = -1, Int_t start = 0);
#ifndef __CINT__
private:
  s_plane_collection addPlot_internal(S_plot plot_def);
  sct_corr::axis_ref* getAxis_ref(const S_Axis & axis);
  sct_corr::treeCollection* getCollection(const char* name);

  S_plane* getPlane(double ID, sct_corr::treeCollection* coll);
  S_plane* pushPlane(const S_plane_def& pl);

  std::shared_ptr<sct_corr::sct_event_buffer> m_eventBuffer;

  std::vector<std::shared_ptr<S_plane>> m_planes;
  std::vector<std::pair<std::string, S_plot>> m_plots;
  std::map<std::string, S_DrawOption> m_drawOption;
  std::vector< std::pair<std::string, sct_corr::treeCollection*>> m_trees;
  std::vector<TFile*> m_file;
#endif
  ClassDef(S_plot_collection, 0);
};





#endif // sct_plots_h__

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

class DllExport sct{
public:
  static const char* col_hit();
  static const char*  col_zsdata_strip();
  static const char*  col_zsdata_m26();
  static const char*  col_zsdata_apix();


  static  const char* plot_hitmap();
  static  const char* plot_correlation();
  static  const char* plot_residual();
};
class treeCollection;

class axis_ref;
class plot;
class plane;
class S_treeCollection;

class DllExport S_plane{
public:
  S_plane();
#ifndef __CINT__
  S_plane(double ID, treeCollection* hits);
#endif
  S_plane(double ID, S_treeCollection* hits);


  axis_ref* getX();
  axis_ref* getY();
#ifndef __CINT__
private:
  std::shared_ptr<plane> m_plane;
#endif
  ClassDef(S_plane, 0);
};





class DllExport S_plot{
public:
  S_plot();
  S_plot(const char* type, const char* name, axis_ref* x, axis_ref* y);

  void fill();
  void Draw(const char* options, const char* cuts = "", const char* axis = "y:x");
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

  void reset();
  void addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis);

  void addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis, S_DrawOption option);

  void Draw();
  void Draw(const char* name);
  void Draw(const char* name, const S_DrawOption& option);

  void loop(Int_t last = -1, Int_t start = 0);
#ifndef __CINT__
private:
  axis_ref* getAxis_ref(const S_Axis & axis);
  treeCollection* getCollection(const char* name);

  S_plane* getPlane(double ID, treeCollection* coll);

  std::vector<S_plane> m_planes;
  std::map<std::string, S_plot> m_plots;
  std::map<std::string, S_DrawOption> m_drawOption;
  std::map<std::string, treeCollection*> m_trees;
  TFile *m_file = NULL;
#endif
  ClassDef(S_plot_collection, 0);
};





#endif // sct_plots_h__

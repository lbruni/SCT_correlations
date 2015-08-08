#ifndef s_plot_collection_h__
#define s_plot_collection_h__
#include "internal/platform.hh"
#include "Rtypes.h"
#include <memory>
#include <map>

class TFile;
class TTree;
class s_plane_collection;
class S_plot;
class S_DrawOption;
class S_plane_def;
class S_Axis;
class S_plane;
namespace sct_corr{
  class axis_ref;
  class treeCollection;
  class sct_event_buffer;
}
class  DllExport S_plot_collection{
public:
  S_plot_collection(TFile* file);
  void addFile(TFile* file);
  void setOutputFile(TFile* file);
  void reset();
  s_plane_collection addPlot(S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis);

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
  bool collectionExist(const char* name);
#ifndef __CINT__
private:
  Int_t getMaxEntriesFromTree(Int_t last);
  s_plane_collection addPlot_internal(S_plot plot_def);
  const sct_corr::axis_ref* getAxis_ref(const S_Axis & axis);
  sct_corr::treeCollection* getCollection(const char* name);
  TTree*                    getTTree(const char* name) const;
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
#ifdef __CINT__

#pragma link C++ class S_plot_collection;
#endif // __CINT__

#endif // s_plot_collection_h__

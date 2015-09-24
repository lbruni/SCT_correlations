#ifndef plot_collection_impl_hh___
#define plot_collection_impl_hh___
#include <memory>
#include <map>

#include "internal/platform.hh"
#include <vector>

namespace sct_corr {
class  DllExport plot_collection_impl :public plot_collection {
public:
  plot_collection_impl();
  virtual ~plot_collection_impl();

  virtual void addFile(TFile* file) override;
  virtual void setOutputFile(TFile* file) override;
  virtual void reset() override;
  virtual s_plane_collection addPlot(S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis) override;

  virtual s_plane_collection addPlot(S_plot plot_def, const S_plane_def& p1, const S_plane_def & p2) override;
  virtual s_plane_collection addPlot(S_plot plot_def, const  S_plane_def& p1) override;
  virtual s_plane_collection addPlot(S_plot plot_def, const  s_plane_collection& p1) override;

  virtual void loop(Int_t last = -1, Int_t start = 0) override;


  virtual Long64_t Draw(const char* name, const S_DrawOption& option) override;
  virtual Long64_t Draw(const S_plane_def& name, const S_DrawOption& option) override;




  bool collectionExist(const char* name)  const override;

  void set_self_weak_pointer(std::weak_ptr<plot_collection> self_);
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
  std::vector< std::pair<std::string, sct_corr::treeCollection*>> m_trees;
  std::vector<TFile*> m_file;
  std::weak_ptr<plot_collection> m_self;

};

}

#endif
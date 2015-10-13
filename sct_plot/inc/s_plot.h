#ifndef s_plot_h__
#define s_plot_h__

#include "internal/platform.hh"
#include "Rtypes.h"
#include <memory>
#include <vector>
#include <map>
#include "plot_save_option_def.h"
#include "internal/plotsBase.hh"

class S_DrawOption;
class s_plane_collection;
class TCut;

class S_plane;


namespace sct_corr{
  class plot;
  class axis_ref;
}




class DllExport S_plot{
public:
  S_plot();
  S_plot(const S_plot&);
  int fill();
  Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x");
  Long64_t Draw(const char* options, const TCut& cuts, const char* axis = "y:x");
  Long64_t Draw(const S_DrawOption& opt);
  s_plane_collection getOutputcollection();
  const char* getName() const;
  const char* getType() const;
#ifndef __CINT__
  S_plot(sct_corr::plot* plot_);

  std::shared_ptr<sct_corr::plot> m_plot;
#endif

};


#ifdef __CINT__





#endif

#endif // s_plot_h__

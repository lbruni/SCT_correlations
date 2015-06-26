#ifndef s_plot_h__
#define s_plot_h__

#include "internal/platform.hh"
#include "Rtypes.h"
#include <memory>
#include <vector>
#include <map>
#include "plot_save_option_def.h"

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
  bool fill();
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
  ClassDef(S_plot, 0);
};

class DllExport S_plot_def{
public:

  S_plot_def(const char* type, const char* name, plot_save_option_def save_option = save_to_disk);
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
#ifdef __CINT__


#pragma link C++ class S_plot;

#pragma link C++ class S_plot_def;
#endif

#endif // s_plot_h__

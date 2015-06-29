#ifndef s_plot_prob_h__
#define s_plot_prob_h__

#include "internal/platform.hh"
#include "Rtypes.h"
#include <string>
#include "plot_save_option_def.h"
class DllExport s_plot_prob{
public:
  s_plot_prob();
  s_plot_prob(const char* name);
  s_plot_prob& setName(const char* name);
  s_plot_prob& setSaveOptione(plot_save_option_def save_);
  s_plot_prob& doNotSaveToDisk();
  s_plot_prob& SaveToDisk();
  const char* getName() const;
  bool getNameSize() const;
  plot_save_option_def getPlotSaveOption() const;

#ifndef __CINT__
  std::string m_name;
  plot_save_option_def m_save = save_to_disk;
#endif
  ClassDef(s_plot_prob, 0);
};

#ifdef __CINT__

#pragma link C++ class s_plot_prob;
#endif
#endif // s_plot_prob_h__

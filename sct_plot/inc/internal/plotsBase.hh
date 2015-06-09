#ifndef plotsBase_h__
#define plotsBase_h__
#include "sct_plots.h"


#include "factory.hh"

#define registerPlot(derivedClass,identifier) registerClass(plot,derivedClass,identifier) 

class plot{
public:

  using MainType= std::string;
  using Parameter_t = S_plot_def;
  using Parameter_ref = const Parameter_t &;
  plot(Parameter_ref plot_def);
  virtual void fill() = 0;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
  virtual void setParameter(const char* tag, const char * value);
  virtual s_plane_collection getOutputcollection();
  Parameter_t m_plot_def;
  virtual const char* getOutputName()  const = 0 ;
  static std::unique_ptr < plot > create(MainType name, Parameter_ref para);
  static std::unique_ptr < plot > create(Parameter_ref para);
};



#endif // plotsBase_h__

#ifndef plotsBase_h__
#define plotsBase_h__
#include "sct_plots.h"

#include "plot_save_option_def.h"
#include "s_plot_prob.h"
#include "s_plot_collection.h"
#include "Rtypes.h"
#include "s_DrawOption.h"
#include "s_plane.h"


namespace sct_corr{
  class plot{
  public:

    plot(const s_plot_prob& = "");
    virtual const char* getType() const = 0;
    virtual bool isReady() = 0;
    virtual void pushAxis(axis_ref* axis) = 0;
    virtual void pushPlane(S_plane* axis) = 0;
    virtual void fill() = 0;
    virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
    virtual Long64_t Draw(const S_DrawOption&) = 0;
    virtual void setParameter(const char* tag, const char * value);
    virtual s_plane_collection getOutputcollection();

    virtual const char* getOutputName()  const = 0;
    const char* getName() const;
    bool getSave2disk() const;
  private:
    s_plot_prob m_prob;
    mutable std::string m_name;
  };

}

#endif // plotsBase_h__

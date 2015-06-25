#ifndef plot2d_h__
#define plot2d_h__


#include "internal/plotsBase.hh"
#include "s_plane.h"
#include "sct_events/rootEvent_X_Y_hits.hh"

namespace sct_corr{
  class treeCollection_ouput;
  class axis_ref;
  class plot2d :public plot{

  public:
    plot2d(const s_plot_prob& = "");
    virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override;
    virtual Long64_t Draw(const S_DrawOption& opt) override;
    virtual void ProcessEvent() = 0;
    virtual void fill() override final;
    void pushHit(Double_t x, Double_t y);
    void pushHit(Double_t x, Double_t y, Double_t ID);

    virtual bool isReady();
    virtual void pushAxis(const axis_ref* axis);
    virtual void pushPlane(S_plane* axis);
    virtual const char* getOutputName()  const;
  protected:
    const axis_ref* m_x = nullptr;
    const axis_ref* m_y = nullptr;

    rootEvent_X_Y_hits m_outputEvent;

    std::shared_ptr<treeCollection_ouput> m_outTree;
    Int_t m_current = 0;
  };
}
#endif // plot2d_h__

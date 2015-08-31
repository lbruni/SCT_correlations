#ifndef inStripEfficiency_h__
#define inStripEfficiency_h__
#include <memory>

#include "S_Axis.h"
#include "Rtypes.h"
#include "internal/platform.hh"
#include "s_plot_prob.h"

class S_plane_def;
class S_Cut;

class TH1D;
class S_DrawOption;

namespace sct_corr {
class DllExport inStripEfficiency {
public:
  inStripEfficiency(
    const S_plane_def& trueHits,
    const S_plane_def& trueHits_with_dut,
    const S_Cut& cut_,
    axis_def search_axis,
    double mod_,
    const s_plot_prob& plot_prob
    );
  Long64_t Draw_true_hits(const S_DrawOption& d_option);

  Long64_t Draw_DUT_hits(const S_DrawOption& d_option);
  Long64_t Draw();
  Long64_t Draw(const S_DrawOption& d_option);


private:
  std::shared_ptr<S_plane_def> m_trueHits;
  std::shared_ptr<S_plane_def> m_DUT_hits;
  std::shared_ptr<TH1D> m_efficiency;
  double m_mod;
  axis_def m_search_axis;
  s_plot_prob m_plot_prob;
};
}
#endif // inStripEfficiency_h__

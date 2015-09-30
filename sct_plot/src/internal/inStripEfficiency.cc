#include "sct_processors.h"
#include "s_plot_prob.h"
#include "s_DrawOption.h"
#include "TH1.h"
#include "SCT_helpers.h"
#include "internal/inStripEfficiency.hh"


sct_corr::inStripEfficiency::inStripEfficiency(
  const sct_corr::plane_def& trueHits,
  const sct_corr::plane_def& trueHits_with_dut,
  const S_Cut& cut_, axis_def search_axis, 
  sct_type::modulo_t mod_,
  const s_plot_prob& plot_prob) 
  :inStripEfficiency(
      sct_processor::cut_x_y(trueHits, cut_, s_plot_prob().doNotSaveToDisk() ),
      sct_processor::cut_x_y(trueHits_with_dut, cut_, s_plot_prob().doNotSaveToDisk()),
      search_axis,
      mod_,
      plot_prob
  )
{

}

sct_corr::inStripEfficiency::inStripEfficiency(
  const sct_corr::plane_def& trueHits,
  const sct_corr::plane_def& trueHits_with_dut,
  axis_def search_axis, 
  sct_type::modulo_t mod_,
  const s_plot_prob& plot_prob
  ) 
  :m_mod(mod_), m_search_axis(search_axis), m_plot_prob(plot_prob)
{

  double mod_x = 10000000000;
  double mod_y = 10000000000;
  if (search_axis == x_axis_def) {
    mod_x = m_mod.value;
  }
  if (search_axis == y_axis_def) {
    mod_y = m_mod.value;
  }



  std::string true_hits_name = plot_prob.getName().value + "_true_hits";
  auto mod_total = sct_processor::moduloHitMap(
    trueHits,
    mod_x,
    mod_y,
    s_plot_prob(true_hits_name.c_str())
    .setSaveOptione(plot_prob.getPlotSaveOption())
    );

  m_trueHits = mod_total.copy();
  std::string dut_hits_name = plot_prob.getName().value + "_DUT_hits";
  auto mod_DUT = sct_processor::moduloHitMap(
    trueHits_with_dut,
    mod_x,
    mod_y,
    s_plot_prob(dut_hits_name.c_str())
    .setSaveOptione(plot_prob.getPlotSaveOption())
    );
  m_DUT_hits = mod_DUT.copy();
}

Long64_t sct_corr::inStripEfficiency::Draw_true_hits(const S_DrawOption& d_option) {
  return m_trueHits->Draw(d_option);
}

Long64_t sct_corr::inStripEfficiency::Draw_DUT_hits(const S_DrawOption& d_option) {
  return m_DUT_hits->Draw(d_option);
}

Long64_t sct_corr::inStripEfficiency::Draw() {
  return Draw(S_DrawOption());
}



TH1D* sct_corr::inStripEfficiency::getEfficiency_map() const {
  return m_efficiency.get();
}

Long64_t sct_corr::inStripEfficiency::Draw(const S_DrawOption& d_option) {
  TH1D trueHitsMod("mod_true", "true hits", 60, 0, m_mod.value);
  TH1D DUtHitsMod("mod_dut", "DUT hits", 60, 0, m_mod.value);
  Draw_true_hits(S_DrawOption().output_object(&trueHitsMod).draw_axis(m_search_axis));
  auto ret = Draw_DUT_hits(S_DrawOption().output_object(&DUtHitsMod).draw_axis(m_search_axis));


  m_efficiency = std::shared_ptr<TH1D>(dynamic_cast<TH1D*>(SCT_helpers::calc_efficiency(&trueHitsMod, &DUtHitsMod)));
  if (!m_efficiency) {
    return -1;
  }
  m_efficiency->Draw(d_option.getOptions());
  m_efficiency->SetTitle(m_plot_prob.getName().value.c_str());
  m_efficiency->GetXaxis()->SetTitle("instrip_pos");
  m_efficiency->GetYaxis()->SetTitle("Efficiency");
  //SCT_helpers::saveTH1_as_txt(*m_efficiency, "instripEffi.txt");
  return ret;
}

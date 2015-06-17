#include "sct_plots.h"
#include "TMath.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include <iostream>
#include "TProfile.h"
#include "TCanvas.h"

TH2* sct_analyis::misalignment(S_plot_collection& treeColl)
{

  auto apix_true_hits = treeColl.addPlot(sct_plot::find_nearest("", 0.1, 0.2), sct_coll::apix_fitted(), sct_coll::apix_hit()).get(1);

  auto dut_fitted_trackts = treeColl.addPlot(sct_plot::find_nearest("", 1, 1), sct_coll::DUT_fitted(), apix_true_hits);

  auto dut_fitted_trackts_cut = treeColl.addPlot(sct_plot::cut_x_y("", S_XCut(-2, 2.01) + S_YCut(-3, 4)), dut_fitted_trackts.get(1));



  auto res = treeColl.addPlot(sct_plot::find_nearest_strip("correlations", x_axis_def, 100), sct_coll::DUT_hit(), dut_fitted_trackts_cut());
  auto res1 = treeColl.addPlot(sct_plot::hitmap(""), res.get(2).getX_def(), res.get(0).getY_def());
  
  // auto cor2 = treeColl->addPlot(sct_plot::save2LCIO("","file.lcio", 1), cor.get(0));

  treeColl.loop();
  TH2D *h = new TH2D("asda", "dasdasd", 100, -2, 2, 100, -0.2, 0.2);
  treeColl.Draw(res1(),S_DrawOption().opt_colz().draw_y_VS_x().output_object(h).cut_y(-0.2,0.2));


  return h;
}

TF1* sct_analyis::Draw_misalignment(TFile& __file0)
{


  TFile* _file1 = new TFile("dummy.root", "recreate");
  S_plot_collection* pl = new S_plot_collection(&__file0);

  auto h = sct_analyis::misalignment(*pl);

  auto h1 = SCT_helpers::HistogrammSilhouette(h, y_axis_def);
  TF1* f2 = new TF1("f2", "gaus");
  h1->Fit(f2, "QN");
  std::cout << "gauss const = " << f2->GetParameter("Constant") << std::endl;
  SCT_helpers::CutTH2(h, S_Cut_BinContent(f2->GetParameter("Constant")*0.2));

  auto p = h->ProfileX();
  TF1* f = new TF1("f1", "pol1", p->GetXaxis()->GetBinCenter(0), p->GetXaxis()->GetBinCenter(p->GetXaxis()->GetBinCenter(p->GetNbinsX())));
  p->Fit(f, "QN");
  std::cout << "Offset: " << f->GetParameters()[0] << "   Slope  " << f->GetParameters()[1] << std::endl;
  new TCanvas();
  h->Draw("colz");
  p->Draw("same");
  f->Draw("same");
  return f;
}


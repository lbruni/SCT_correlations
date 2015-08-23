#include "sct_plots.h"
#include "TMath.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include <iostream>
#include "TProfile.h"
#include "TCanvas.h"
#include "s_plot_collection.h"
#include "s_DrawOption.h"
#include "s_cuts.h"
#include "s_plane.h"
TH2* sct_analyis::misalignment(S_plot_collection& treeColl)
{

  auto res1=sct_plot::misalignment_strip(treeColl, sct_coll::DUT_fitted(), sct_coll::DUT_hit(),x_axis_def);
  //auto res1 = sct_plot::misalignment_strip(treeColl, sct_coll::tel_fitted(1), sct_coll::tel_hit(1), x_axis_def);
  //auto res1 = sct_plot::misalignment_strip(treeColl, sct_coll::apix_fitted(), sct_coll::apix_hit(), x_axis_def);

  treeColl.loop();
  TH2D *h = new TH2D("asda", "dasdasd", 100, -10, 10, 100, -0.2, 0.2);
  
  treeColl.Draw(
    res1(),
    S_DrawOption()
    .opt_colz()
    .draw_y_VS_x()
    .output_object(h)
    .cut_y(-0.2,0.2)
    );

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


TF1* sct_analyis::Draw_New_aligment(TFile& __file0, TF1 *f)
{
  
  Double_t Slope = f->GetParameter("p1");
  Double_t offset = f->GetParameter("p0");
  TFile* _file1 = new TFile("dummy.root", "recreate");
  S_plot_collection* pl = new S_plot_collection(&__file0);




  auto apix_true_hits = sct_plot::find_nearest(
    *pl, 
    0.1, 
    0.2, 
    sct_coll::apix_fitted(), 
    sct_coll::apix_hit()
    ).getHitOnPlaneA();

  auto dut_fitted_trackts = sct_plot::find_nearest(
    *pl, 
    1, 
    1, 
    sct_coll::DUT_fitted(), 
    apix_true_hits
    ).getHitOnPlaneA();


  auto dut_rotated_17 = sct_plot::rotate(
    *pl, 
    Slope, 
    dut_fitted_trackts, 
    s_plot_prob().doNotSaveToDisk()
    );


  auto dut_rot_moved = sct_plot::coordinate_transform_move(
    *pl, 
    0, 
    offset, 
    dut_rotated_17, 
    s_plot_prob().doNotSaveToDisk()
    );


  auto res = sct_plot::find_nearest_strip(*pl, x_axis_def, 100, sct_coll::DUT_hit(), dut_rot_moved, s_plot_prob("correlations"));
  auto res1 = sct_plot::hitmap(*pl, res.get(2).getX_def(), res.get(0).getY_def());

  
  // auto cor2 = treeColl->addPlot(sct_plot::save2LCIO("","file.lcio", 1), cor.get(0));

  pl->loop();
  TH2D *h = new TH2D("asda", "dasdasd", 100, -10, 10, 100, -0.2, 0.2);
  pl->Draw(res1, S_DrawOption().opt_colz().draw_y_VS_x().output_object(h).cut_y(-0.2, 0.2));

  auto p = h->ProfileX();
  TF1* f1 = new TF1("f1", "pol1", p->GetXaxis()->GetBinCenter(0), p->GetXaxis()->GetBinCenter(p->GetXaxis()->GetBinCenter(p->GetNbinsX())));
  p->Fit(f1, "QN");
  //std::cout << "Offset: " << f->GetParameters()[0] << "   Slope  " << f->GetParameters()[1] << std::endl;

  
  new TCanvas();
  pl->Draw(res1, S_DrawOption().opt_colz().draw_y_VS_x().output_object(h).cut_y(-0.2, 0.2));
  return f1;

}
S_Cut* sct_analyis::Get_fiducial_area(TFile& __file0)
{
  TFile* _file1 = new TFile("dummy.root", "recreate");
  S_plot_collection* pl = new S_plot_collection(&__file0);
  return 0;
}
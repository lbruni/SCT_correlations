#include "TFile.h"
#include "TApplication.h"
#include "TBrowser.h"



#include "../sct_plot/inc/sct_plots.h"
#include <iostream>
#include "TCanvas.h"
#include "TMath.h"
#include "TH2.h"
#include <vector>





void residual_plane3(){

  TFile* _file0 = new TFile("C:/Users/Argg/OneDrive/alibava/new/fitter.root");
  TFile* _file1 = new TFile("test.root", "recreate");

  S_plot_collection* pl =new S_plot_collection(_file0);

  auto tel_rotated_17= pl->addPlot(sct_plot::s_rotated("", 0.017, false), sct_coll::DUT_fitted());
  auto tel_rotated_0 = pl->addPlot(sct_plot::s_rotated("", 0.0, false), sct_coll::DUT_fitted());

  s_plane_collection strip_no_rot = pl->addPlot(sct_plot::s_find_nearest_strip("",  y_axis_def), sct_coll::DUT_hit(), tel_rotated_0());
  s_plane_collection hitmap__no_rot = pl->addPlot(sct_plot::s_hitmap(""), strip_no_rot.get(0).getY_def(), strip_no_rot.get(2).getX_def());


  s_plane_collection strip_rot = pl->addPlot(sct_plot::s_find_nearest_strip("", y_axis_def), sct_coll::DUT_hit(), tel_rotated_17());
  s_plane_collection hitmap__rot = pl->addPlot(sct_plot::s_hitmap(""), strip_rot.get(0).getY_def(), strip_rot.get(2).getX_def());



  pl->loop();
  TH2D* h = new TH2D("h2", "h2", 100, -0.2, 0.2, 100, -5, 5);

  new TCanvas();
  std::cout << "FEI4 && DUT = " << pl->Draw(hitmap__no_rot(), S_DrawOption("colz", "x<0.2&&x>-0.2", "x")) << std::endl;


  new TCanvas();
  std::cout << "FEI4 && DUT = " << pl->Draw(hitmap__rot(), S_DrawOption("colz", "x<0.2&&x>-0.2", "x")) << std::endl;
}

void plotCorrelations(){

  TFile* _file0 = new TFile("C:/Users/Argg/OneDrive/alibava/new/fitter.root");
  TFile* _file1 = new TFile("test.root", "recreate");
  S_plot_collection* pl = new S_plot_collection(_file0);


  s_plane_collection corr = pl->addPlot(sct_plot::s_correlation(""), sct_coll::DUT_fitted().getY_def(), sct_coll::DUT_hit().getY_def());
  pl->loop();

  pl->Draw(corr(), S_DrawOption("colz", "", "y:x"));
}

int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);

  residual_plane3();

  

  theApp.Run();



  TFile* _file0 = new TFile("C:/Users/Argg/OneDrive/alibava/new/fitter.root");
  TFile* _file1 = new TFile("test.root","recreate");

 S_plot_collection pl(_file0);
 //pl.addFile(_file1);
 const char * strip_distance = "plot1";
 const char * name2 = "plot2";
 const char * strip_filtered = "plot3";
 const char * name4 = "plot4";
 const char* fei4 = "fei4";
 const char* strip = "strip";
 const char* strip_fit_rot = "strip_fit_rot";
 const char* fitted_vs_Original = "fitted_vs_Original";
 Int_t fei4_Plane_id = 20;
 Int_t Strip_Plane_id = 8;

 auto fei4_nearest=pl.addPlot(sct_plot::s_find_nearest(fei4,100,100) , sct_coll::apix_hit(), sct_coll::apix_fitted());

 //auto strip_rotated= pl.addPlot(sct_plot::s_rotated(strip_fit_rot, 0.017, false), sct_coll::DUT_fitted().getX_def(), sct_coll::DUT_fitted().getY_def());


 //pl.addPlot(sct::plot_find_nearest_strip(), strip, S_plane(sct::col_hit(), 8), S_plane(sct::col_fitpoints(), 8));
// pl.addPlot(sct::plot_find_nearest_strip(), strip, S_plane(sct::col_hit(), 8), S_plane(sct::col_fitpoints(), 8));
 auto strip_nearest = pl.addPlot(sct_plot::s_find_nearest_strip(strip, y_axis_def, 1000, true), sct_coll::DUT_hit(), sct_coll::DUT_fitted());
 auto hitmap__= pl.addPlot(sct::plot_hitmap(), name4, strip_nearest.get(0).getY_def(), strip_nearest.get(2).getX_def() );
 

 s_plane_collection plane2 = pl.addPlot(sct_plot::s_find_nearest("fe", 100, 100), sct_coll::tel_hit(3), sct_coll::tel_fitted(3));

 /*
 //pl.addPlot(sct::plot_find_correspondingX(), name, S_plane(sct::col_hit(), 8), S_plane(sct::col_fitpoints(), 20));

 pl.addPlot(sct::plot_plane_distance(), strip_distance, strip_nearest.get(0), fei4_nearest.get(0));

 pl.addPlot(sct::plot_A_if_B(), strip_filtered, S_plane(strip, 2), S_plane(strip_distance,0));

 pl.addPlot(sct::plot_correlation(), name2, S_Axis(strip_distance, 0, y_axis_def), S_Axis(strip_filtered, 0, x_axis_def));

 pl.addPlot(sct::plot_correlation(), fitted_vs_Original, S_Axis(strip_distance, 0, y_axis_def), S_Axis(strip_filtered, 0, x_axis_def));

 pl.addPlot(sct::plot_hitmap(), name4, S_Axis(strip_distance, 0, x_axis_def), S_Axis(strip_distance, 0, y_axis_def));
 auto projected_strip_rotated = pl.addPlot(sct_plot::s_rotated("rotatedprojected", -TMath::Pi() / 2 + 0.017), sct_coll::DUT_fitted().getX_def(), sct_coll::DUT_fitted().getY_def());
 
 
 
 auto fitted_in_channels = pl.addPlot(sct_plot::s_coordinate_transform("transorm", 13.4228187919463, 318.176 - 2 - 2.93916e-002 - 2.11225e-003, 1, 0), projected_strip_rotated().getX_def(), projected_strip_rotated().getY_def());

 
 
 
 
 
 auto projected =pl.addPlot(sct_plot::s_correlation("channels_fitted", 1), fitted_in_channels.get(0).getY_def(), sct_coll::DUT_zs_data().getX_def());

 auto projected1 = pl.addPlot(sct_plot::s_correlation("channels_fitted1", 1), fitted_in_channels().getX_def(), sct_coll::DUT_zs_data().getX_def());
 auto strip_clustered = pl.addPlot(sct_plot::s_clustering("strip_clustered"), sct_coll::DUT_zs_data().getX_def(), sct_coll::DUT_zs_data().getY_def());
 auto strip__projected_nearest = pl.addPlot(sct_plot::s_find_nearest_strip("strip_nearest", x_axis_def), fitted_in_channels(), sct_coll::DUT_zs_data());



 auto strip__projected_nearest_map = pl.addPlot(sct_plot::s_hitmap("strip_nearest_map"), strip__projected_nearest().getX_def(), strip__projected_nearest.get(1).getX_def());


 */
 //pl.addPlot(sct::plot_correlation(), "corr", S_Axis(sct::col_hit(), 20 ,y_axis_def), S_Axis(sct::col_fitpoints(), 20,y_axis_def));
#ifdef  _DEBUG

 pl.loop(1000);
#else

 pl.loop();
#endif //  _DEBUG

 //std::cout << "events = " << pl.Draw(fei4, S_DrawOption("colz", "", "y:x")) << std::endl;
 //pl.reset();
 //pl.addPlot(sct::plot_hitmap(), name2, S_Axis(name, 0, x_axis_def), S_Axis(name, 0, y_axis_def));
 //pl.loop(10000);

//  Int_t fei4AndDut, FEI4AndNotDUT;
// 
//  std::cout << "all events = " << pl.Draw(strip_distance, S_DrawOption("colz", "", "y:x")) << std::endl;
//  fei4AndDut = pl.Draw(strip_distance, S_DrawOption("", "ID==0", "y:x"));
//  std::cout << "FEI4 && DUT = " << fei4AndDut << std::endl;
//  FEI4AndNotDUT = pl.Draw(strip_distance, S_DrawOption("", "ID==1", "y:x"));
//  std::cout << "FEI4 && !DUT = " << FEI4AndNotDUT << std::endl;
// 
//  std::cout << "efficiency= " << (double)fei4AndDut / (fei4AndDut + FEI4AndNotDUT) * 100 << std::endl;
//  std::cout << "!FEI4 && DUT = " << pl.Draw(strip_distance, S_DrawOption("", "ID==10", "y:x")) << std::endl;
//  std::cout << "!FEI4 && DUT = " << pl.Draw(strip_distance, S_DrawOption("", "ID==11", "y:x")) << std::endl;
 TH2D* h = new TH2D("h2", "h2", 100, -0.2, 0.2, 100, -5, 5);
 std::cout << "FEI4 && DUT = " << pl.Draw(hitmap__(), S_DrawOption("colz", "x<0.2&&x>-0.2", "y:x>>h2")) << std::endl;


 TCanvas c;

 h->Draw("colz");
 

 //SCT_helpers::CutTH2(h,S_XCut(-0.1,0.1),S_YCut(0,2),  S_ZCut());
 SCT_helpers::CutTH2(h, S_ZCut(30));
 TCanvas c1;

 
 h->Draw("colz");
 new TBrowser();
   //pl.Draw();
 //TCanvas c;
// c.Divide(2, 1);
// c.cd(0);
// std::cout << "events = " << pl.Draw(name, S_DrawOption("colz", "", "y:x")) << std::endl;
 //c.cd(1);
 //std::cout << "events = " << pl.Draw("test", S_DrawOption("colz", "", "y:x")) << std::endl;
// std::cout << "events = " << pl.Draw("corr", S_DrawOption("colz", "", "y:x")) << std::endl;
//std::cout << "events = " << pl.Draw(name, S_DrawOption("colz", "x<0.1&&y<0.5", "y:x")) << std::endl;
 
 //new TBrowser();
 
  theApp.Run();
}


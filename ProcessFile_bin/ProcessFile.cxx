
#include "../sct_plot/inc/s_process_files.h"
#include "../sct_plot/inc/sct_runs.h"
#include "TError.h"

#include "TApplication.h"
#include "TBrowser.h"
#include "../sct_plot/inc/s_cuts.h"
#include "TF1.h"
#include "TProfile.h"


int main(int argc, char **argv) {

  TApplication theApp("App", &argc, argv);
  s_process_files p;

 
  gErrorIgnoreLevel = kError;  // ignoring root printouts (replace of histograms) 
  TFile * __file = new TFile("D:/data/DEVICE_1_ASIC_on_Position_5_150V/run000893-fitter.root");
  TFile * __file1 = new TFile("dummy.root","recreate");


  p.push_files(__file, 50, 893);
  p.setOutputName("test.root");
  p.process();

  
  p.DrawResidualVsMissingCordinate(-255, -240);
//   auto h2 = p.getResidualVsMissingCordinate();
// 
//   SCT_helpers::CutTH2(h2, S_Cut_BinContent(20));
//   h2->Draw("colz");
//   
//   
//   TF1 f = SCT_helpers::LinearFit_Of_Profile(h2);
// 
//   h2->Draw("colz");
//   f.Draw("same");
// 
//   std::cout << "f.GetParameter(\"p1\")  " << f.GetParameter("p1") << std::endl;
//   std::cout << "f.GetParameter(\"p0\")  " << f.GetParameter("p0") << std::endl;
  new TBrowser();
  theApp.Run();

}

 
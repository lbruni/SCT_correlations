
#include "local_hit_new.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
using namespace std;

void local_hit_new::Loop()
{
    Double_t x_0, y_0;
    Double_t x_1, y_1;
    Double_t x_2, y_2;
    Double_t x_3, y_3;
    Double_t x_4, y_4;
    Double_t x_5, y_5;
    Double_t x_8, y_8;
    
    std::vector<double> &id = *ID;
    std::vector<double> &x_pos = *x;
    std::vector<double> &y_pos = *y;
   
    if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
       for(size_t i=0;i<m_corr.size();++i){
           m_corr[i].fill(id,y_pos);
        
       }
      
   }
   /* TCanvas *c1 = new TCanvas("c1","c1", 1000, 800);
    c1->Divide(3,2);
    c1->cd(1);
    hx0x1->Draw("colz");
    c1->cd(2);
    hx0x2->Draw("colz");
    c1->cd(3);
    hx0x3->Draw("colz");
    c1->cd(4);
    hx0x4->Draw("colz");
    c1->cd(5);
    hx0x5->Draw("colz");
    c1->cd(6);
    hx0x8->Draw("colz");
    
    TCanvas *c2 = new TCanvas("c2","c2", 1000, 800);
    c2->Divide(3,2);
    c2->cd(1);
    hx1x2->Draw("colz");
    c2->cd(2);
    hx1x3->Draw("colz");
    c2->cd(3);
    hx1x4->Draw("colz");
    c2->cd(4);
    hx1x5->Draw("colz");
    c2->cd(5);
    hx1x8->Draw("colz");

    TCanvas *c3 = new TCanvas("c3","c3", 1000, 800);
    c3->Divide(2,2);
    c3->cd(1);
    hx2x3->Draw("colz");
    c3->cd(2);
    hx2x4->Draw("colz");
    c3->cd(3);
    hx2x5->Draw("colz");
    c3->cd(4);
    hx2x8->Draw("colz");
    
    
    TCanvas *c4 = new TCanvas("c4","c4", 1000, 800);
    c4->Divide(3,2);
    c4->cd(1);
    hx3x4->Draw("colz");
    c4->cd(2);
    hx3x5->Draw("colz");
    c4->cd(3);
    hx3x8->Draw("colz");
    c4->cd(4);
    hx4x5->Draw("colz");
    c4->cd(5);
    hx4x8->Draw("colz");
    c4->cd(6);
    hx5x8->Draw("colz");

    TCanvas *c5 = new TCanvas("c5","c5", 1000, 800);
    c5->Divide(3,2);
    c5->cd(1);
    hy0y1->Draw("colz");
    c5->cd(2);
    hy0y2->Draw("colz");
    c5->cd(3);
    hy0y3->Draw("colz");
    c5->cd(4);
    hy0y4->Draw("colz");
    c5->cd(5);
    hy0y5->Draw("colz");
    c5->cd(6);
    hy0y8->Draw("colz");
    
    TCanvas *c6 = new TCanvas("c6","c6", 1000, 800);
    c6->Divide(3,2);
    c6->cd(1);
    hy1y2->Draw("colz");
    c6->cd(2);
    hy1y3->Draw("colz");
    c6->cd(3);
    hy1y4->Draw("colz");
    c6->cd(4);
    hy1y5->Draw("colz");
    c6->cd(5);
    hy1y8->Draw("colz");
    
    TCanvas *c7 = new TCanvas("c7","c7", 1000, 800);
    c7->Divide(2,2);
    c7->cd(1);
    hy2y3->Draw("colz");
    c7->cd(2);
    hy2y4->Draw("colz");
    c7->cd(3);
    hy2y5->Draw("colz");
    c7->cd(4);
    hy2y8->Draw("colz");
    
    TCanvas *c8 = new TCanvas("c8","c8", 1000, 800);
    c8->Divide(3,2);
    c8->cd(1);
    hy3y4->Draw("colz");
    c8->cd(2);
    hy3y5->Draw("colz");
    c8->cd(3);
    hy3y8->Draw("colz");
    c8->cd(4);
    hy4y5->Draw("colz");
    c8->cd(5);
    hy4y8->Draw("colz");
    c8->cd(6);
    hy5y8->Draw("colz");
    
    c1->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker1.eps");
    c2->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker2.eps");
    c3->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker3.eps");
    c4->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker4.eps");
    c5->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker5.eps");
    c6->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker6.eps");
    c7->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker7.eps");
    c8->SaveAs("Hitmaker_correlation_plots/Correlation_hitmaker8.eps");
    */
}

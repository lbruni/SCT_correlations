 #include "TF1.h"
void fit_prova()
{
    gSystem->Load("libSCT_plot.dylib");
    
    TFile *histo1 = new TFile("output688.root");
     S_plot_collection * pl=new  S_plot_collection(histo1);
    TH2F *h1 = (TH2F*)histo1->Get("h2");
       //gStyle->SetOptFit();
    gStyle->SetOptStat(0);
        c1->Divide(1,2);
    c1->cd(1);
    

    
    
    
    
    h1->SetTitle("run 688");
    
   
    
    
    c1->cd(2);
    
    f1->SetLineColor(1);
    f1->Draw();
    
    
    
         c1->SaveAs("fitall.eps");
     c1->SaveAs("fitall.root");
    
    Double_t p1_1 = f1->GetParameter(1);
   
   
    
    
    TCanvas *call = new TCanvas("call","show profile",900,1200);
   
   
   
       h1->Draw("colz");
    prof1->Draw("same");
    f1->Draw("same");
    
    call->SaveAs("plot.eps");
    
    
    
    
}
   

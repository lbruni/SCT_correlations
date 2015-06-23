#include "TF1.h"
void fit2d()
{   
    TFile *histo1 = new TFile("output688.root");
    TFile *histo2 = new TFile("output689.root");
    TFile *histo3 = new TFile("output690.root");
    TFile *histo4 = new TFile("output691.root");
    TFile *histo5 = new TFile("output692.root");
    TFile *histo6 = new TFile("output693.root");
    TFile *histo7 = new TFile("output694.root");
    TFile *histo8 = new TFile("output703.root");

    TH2F *h1 = (TH2F*)histo1->Get("h2");
    TH2F *h2 = (TH2F*)histo2->Get("h2");
    TH2F *h3 = (TH2F*)histo3->Get("h2");
    TH2F *h4 = (TH2F*)histo4->Get("h2");
    TH2F *h5 = (TH2F*)histo5->Get("h2");
    TH2F *h6 = (TH2F*)histo6->Get("h2");
    TH2F *h7 = (TH2F*)histo7->Get("h2");
    TH2F *h8 = (TH2F*)histo8->Get("h2");
  
   
   
   //gStyle->SetOptFit();
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","show profile",600,900);
    c1->Divide(1,2);
    c1->cd(1);
    
    TProfile *prof1 = h1->ProfileX("_pfx1", 1, 100,"");
    TProfile *prof2 = h2->ProfileX("_pfx2", 1, 100,"");
    TProfile *prof3 = h3->ProfileX("_pfx3", 1, 100,"");
    TProfile *prof4 = h4->ProfileX("_pfx4", 1, 100,"");
    TProfile *prof5 = h5->ProfileX("_pfx5", 1, 100,"");
    TProfile *prof6 = h6->ProfileX("_pfx6", 1, 100,"");
    TProfile *prof7 = h7->ProfileX("_pfx7", 1, 100,"");
    TProfile *prof8 = h8->ProfileX("_pfx8", 1, 100,"");
    prof1->SetLineColor(1);
    prof2->SetLineColor(46);
    prof3->SetLineColor(30);
    prof4->SetLineColor(4);
    prof5->SetLineColor(16);
    prof6->SetLineColor(6);
    prof7->SetLineColor(7);
    prof8->SetLineColor(9);
    
    h1->SetTitle("run 688");
    h2->SetTitle("run 689");
    h3->SetTitle("run 690");
    h4->SetTitle("run 691");
    h5->SetTitle("run 692");
    h6->SetTitle("run 693");
    h7->SetTitle("run 694");
    h8->SetTitle("run 703");
   
    prof1->Draw();
    prof2->Draw("same");
    prof3->Draw("same");
    prof4->Draw("same");
    prof5->Draw("same");
    prof6->Draw("same");
    prof7->Draw("same");
    prof8->Draw("same");
    
    prof1->SetLineWidth(4);
    prof2->SetLineWidth(4);
    prof3->SetLineWidth(4);
    prof4->SetLineWidth(4);
    prof5->SetLineWidth(4);
    prof6->SetLineWidth(4);
    prof7->SetLineWidth(4);
    prof8->SetLineWidth(4);
    
    TLegend *leg = new TLegend(0.4,0.6,0.89,0.89);
    leg->AddEntry(prof1,"run 688","l");
    leg->AddEntry(prof2,"run 689","l");
    leg->AddEntry(prof3,"run 690","l");
    leg->AddEntry(prof4,"run 691","l");
    leg->AddEntry(prof5,"run 692","l");
    leg->AddEntry(prof6,"run 693","l");
    leg->AddEntry(prof7,"run 694","l");
    leg->AddEntry(prof8,"run 703","l");
    leg->Draw();
    
    c1->cd(2);
    prof1->Fit("pol1","0");
    prof2->Fit("pol1","0");
    prof3->Fit("pol1","0");
    prof4->Fit("pol1","0");
    prof5->Fit("pol1","0");
    prof6->Fit("pol1","0");
    prof7->Fit("pol1","0");
    prof8->Fit("pol1","0");

    TF1 *f1 =(TF1*)prof1->GetFunction("pol1");
    TF1 *f2 =(TF1*)prof2->GetFunction("pol1");
    TF1 *f3 =(TF1*)prof3->GetFunction("pol1");
    TF1 *f4 =(TF1*)prof4->GetFunction("pol1");
    TF1 *f5 =(TF1*)prof5->GetFunction("pol1");
    TF1 *f6 =(TF1*)prof6->GetFunction("pol1");
    TF1 *f7 =(TF1*)prof7->GetFunction("pol1");
    TF1 *f8 =(TF1*)prof8->GetFunction("pol1");
    
    
    f1->SetLineColor(1);
    f2->SetLineColor(46);
    f3->SetLineColor(30);
    f4->SetLineColor(4);
    f5->SetLineColor(16);
    f6->SetLineColor(6);
    f7->SetLineColor(7);
    f8->SetLineColor(9);
    
    
    f1->Draw();
    f2->Draw("same");
    f3->Draw("same");
    f4->Draw("same");
    f5->Draw("same");
    f6->Draw("same");
    f7->Draw("same");
    f8->Draw("same");
    
    
    TLegend *leg2 = new TLegend(0.4,0.6,0.89,0.89);
    leg2->AddEntry(f1,"run 688","l");
    leg2->AddEntry(f2,"run 689","l");
    leg2->AddEntry(f3,"run 690","l");
    leg2->AddEntry(f4,"run 691","l");
    leg2->AddEntry(f5,"run 692","l");
    leg2->AddEntry(f6,"run 693","l");
    leg2->AddEntry(f7,"run 694","l");
    leg2->AddEntry(f8,"run 703","l");
    leg2->Draw();
     c1->SaveAs("fitall.eps");
     c1->SaveAs("fitall.root");
    
    Double_t p1_1 = f1->GetParameter(1);
    Double_t p1_2 = f2->GetParameter(1);
    Double_t p1_3 = f3->GetParameter(1);
    Double_t p1_4 = f4->GetParameter(1);
    Double_t p1_5 = f5->GetParameter(1);
    Double_t p1_6 = f6->GetParameter(1);
    Double_t p1_7 = f7->GetParameter(1);
    Double_t p1_8 = f8->GetParameter(1);
    std::cout<<p1_8<<"  "<< f8->GetParameter(0)<<std::endl;
    
    Double_t run_number[8] ={688.0,689.0,690.0,691.0,692.0,693.0,694.0,703.0};

    Double_t p1[8];
    Int_t n =8 ;
    
    p1[0] = p1_1;
    p1[1] = p1_2;
    p1[2] = p1_3;
    p1[3] = p1_4;
    p1[4] = p1_5;
    p1[5] = p1_6;
    p1[6] = p1_7;
    p1[7] = p1_8;
        for(int i =0; i<8; i++){
    std::cout<< "run num"<<run_number[i]<<"   p1: "<<p1[i]<<std::endl;
        
}
    TGraph *g = new TGraph(n,run_number,p1);
        TCanvas *c3 = new TCanvas("c3","",600,400);
    g->SetTitle("p1 vs run number");
    g->Draw("AC*");
    c3->SaveAs("p1.eps");
    
    TCanvas *call = new TCanvas("call","show profile",900,1200);
   
   
    call->Divide(2,4);
    call->cd(1);
    h1->Draw("colz");
    prof1->Draw("same");
    f1->Draw("same");
    call->cd(2);
    h2->Draw("colz");
    prof2->Draw("same");
    f2->Draw("same");
    call->cd(3);
    h3->Draw("colz");
    prof3->Draw("same");
    f3->Draw("same");
    call->cd(4);
    h4->Draw("colz");
    prof4->Draw("same");
    f4->Draw("same");
    call->cd(5);
    h5->Draw("colz");
    prof5->Draw("same");
    f5->Draw("same");
    call->cd(6);
    h6->Draw("colz");
    prof6->Draw("same");
    f6->Draw("same");
    call->cd(7);
    h7->Draw("colz");
    prof7->Draw("same");
    f7->Draw("same");
    call->cd(8);
    h8->Draw("colz");
    prof8->Draw("same");
    f8->Draw("same");
    
    call->SaveAs("plot.eps");
    
}
   

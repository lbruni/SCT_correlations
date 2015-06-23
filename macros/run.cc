
void run(){
    gSystem->Load("libSCT_plot.dylib");
    const char* inFileName="/Users/lucreziastellabruni/fitter703.root";
    TFile* _file0 = new TFile(inFileName);
    TFile* _file1 = new TFile("output703.root","RECREATE");
    
    S_plot_collection * pl=new  S_plot_collection(_file0);
    const char* name4 = "plot4";
    const char* strip = "strip";
    
    s_plane_collection strip_nearest = pl->addPlot(sct_plot::s_find_nearest_strip(strip, y_axis_def, 1000, true), sct_coll::DUT_hit(), sct_coll::DUT_fitted());
    s_plane_collection hitmap__= pl->addPlot(sct_plot::s_cut_x_y("",S_XCut(-2.8,5.2)+S_YCut(-3.6,4.2)),strip_nearest.get(2).getX_def(),strip_nearest.get(0).getY_def()  );
    pl->loop();
    
    TCanvas *c1 = new TCanvas("c1","c1",600,900);
    c1->Divide(1,2);
    
    TH1D* h1 = new TH1D("h1", "h1", 100, -0.2, 0.2);
    TH2D* h2 = new TH2D("h2", "h2", 100,-5, 6, 100, -0.2, 0.2);
    c1->cd(2);
    
    pl->Draw(hitmap__(), S_DrawOption("colz", "", "y:x>>h2"));
    c1->cd(1);
    auto h3 = SCT_helpers::HistogrammSilhouette(h2, y_axis_def);
    h3->Fit("gaus");
    TF1 *fit1 = (TF1*)h3->GetFunction("gaus");
    
    Double_t max, Z_cut;
    max = fit1->GetParameter(0) ;
    Z_cut=max*0.2;
    std::cout<<" max: "<<max<<"  Z_cut: "<<Z_cut<<std::endl;
    SCT_helpers::CutTH2(h2, S_Cut_BinContent(Z_cut));
    
    h3->Write();
    h2->Write();
    
    
   }








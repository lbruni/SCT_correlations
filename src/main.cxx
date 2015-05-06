#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TApplication.h"
#include "TH2.h"
int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);
  TFile* _file0 = new TFile("C:/Users/Argg/OneDrive/alibava/ROOT/missing.root");
  TTree* tree = (TTree*)_file0->Get("hit_local_new");

  tree->Draw("ID", "", "bar");

  theApp.Run();
}


class correlations{


  correlations(const char* name){

    //h2 = new TH2D(name, name, 1000, -10., 10.);
  }

  void fill(const std::vector<double>& m_id, const std::vector<double>& m_pos, int id){
    double plane_1, plane_2;


    h2->Fill(plane_1, plane_2);

  }

  TH2D* h2 = nullptr;
};
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TApplication.h"
#include "TH2.h"

#include "local_hit_new.h"


int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);
  TFile* _file0 = new TFile("C:/Users/Argg/OneDrive/alibava/ROOT/missing.root");
  TTree* tree = (TTree*)_file0->Get("hit_local_new");

  tree->Draw("ID", "", "bar");

  theApp.Run();
}


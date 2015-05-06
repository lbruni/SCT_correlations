//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May  5 14:51:29 2015 by ROOT version 6.02/05
// from TTree local_hit_new/local_hit_new
// found on file: missing.root
//////////////////////////////////////////////////////////

#ifndef local_hit_new_h
#define local_hit_new_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "vector"

class correlations{
public:
    correlations(const char* name, int identifier1,int identifier2):m_name(name),m_plane_number_1(identifier1),m_plane_number_2(identifier2){
        h = new TH2D(name,name, 50, -10, 10, 50, -10, 10);
    }
    void fill(const std::vector<double> &id, const std::vector<double> &pos)
    {
        Double_t pos_plane1, pos_plane2;
        
        for(std::vector<int>::size_type i = 0; i != id.size(); i++) {
            for(std::vector<int>::size_type j = 0; j != id.size(); j++) {
                
                if(id.at(i) == m_plane_number_1 && id.at(j) ==  m_plane_number_2){
                    pos_plane1 = pos.at(i); pos_plane2 = pos.at(j);
                }
            }
        }
        
        h->Fill(pos_plane1,pos_plane2);
    }
    void Draw(const char* options){
        h->Draw(options);
    }
    TH2D *h; 
    int m_plane_number_1, m_plane_number_2;
    std::string m_name;
    
};


class local_hit_new {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   std::vector<double>  *ID;
   std::vector<double>  *x;
   std::vector<double>  *y;
   Int_t           event_nr;

   // List of branches
   TBranch        *b_ID;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_event_nr;   //!

   local_hit_new(TTree *tree=0);
   virtual ~local_hit_new();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void End();
    virtual void run();
    std::vector<correlations> m_corr;
};

local_hit_new::local_hit_new(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("missing.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("missing.root");
      }
      f->GetObject("local_hit_new",tree);

   }
   Init(tree);
}

local_hit_new::~local_hit_new()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t local_hit_new::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t local_hit_new::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void local_hit_new::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

    m_corr.emplace_back("name",1,2);
   // Set object pointer
   ID = 0;
   x = 0;
   y = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ID", &ID, &b_ID);
   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("event_nr", &event_nr, &b_event_nr);
   Notify();
}

void local_hit_new::End(){
 TCanvas *c1 = new TCanvas("c1","c1", 1000, 800);
    for (int i=0;i<m_corr.size();++i){
        
        m_corr.at(i).Draw("colz");
        c1->SaveAs(m_corr.at(i).m_name.c_str());
    }
}

Bool_t local_hit_new::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void local_hit_new::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t local_hit_new::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


void local_hit_new::run(){
    local_hit_new t;
   // t.Init();
    t.Loop();
    t.End();
}
#endif // #ifdef local_hit_new_cxx

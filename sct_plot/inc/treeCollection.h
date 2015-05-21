//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 20 16:34:27 2015 by ROOT version 5.34/18
// from TTree fitpoints/fitpoints
// found on file: C:/Users/Argg/OneDrive/alibava/fitter.root
//////////////////////////////////////////////////////////

#ifndef fitpoints_h
#define fitpoints_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

#ifdef _DEBUG
class Hit_extractor;
#endif // _DEBUG



class treeCollection {
public :


   // Declaration of leaf types
  std::vector<double>  *ID;
  std::vector<double>  *x;
  std::vector<double>  *y;
   Int_t           event_nr;


   treeCollection(TTree *tree);
   virtual ~treeCollection();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Int_t    GetEntries() const;

private:
#ifdef _DEBUG
  Hit_extractor *m_tree;
#else //release
   // List of branches
   TBranch        *b_ID;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_event_nr;   //!
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
#endif // _DEBUG

  
};

#endif




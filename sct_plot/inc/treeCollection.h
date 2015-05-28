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
#include "Rtypes.h"


bool isClobalCollection(const char* name);
#ifdef _DEBUG
class Hit_extractor;
class Hit_output;
#endif // _DEBUG



class treeCollection {
public :


   // Declaration of leaf types
  std::vector<double>  *ID;
  std::vector<double>  *x;
  std::vector<double>  *y;
   Int_t                event_nr;
   Int_t                *event_nr_pointer;

   treeCollection(TTree *tree);
   treeCollection(const char *name);
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



class treeCollection_ouput {
public:

  treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr);
  virtual ~treeCollection_ouput();
  void fill();
  Int_t Draw(const char* axis, const char* cuts, const char * options);
private:
#ifdef _DEBUG
  Hit_output *m_tree;
  std::vector<double>  *m_ID;
  std::vector<double>  *m_x;
  std::vector<double>  *m_y;
  Int_t*                m_event_nr;
#else //release
 
 
  TTree          *m_tree;   //!pointer to the analyzed TTree or TChain
 
#endif // _DEBUG


};

#endif




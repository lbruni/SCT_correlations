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
#include "sct_event_buffer.h"



#ifdef _DEBUG
class Hit_extractor;
class Hit_output;
#endif // _DEBUG



class treeCollection {
public :

  root_event m_buffer;
   // Declaration of leaf types
   Int_t                event_nr =0;

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

  treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr,bool save2disk=true);
  virtual ~treeCollection_ouput();
  void fill();
  Int_t Draw(const char* axis, const char* cuts, const char * options);

  root_event m_buffer;
#ifdef _DEBUG
  Hit_output *m_tree=nullptr;
#else //release
 
 
  TTree          *m_tree=nullptr;   //!pointer to the analyzed TTree or TChain
 
#endif // _DEBUG

  std::string m_name;
};

#endif




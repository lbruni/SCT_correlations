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
#include "sct_events\rootEventBase.hh"
#ifdef _DEBUG
  class Hit_extractor;
  class Hit_output;
#endif // _DEBUG

namespace sct_corr{




  class treeCollection {
  public:


    rootEventBase m_rootBuffer;
    // Declaration of leaf types
   

    treeCollection(TTree *tree);
    treeCollection(const char *name);
    virtual ~treeCollection();
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Int_t    GetEntries() const;
    const char* getName() const;
  private:

    TTree* fChain = NULL;


    std::string m_name;
  };



  class treeCollection_ouput {
  public:

    treeCollection_ouput(const rootEventBase& ev, bool save2disk);
    virtual ~treeCollection_ouput();
    void fill();
    Int_t Draw(const char* axis, const char* cuts, const char * options);
    TTree * getTTree();

    rootEventBase m_rootBuffer;



    TTree          *m_tree=nullptr;   //!pointer to the analyzed TTree or TChain



    std::string m_name;
  };
}
#endif




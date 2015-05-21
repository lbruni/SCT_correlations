#include "treeCollection.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

#ifdef _DEBUG

#include "TTreeLoader.h"




treeCollection::~treeCollection()
{
  if (!m_tree) return;
  delete m_tree;
}



treeCollection::treeCollection(TTree *tree/*=0*/) :m_tree(new Hit_extractor(tree))
{
  x = new std::vector<double>();
  y = new std::vector<double>();
  ID = new std::vector<double>();
  event_nr = 0;
}

Int_t treeCollection::GetEntry(Long64_t entry)
{
  m_tree->GetEvent(entry);
  x->clear();
  y->clear();
  ID->clear();
  event_nr = 0;


  for (Int_t i = 0; i < m_tree->GetNumberOfEntries(); ++i)
  {
    m_tree->getEntry(i);
    x->push_back(m_tree->getX());
    y->push_back(m_tree->getY());
    ID->push_back(m_tree->getID());
    
  }
  event_nr = m_tree->getEventNr();
  return m_tree->GetNumberOfEntries();
}

Int_t treeCollection::GetEntries() const 
{
  return m_tree->GetNumberOfEvents();
}




#else







treeCollection::treeCollection(TTree *tree) : fChain(0)
{
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
}
treeCollection::~treeCollection()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t treeCollection::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Int_t treeCollection::GetEntries() const 
{
  return fChain->GetEntries();
}

#endif // _DEBUG
#include "treeCollection.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

#include "sct_global.h"

#ifdef _DEBUG

#include "TTreeLoader.h"




treeCollection::~treeCollection()
{

  delete m_buffer.m_ID;
  delete m_buffer.m_x;
  delete m_buffer.m_y;
  if (!m_tree) return;
  delete m_tree;
}



treeCollection::treeCollection(TTree *tree/*=0*/) :m_buffer(new std::vector<double>(), new std::vector<double>(), new std::vector<double>(),&event_nr)
{
  std::cout << tree->GetName() << std::endl;
  m_tree = new Hit_extractor(tree);
  event_nr = 0;
  getGlobalPlotCollection()->set(tree->GetName(), &m_buffer);
}

treeCollection::treeCollection(const char *name)
{
  m_tree = NULL;
  if (!getGlobalPlotCollection()->get(name, &m_buffer))
  {
    std::cout << "collection not found. name: \"" << name << "\"" << std::endl;
  }
  
  event_nr = *(m_buffer.m_event_nr);
}

Int_t treeCollection::GetEntry(Long64_t entry)
{
  if (m_tree==NULL)
  {
    event_nr = *(m_buffer.m_event_nr);
    return 0;
  }
  m_tree->GetEvent(entry);
  m_buffer.m_x->clear();
  m_buffer.m_y->clear();
  m_buffer.m_ID->clear();
  event_nr = 0;


  for (Int_t i = 0; i < m_tree->GetNumberOfEntries(); ++i)
  {
    m_tree->getEntry(i);
    m_buffer.m_x->push_back(m_tree->getX());
    m_buffer.m_y->push_back(m_tree->getY());
    m_buffer.m_ID->push_back(m_tree->getID());
    
  }
  event_nr = m_tree->getEventNr();
  return m_tree->GetNumberOfEntries();
}

Int_t treeCollection::GetEntries() const 
{
  if (m_tree==NULL)
  {
    return kMaxInt;
  }
  return m_tree->GetNumberOfEvents();
}




#else


treeCollection::treeCollection(const char *name)
{
  fChain = NULL;
  if (!getGlobalPlotCollection()->get(name, &m_buffer))
  {
    std::cout << "collection not found. name: \"" << name << "\"" << std::endl;
  }

  event_nr = *m_buffer.m_event_nr;
}





treeCollection::treeCollection(TTree *tree) : fChain(0)
{

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("ID", &m_buffer.m_ID, &b_ID);
  fChain->SetBranchAddress("x", &m_buffer.m_x, &b_x);
  fChain->SetBranchAddress("y", &m_buffer.m_y, &b_y);
  fChain->SetBranchAddress("event_nr", &event_nr, &b_event_nr);
  m_buffer.m_event_nr=&event_nr;
  getGlobalPlotCollection()->set(tree->GetName(), &m_buffer);
}
treeCollection::~treeCollection()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t treeCollection::GetEntry(Long64_t entry)
{
  if (fChain== NULL)
  {
    event_nr = *m_buffer.m_event_nr;
    return 0;
  }
  return fChain->GetEntry(entry);
}

Int_t treeCollection::GetEntries() const 
{
  if (fChain == NULL)
  {
    return kMaxInt;
  }
  return fChain->GetEntries();
}

#endif // _DEBUG











#ifdef _DEBUG



treeCollection_ouput::treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr) :
m_tree(new Hit_output(name)),m_buffer(ID,x,y,event_nr)
{
  getGlobalPlotCollection()->set(name, &m_buffer);
}

treeCollection_ouput::~treeCollection_ouput()
{
  delete m_tree;
}

void treeCollection_ouput::fill()
{
  for (size_t i = 0; i < m_buffer.m_x->size();++i)
  {
    m_tree->set(m_buffer.m_x->at(i), m_buffer.m_y->at(i), m_buffer.m_ID->at(i));
  }
  m_tree->setEventNR(*m_buffer.m_event_nr);
  m_tree->fill();
}

Int_t treeCollection_ouput::Draw(const char* axis, const char* cuts, const char * options)
{
 return m_tree->Draw(axis, cuts, options);
}

#else 


treeCollection_ouput::treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr) :m_buffer(ID,x,y,event_nr)
{
 
  getGlobalPlotCollection()->set(name ,&m_buffer);
  m_tree = new TTree(name, name);
  
  
  m_tree->Branch("ID",ID);
  m_tree->Branch("x", x);
  m_tree->Branch("y", y);
  m_tree->Branch("event_nr", event_nr);
}

treeCollection_ouput::~treeCollection_ouput()
{
 
}

void treeCollection_ouput::fill()
{
  m_tree->Fill();
}

Int_t treeCollection_ouput::Draw(const char* axis, const char* cuts, const char * options)
{
  return m_tree->Draw(axis, cuts, options);
}


#endif // _DEBUG

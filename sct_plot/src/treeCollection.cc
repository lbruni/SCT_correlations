#include "treeCollection.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>


class root_event{
public:
  std::vector<double>  *m_ID;
  std::vector<double>  *m_x;
  std::vector<double>  *m_y;
  Int_t*                m_event_nr;
};
std::map<std::string, root_event> gEvents;
void push2global(const char * name, treeCollection* tree){
  gEvents[name].m_ID = tree->ID;
  gEvents[name].m_x = tree->x;
  gEvents[name].m_y = tree->y;
  gEvents[name].m_event_nr = &tree->event_nr;
}

bool getFromGlobal(const char* name, treeCollection* tree){
  
  if (!isClobalCollection(name))
  {
    return false;
  }

  tree->ID = gEvents[name].m_ID;
  tree->x= gEvents[name].m_x;
  tree->y=gEvents[name].m_y;
  tree->event_nr_pointer = gEvents[name].m_event_nr;
  
  return true;
}

bool isClobalCollection(const char* name)
{
  auto it = gEvents.find(name);
  if (it == gEvents.end())
  {
    return false;
  }
  return true;
}

#ifdef _DEBUG

#include "TTreeLoader.h"




treeCollection::~treeCollection()
{
  if (!m_tree) return;
  delete m_tree;
}



treeCollection::treeCollection(TTree *tree/*=0*/) 
{
  std::cout << tree->GetName() << std::endl;
  m_tree = new Hit_extractor(tree);
  x = new std::vector<double>();
  y = new std::vector<double>();
  ID = new std::vector<double>();
  event_nr = 0;
  event_nr_pointer = &event_nr;
  push2global(tree->GetName(), this);
}

treeCollection::treeCollection(const char *name)
{
  m_tree = NULL;
  if (!getFromGlobal(name, this))
  {
    std::cout << "collection not found. name: \"" << name << "\"" << std::endl;
  }
  
  event_nr = *event_nr_pointer;
}

Int_t treeCollection::GetEntry(Long64_t entry)
{
  if (m_tree==NULL)
  {
    event_nr = *event_nr_pointer;
    return 0;
  }
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
  if (m_tree==NULL)
  {
    return 1000000000000;
  }
  return m_tree->GetNumberOfEvents();
}




#else


treeCollection::treeCollection(const char *name)
{
  fChain = NULL;
  if (!getFromGlobal(name, this))
  {
    std::cout << "collection not found. name: \"" << name << "\"" << std::endl;
  }

  event_nr = *event_nr_pointer;
}





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
  event_nr_pointer=&event_nr;
  push2global(tree->GetName(), this);
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
    event_nr = *event_nr_pointer;
    return 0;
  }
  return fChain->GetEntry(entry);
}

Int_t treeCollection::GetEntries() const 
{
  if (fChain == NULL)
  {
    return 1000000000000;
  }
  return fChain->GetEntries();
}

#endif // _DEBUG











#ifdef _DEBUG



treeCollection_ouput::treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr) :
m_tree(new Hit_output(name)), m_x(x), m_y(y), m_ID(ID), m_event_nr(event_nr)
{
  gEvents[name].m_event_nr = event_nr;
  gEvents[name].m_ID = ID;
  gEvents[name].m_x = x;
  gEvents[name].m_y = y;
}

treeCollection_ouput::~treeCollection_ouput()
{
  delete m_tree;
}

void treeCollection_ouput::fill()
{
  for (size_t i = 0; i < m_x->size();++i)
  {
    m_tree->set(m_x->at(i), m_y->at(i), m_ID->at(i));
  }
  m_tree->setEventNR(*m_event_nr);
  m_tree->fill();
}

Int_t treeCollection_ouput::Draw(const char* axis, const char* cuts, const char * options)
{
 return m_tree->Draw(axis, cuts, options);
}

#else 


treeCollection_ouput::treeCollection_ouput(const char * name, std::vector<double>* x, std::vector<double>* y, std::vector<double>* ID, Int_t * event_nr) 
{
 
  gEvents[name].m_event_nr = event_nr;
  gEvents[name].m_ID = ID;
  gEvents[name].m_x = x;
  gEvents[name].m_y = y;

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

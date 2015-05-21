
#include "TTreeLoader.h"
#include "fitpoints.h"
#include <iostream>

Hit_extractor::Hit_extractor(TTree* tree) :m_hits(new fitpoints(tree))
{

}

Hit_extractor::~Hit_extractor()
{
  delete m_hits;
}

double Hit_extractor::getX()
{
  return m_hits->x->at(m_entry);
}

double Hit_extractor::getY()
{
  return m_hits->y->at(m_entry);
}

Int_t Hit_extractor::getEventNr()
{
  return m_hits->event_nr;
}

double Hit_extractor::getID()
{
  return m_hits->ID->at(m_entry);
}

void Hit_extractor::getEntry(Long64_t entry)
{
  m_entry = entry;
}

Int_t Hit_extractor::GetNumberOfEntries()
{
  Int_t ret = m_hits->x->size();
  
  if (ret != m_hits->y->size())
  {
    std::cout << "[Hit_extractor] vectors dont have the same size " << std::endl;
    return -1;
  }

  if (ret!= m_hits->ID->size())
  {
    std::cout << "[Hit_extractor] vectors dont have the same size " << std::endl;
    return -1;
  }
  return ret;
}

Int_t Hit_extractor::GetEvent(Long64_t eventNr)
{
  
  auto ret=m_hits->GetEntry(eventNr);
  getEntry(0);
  return ret;
}

Int_t Hit_extractor::GetNumberOfEvents()
{

  return m_hits->GetEntries();
}


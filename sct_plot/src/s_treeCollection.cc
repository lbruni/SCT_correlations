#include "sct_plots.h"
#include "treeCollection.h"
#include <iostream>
#include "TCanvas.h"







S_treeCollection::S_treeCollection(TTree *tree) :m_tree(new sct_corr::treeCollection(tree))
{

}


Int_t S_treeCollection::GetEntry(Long64_t entry)
{
  if (!m_tree)
  {
    std::cout << "[S_treeCollection] tree not set " << std::endl;
    return -1;
  }
  return m_tree->GetEntry(entry);
}

Int_t S_treeCollection::GetEntries() const
{
  if (!m_tree)
  {
    std::cout << "[S_treeCollection] tree not set " << std::endl;
    return -1;
  }
  return m_tree->GetEntries();
}
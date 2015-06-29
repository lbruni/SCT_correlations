#ifndef s_treeCollection_h__
#define s_treeCollection_h__
#include "internal/platform.hh"
#include <memory>
#include "Rtypes.h"

class TTree;
namespace sct_corr{
  class treeCollection;
}
class DllExport S_treeCollection{
public:
  S_treeCollection(TTree *tree);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Int_t    GetEntries() const;
#ifndef __CINT__
  std::shared_ptr<sct_corr::treeCollection> m_tree;
#endif // !__CINT__
  ClassDef(S_treeCollection, 0);
};


#ifdef __CINT__
#pragma link C++ class S_treeCollection;
#endif // __CINT__

#endif // s_treeCollection_h__

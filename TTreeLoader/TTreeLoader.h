#ifndef TTreeLoader_h__
#define TTreeLoader_h__

#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#include "TTree.h"
class fitpoints;
class DllExport Hit_extractor{
public:
  

  Hit_extractor(TTree* tree);
  virtual ~Hit_extractor();


  double getX();
  double getY();
  Int_t getEventNr();
  double getID();

  void getEntry(Long64_t entry);
  Int_t GetNumberOfEntries();
  Int_t GetEvent(Long64_t eventNr);
  Int_t GetNumberOfEvents();

private:
  fitpoints *m_hits;
  Int_t m_entry=0;
};

#endif // TTreeLoader_h__

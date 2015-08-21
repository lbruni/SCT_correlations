#include "treeCollection.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "TTree.h"
#include "sct_global.h"


namespace sct_corr{
  const char* treeCollection::getName() const
  {
    if (fChain)
    {
      return fChain->GetName();
    }
    return m_name.c_str();
  }


  treeCollection::treeCollection(const char *name, sct_event_buffer* outputBuffer) :m_name(name)
  {
    fChain = NULL;
    auto buffer = outputBuffer;
    if (!buffer)
    {
      std::cout << "global buffer not set" << std::endl;
      return;

    }


    if (!buffer->get(name, &m_rootBuffer))
    {
      std::cout << "collection not found. name: \"" << name << "\"" << std::endl;
    }


  }





  treeCollection::treeCollection(TTree *tree) : fChain(0), m_rootBuffer(tree)
  {

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fChain->SetMakeClass(1);


   

    //buffer->set(tree->GetName(), &m_buffer);
  }
  treeCollection::~treeCollection()
  {
    if (!fChain) return;
    delete fChain->GetCurrentFile();
  }

  Int_t treeCollection::GetEntry(Long64_t entry)
  {
    if (fChain == NULL)
    {


      return 0;
    }
    auto ret = fChain->GetEntry(entry);
#ifdef _DEBUG
    m_rootBuffer.loadFromVector();
#endif // _DEBUG

    return ret;
  }

  Int_t treeCollection::GetEntries() const
  {
    Int_t max_entry = kMaxInt;
    if (fChain == NULL)
    {
      return kMaxInt;
    }

    auto entries = fChain->GetListOfBranches()->GetEntries();

    for (int i = 0; i < entries; i++) {
      auto br = dynamic_cast<TBranch*>(fChain->GetListOfBranches()->At(i));
      if (!br) {
        continue;
      }
      auto className = br->GetClassName();
      if (strcmp("vector<double>", className) != 0) {
        continue;
      }
      Int_t current_max = br->GetEntries();
      max_entry = TMath::Min(max_entry, current_max);
    }
    Int_t current_max = fChain->GetEntries();
    max_entry = TMath::Min(max_entry, current_max);
    return max_entry;

  }
}






namespace sct_corr{
  treeCollection_ouput::treeCollection_ouput(rootEventBase& ev, sct_event_buffer* outputBuffer,bool save2disk) :m_rootBuffer(ev), m_name(ev.getName())
  {


  
    if (!outputBuffer)
    {
      std::cout << "global buffer not set" << std::endl;
      return;

    }

    outputBuffer->set(m_rootBuffer.getName(), &m_rootBuffer);
    if (save2disk)
    {

      m_tree = new TTree(m_name.c_str(), m_name.c_str());
      if (outputBuffer->getOutputFile())
      {
        m_tree->SetDirectory(outputBuffer->getOutputFile()->GetDirectory("/"));
      }
      ev.Save2Tree(m_tree);
    }



  }

  treeCollection_ouput::~treeCollection_ouput()
  {

  }

  void treeCollection_ouput::fill()
  {

    if (m_tree)
    {
#ifdef _DEBUG
      m_rootBuffer.PushToVector();
#endif // _DEBUG
 
      m_tree->Fill();
    }
  }

  Int_t treeCollection_ouput::Draw(const char* axis, const char* cuts, const char * options)
  {
    if (!m_tree)
    {
      std::cout << "collection not stored: \"" << m_name << "\"" << std::endl;
      return -1; // nothing to do 
    }
    return m_tree->Draw(axis, cuts, options);
  }
  TTree * treeCollection_ouput::getTTree()
  {
    return m_tree;
  }
}


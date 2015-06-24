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


  treeCollection::treeCollection(const char *name) :m_name(name)
  {
    fChain = NULL;
    auto buffer = Buffer_accessor::getGlobalPlotCollection();
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


    auto buffer = Buffer_accessor::getGlobalPlotCollection();
    if (!buffer)
    {
      std::cout << "global buffer not set" << std::endl;
      return;

    }

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
    if (fChain == NULL)
    {
      return kMaxInt;
    }
    return fChain->GetEntries();
  }
}













namespace sct_corr{
  treeCollection_ouput::treeCollection_ouput(const rootEventBase& ev,bool save2disk) :m_rootBuffer(ev),m_name(ev.getName())
  {


    auto buffer = Buffer_accessor::getGlobalPlotCollection();
    if (!buffer)
    {
      std::cout << "global buffer not set" << std::endl;
      return;

    }

    buffer->set(m_rootBuffer.getName(), &m_rootBuffer);
    if (save2disk)
    {
      m_tree = new TTree(m_name.c_str(), m_name.c_str());
      m_rootBuffer.Save2Tree(m_tree);
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


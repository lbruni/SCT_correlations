#include "sct_events/rootEventBase.hh"
#include "TTree.h"
#include <iostream>
#include "S_Axis.h"
#include "internal/plane.hh"


namespace sct_corr{




#ifdef _DEBUG   
  TTreeVectorExtractor::TTreeVectorExtractor(const char* name, TTree* tree) :
    m_vecRel(std::make_shared<ReleaseVectorDef>(name)),
    m_name(name)
   
  {


    m_vecRel->load_from_TTree(tree);
    m_owend_vector= std::make_shared<std::vector<double> >();
    m_vec = m_owend_vector.get();
  }


  TTreeVectorExtractor::TTreeVectorExtractor(const char* name) :m_vecRel(std::make_shared<ReleaseVectorDef>(name)), m_name(name)
  {
    m_owend_vector = std::make_shared<std::vector<double> >();
    m_vec = m_owend_vector.get();
  }

  TTreeVectorExtractor::TTreeVectorExtractor() 
  {

  }

  TTreeVectorExtractor::~TTreeVectorExtractor()
  {

    
  }
  bool TTreeVectorExtractor::push2TTree(TTree* tree)
  {
    return m_vecRel->push2TTree(tree);
  }


  void TTreeVectorExtractor::loadFromVector()
  {


    m_vec->clear();
    for (size_t i = 0; i < m_vecRel->size();++i)
    {
      m_vec->push_back(m_vecRel->at(i));
    }
  }


  void TTreeVectorExtractor::PushToVector() const
  {
    m_vecRel->clear();
    for (auto&e:*m_vec)
    {
      m_vecRel->push_back(e);
    }
  }
  std::vector<double>* TTreeVectorExtractor::getVec() const
  {
    return m_vec;
  }



#else  //release

  TTreeVectorExtractor::TTreeVectorExtractor(const char* name, TTree* tree) :
    m_name(name)
   {

    tree->SetBranchAddress(m_name.c_str(), &m_vec);


  }


  TTreeVectorExtractor::TTreeVectorExtractor(const char* name) : m_name(name)
  {
    m_owend_vector = std::make_shared<std::vector<double>>();
    m_vec = m_owend_vector.get();
  }
  TTreeVectorExtractor::~TTreeVectorExtractor()
  {


  }
  bool TTreeVectorExtractor::push2TTree(TTree* tree)
  {

    if (!tree)
    {
      std::cout << "unable to push \"" << m_name << "\" to TTree. Tree is empty " << std::endl;
      return false;
    }

    tree->Branch(m_name.c_str(), &m_vec);

    return true;
  }



  std::vector<double>* TTreeVectorExtractor::getVec() const
  {
    return m_vec;
  }



#endif


  const char* TTreeVectorExtractor::getName() const
  {
    return m_name.c_str();
  }

  void TTreeVectorExtractor::reset()
  {
    if (!m_owend_vector)
    {
      std::cout << "unable to clear not owned vector \n";
    }
    m_vec->clear();
    
  }

  rootEventBase::rootEventBase(TTree* tree) :m_name(tree->GetName())
  {
    
    auto entries =  tree->GetListOfBranches()->GetEntries();

    for (int i = 0; i < entries; i++)
    {
      auto axisName = tree->GetListOfBranches()->At(i)->GetName();
      if (strcmp("event_nr" ,axisName)==0)
      {
        continue;
      }
      
      m_data.push_back(TTreeVectorExtractor( axisName, tree));
    }
    m_event_nr_ownd = std::make_shared<int>(0);
   
    tree->SetBranchAddress("event_nr", m_event_nr_ownd.get());

  }

  rootEventBase::rootEventBase(const char* collectionName, std::vector<std::string> axis_list) :m_name(collectionName)
  {
    for (auto&e : axis_list)
    {
      m_data.emplace_back(e.c_str());
    }
    m_event_nr_ownd = std::make_shared<int>(0);
    m_event_nr = m_event_nr_ownd.get();
  }


  rootEventBase::rootEventBase()
  {

  }

  std::shared_ptr<plane> rootEventBase::createPlane(double ID)
  {
    return std::make_shared<plane>(ID, this);
  }

  void rootEventBase::Save2Tree(TTree* outputTree)
  {
    for (auto& e:m_data)
    {
      e.push2TTree(outputTree);
    }
    outputTree->Branch("event_nr", m_event_nr_ownd.get());
  }



  std::vector<double>* rootEventBase::getData(const char* name) const
  {
    
    for (auto&e:m_data)
    {
      if (strcmp(name, e.getName()) ==0 )
      {
        return e.getVec();
      }
       
    }
    return nullptr;
  }

  std::vector<std::string> rootEventBase::getDataNames() const
  {
    std::vector<std::string> ret;
    for (auto&e : m_data)
    {
      ret.push_back(e.getName());
    }
    return ret;
  }

  const char* rootEventBase::getName() const
  {
    return m_name.c_str();
  }

  void rootEventBase::reset()
  {
    for (auto&e : m_data)
    {
      e.reset();
    }
  }

#ifdef _DEBUG
  void rootEventBase::loadFromVector()
  {
    for (auto& e:m_data)
    {
      e.loadFromVector();
    }
  }

  void rootEventBase::PushToVector() const
  {
    for (auto& e:m_data)
    {
      e.PushToVector();
    }
  }



#endif // _DEBUG

  int& rootEventBase::get_event_nr()
  {
    return *m_event_nr;
  }

  rootEventBaseAxis::rootEventBaseAxis(const rootEventBase& ev, axis_def ax, double id_) :m_planeID(id_), m_name(axis2String(ax))
  {
    m_axis = ev.getData(m_name.c_str());
    m_ID = ev.getData("ID");
  }

  rootEventBaseAxis::rootEventBaseAxis(const rootEventBase& ev, const char* ax_name, double id_) :m_planeID(id_), m_name(ax_name)
  {
    m_axis = ev.getData(m_name.c_str());
    m_ID = ev.getData("ID");
  }

  bool rootEventBaseAxis::next()
  {
    do{
      if (++m_curr >= static_cast<int>(m_ID->size()))
      {
        m_curr = -1;
        return false;
      }

    } while ((m_ID->at(m_curr) != m_planeID));

    return true;
  }

  double rootEventBaseAxis::get() const
  {
    return m_axis->at(m_curr);
  }

  const char* rootEventBaseAxis::getName() const
  {
    return m_name.c_str();
  }

  rootEventBaseAxisCollection::rootEventBaseAxisCollection(const rootEventBase& ev, double id_)
  {
    auto names = ev.getDataNames();
    for (auto& e : names){
      m_axis.emplace_back(ev, e.c_str(), id_);
    }
  }

  axis_ref* rootEventBaseAxisCollection::getAxis(axis_def ax) 
  {
    return getAxis(axis2String(ax));
  }

  axis_ref* rootEventBaseAxisCollection::getAxis(const char* axisName) 
  {
    for (auto& e:m_axis)
    {
      if (strcmp(axisName, e.getName()) == 0)
      {
        return dynamic_cast<axis_ref*>(&e);
      }
    }

    return nullptr;
  }

}
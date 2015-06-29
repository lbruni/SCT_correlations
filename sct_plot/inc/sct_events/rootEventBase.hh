#ifndef rootEventBase_h__
#define rootEventBase_h__
#include <vector>
#include <string>
#include "internal/axis_ref.hh"

class TTree;

#ifdef _DEBUG
#include "ReleaseVectorDef.h"
#endif // _DEBUG
#include "S_Axis.h"
#include <memory>


namespace sct_corr{
  class plane;

  class TTreeVectorExtractor{
  public:
    TTreeVectorExtractor(const char* name, TTree* tree);
    TTreeVectorExtractor(const char* name);
    TTreeVectorExtractor();
    bool push2TTree(TTree* tree);
    ~TTreeVectorExtractor();
    std::vector<double>* getVec() const;
    const char* getName() const;
    void reset();
#ifdef _DEBUG
    void loadFromVector();
    void PushToVector() const;
    std::shared_ptr<ReleaseVectorDef> m_vecRel;
#endif // _DEBUG

  private:
    std::string m_name;
    std::shared_ptr<std::vector<double>> m_owend_vector;
    std::vector<double>* m_vec=NULL;
    
  };


  class rootEventBase{
  public:
    rootEventBase(TTree* tree);
    rootEventBase(const char* collectionName,std::vector<std::string> axis_list);

    rootEventBase();
    virtual ~rootEventBase(){}
    virtual std::shared_ptr<plane> createPlane(double ID) ;
    void Save2Tree(TTree* outputTree);
    
    std::vector<double>* getData(const char* name) const;
    std::vector<double>* getData(axis_def ax) const;
    std::vector<std::string> getDataNames() const;
    const char* getName() const;
    void reset();
#ifdef _DEBUG
    void loadFromVector();
    void PushToVector() const;
#endif
    int& get_event_nr();
  private:
    std::string m_name;
    std::vector<TTreeVectorExtractor> m_data;
    std::shared_ptr<int> m_event_nr_ownd;
    int* m_event_nr = NULL;
  };

  class rootEventBaseAxis :public axis_ref{
  public:
    rootEventBaseAxis(const rootEventBase& ev, axis_def ax,double id_);
    rootEventBaseAxis(const rootEventBase& ev, const char* ax_name, double id_);
    virtual bool next() const override;
    virtual double get() const override;

    const char* getName() const;
    std::vector<double>* m_axis;
    std::vector<double>* m_ID;
    const double m_planeID = 0;
    mutable int m_curr = -1;
    std::string m_name;
  };


  class rootEventBaseAxisCollection{
  public:

    rootEventBaseAxisCollection(const rootEventBase& ev, double id_);
    const axis_ref* getAxis(axis_def ax) const;
    const axis_ref* getAxis(const char* axisName) const;
    bool next() const;
  private:
    std::vector<rootEventBaseAxis> m_axis;
  };
}

#endif // rootEventBase_h__

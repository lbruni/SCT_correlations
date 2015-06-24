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


namespace sct_corr{


  class TTreeVectorExtractor{
  public:
    TTreeVectorExtractor(const char* name, TTree* tree);
    TTreeVectorExtractor(const char* name);
    bool push2TTree(TTree* tree);
    ~TTreeVectorExtractor();
    std::vector<double>* getVec() const;
    const char* getName() const;
#ifdef _DEBUG
    void loadFromVector();
    void PushToVector();
    ReleaseVectorDef m_vecRel;
#endif // _DEBUG

  private:
    std::string m_name;
    std::vector<double> *m_vec;
    const bool m_classDoesNotOwnVector;
  };


  class rootEventBase{
  public:
    rootEventBase(TTree* tree);
    rootEventBase(const char* collectionName,std::vector<std::string> axis_list);
    void Save2Tree(TTree* outputTree);
    std::vector<double>* getData(const char* name) const;
    std::vector<std::string> getDataNames() const;
#ifdef _DEBUG
    void loadFromVector();
    void PushToVector();
#endif
  private:
    std::string m_name;
    std::vector<TTreeVectorExtractor> m_data;
  };

  class rootEventBaseAxis :public axis_ref{
  public:
    rootEventBaseAxis(const rootEventBase& ev, axis_def ax,double id_);
    rootEventBaseAxis(const rootEventBase& ev, const char* ax_name, double id_);
    virtual bool next() override;
    virtual double get() const override;

    const char* getName() const;
    std::vector<double>* m_axis;
    std::vector<double>* m_ID;
    const double m_planeID = 0;
    int m_curr = -1;
    std::string m_name;
  };


  class rootEventBaseAxisCollection{
  public:

    rootEventBaseAxisCollection(const rootEventBase& ev, double id_);
    axis_ref* getAxis(axis_def ax);
    axis_ref* getAxis(const char* axisName);
  private:
    std::vector<rootEventBaseAxis> m_axis;
  };
}

#endif // rootEventBase_h__

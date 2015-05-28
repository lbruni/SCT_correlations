#include "sct_plots.h"

#include "treeCollection.h"
#include <iostream>
#include "cluster.h"
#include "TCanvas.h"
#include "TVector.h"






class plot{
public:
  virtual void fill() = 0;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
};

plot* create_plot(const char* type, const char* name, axis_ref* x, axis_ref* y);

plot* create_plot(const char* type, const char* name, S_plane* x, S_plane* y);

class axis_ref{
public:
  virtual bool next() = 0;
  virtual double get() const = 0;

};


class plane{
public:
  plane(double ID, treeCollection* hits);


  axis_ref* getX();
  axis_ref* getY();

private:
  class axis_vector :public axis_ref{
  public:
    axis_vector(std::vector<double>*  axis, std::vector<double>* ID, double planeID);
    virtual bool next() override;
    virtual double get() const override;

    std::vector<double>* m_axis;
    std::vector<double>* m_ID;
    double m_planeID = 0;
    Int_t m_curr = -1;
  };

  axis_vector m_x, m_y;
  double m_ID = 0;
  treeCollection* m_hits = nullptr;

};

plane::plane(double ID, treeCollection* hits) :m_hits(hits), m_ID(ID), m_x(hits->x, hits->ID, ID), m_y(hits->y, hits->ID, ID)
{

}

axis_ref* plane::getX()
{
  return dynamic_cast<axis_ref*>(&m_x);
}

axis_ref* plane::getY()
{
  return dynamic_cast<axis_ref*>(&m_y);
}

plane::axis_vector::axis_vector(std::vector<double>* axis, std::vector<double>* ID, double planeID) :m_axis(axis), m_ID(ID), m_planeID(planeID)
{

}

bool plane::axis_vector::next()
{
  do{
    if (++m_curr >= m_ID->size())
    {
      m_curr = -1;
      return false;
    }

  } while ((m_ID->at(m_curr) != m_planeID));

  return true;
}

double plane::axis_vector::get() const
{
  return m_axis->at(m_curr);
}




class plotPlaneVsPlane :public plot{
public:
  plotPlaneVsPlane(const char* name, S_plane* x, S_plane* y) :m_x(x), m_y(y){
    m_outTree = std::make_shared<treeCollection_ouput>(name, &m_x_points, &m_y_points, &m_id, &m_current);

  }

  virtual void processEventStart() {}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) = 0;
  virtual void processEventEnd() {}
  virtual void fill() override final{
    m_size_x = 0, m_size_y = 0;
    m_x_points.clear();
    m_y_points.clear();
    m_id.clear();

    bool first = true;
    processEventStart();
    while (m_y->next()){
      ++m_size_x;
      while (m_x->next()){
        if (first)
        {
          ++m_size_y;
        }
        processHit(m_x->get(), m_y->get());
      }
      first = false;
    }
    processEventEnd();
    m_outTree->fill();
    ++m_current;
  }
  inline void pushHit(Double_t x, Double_t y){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(0);
    
  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    return m_outTree->Draw(axis, cuts, options);
  }

  std::vector<double> m_x_points, m_y_points, m_id;

  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;


  S_plane *m_x, *m_y;
  
  Int_t m_size_x = 0, m_size_y = 0;
};

class plane_distance :public plotPlaneVsPlane {
public:
  plane_distance(const char* name, S_plane* x, S_plane* y) : plotPlaneVsPlane(name, x, y){}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {
  
    pushHit(hit_abs(p1), hit_abs(p2));
  }
  static double hit_abs(const plane_hit& h){

    return sqrt(h.x*h.x + h.y * h.y);
  }
};
class plot_find_correspondingX :public plotPlaneVsPlane{
public:
  plot_find_correspondingX(const char* name, S_plane* x, S_plane* y) : plotPlaneVsPlane(name, x, y){}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {



    if (abs(p1.x - p2.x) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }

  }
};
class plot_Event_size :public plotPlaneVsPlane{
public:
  plot_Event_size(const char* name, S_plane* x, S_plane* y) : plotPlaneVsPlane(name, x, y){}
  virtual void processEventEnd() {

    pushHit(m_size_x, m_size_y);

  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {};
};
class plot_find_correspondingXY :public plotPlaneVsPlane{
public:
  plot_find_correspondingXY(const char* name, S_plane* x, S_plane* y) : plotPlaneVsPlane(name, x, y){}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {

    if (abs(p1.x - p2.x) < 0.1 && abs(p1.y - p2.y) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }

  }

};

class find_nearest :public plotPlaneVsPlane{
public:
  find_nearest(const char* name, S_plane* x, S_plane* y) : plotPlaneVsPlane(name, x, y){}
  virtual void processEventStart() {
    m_hit.clear();
  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {


    m_hit.emplace_back(abs(p1.x - p2.x), abs(p1.y - p2.y));

  }

  virtual void processEventEnd() {

    double r = 10000000000000;
    plane_hit h(0, 0);
    for (auto& e : m_hit)
    {
      auto r1 = sqrt((e.x)*(e.x) + (e.y)*(e.y));

      if (r1 > 0 && r1 < r)
      {
        r = r1;
        h = e;
      }

    }

    if (r < 100000000000)
    {
      pushHit(h.x, h.y);

    }
  }
  std::vector<plane_hit> m_hit;
};
class plot2d :public plot{

public:
  plot2d(const char* name, axis_ref* x, axis_ref* y) :m_x(x), m_y(y){
   
    m_outTree = std::make_shared<treeCollection_ouput>(name, &m_x_points, &m_y_points, &m_id, &m_current);
  
  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    return m_outTree->Draw(axis, cuts, options);
  }
  virtual void ProcessEvent() = 0;
  virtual void fill() override final{
    m_x_points.clear();
    m_y_points.clear();
    m_id.clear();
    

    ProcessEvent();

    m_outTree->fill();
    ++m_current;
  }
  inline void pushHit(Double_t x, Double_t y){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(0);

  
  }
protected:
  axis_ref* m_x;
  axis_ref* m_y;
  std::vector<double> m_x_points, m_y_points, m_id;

  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;
};
class plot_corr2d :public plot2d{
public:
  plot_corr2d(const char* name, axis_ref* x, axis_ref* y) :plot2d(name, x, y){}
  virtual void processEventStart() {}
  virtual void processHit(double x, double y) = 0;
  virtual void processEventEnd() {}
  virtual void ProcessEvent() override final{
    processEventEnd();
    while (m_y->next()){
      while (m_x->next()){
        //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
        processHit(m_x->get(), m_y->get());
      }
    }
    processEventEnd();
  }
};
class plot_hit2d :public plot2d{
public:
  plot_hit2d(const char* name, axis_ref* x, axis_ref* y) :plot2d(name, x, y){}
  virtual void processEventStart() {}
  virtual void processHit(double x, double y) = 0;
  virtual void processEventEnd() {}
  virtual void ProcessEvent() override final{
    processEventStart();
    while (m_y->next()){
      if (!m_x->next())
      {
        std::cout << "vector have different length" << std::endl;
        return;
      }
      //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
      processHit(m_x->get(), m_y->get());

    }

    if (m_x->next())
    {
      std::cout << "vector have different length" << std::endl;
    }
    processEventEnd();
  }
};

class hitmap :public plot_hit2d{
public:
  hitmap(const char* name, axis_ref* x, axis_ref* y) :plot_hit2d(name, x, y){}
  virtual void processHit(double x, double y) override{
    pushHit(m_x->get(), m_y->get());
  }

};
class ProjectOnPixel :public plot_hit2d{
public:
  ProjectOnPixel(const char* name, axis_ref* x, axis_ref* y) :plot_hit2d(name, x, y){}

  virtual void processHit(double x, double y) override{
    pushHit((int)(m_x->get() / m_x_pixelsize * 100) % 100, (int)(m_y->get() / m_ypixelsize * 100) % 100);
  }

  double m_x_pixelsize = 0.074, m_ypixelsize = 100000;

};
class clusterSize :public plot_hit2d{
public:
  clusterSize(const char* name, axis_ref* x, axis_ref* y) :plot_hit2d(name, x, y){}

  virtual void processEventStart() override {
    m_cl.reset();
  }

  virtual void processHit(double x, double y) override{

    m_cl.push_pixel(pixelHit<Double_t>(m_x->get(), m_y->get()));
  }
  virtual void processEventEnd() override {


    for (size_t i = 0; i < m_cl.NumOfCluster(); ++i){

      pushHit(m_cl.getCluster(i).m_hits.size(), 0);
    }
  }
  clusterMaker<Double_t> m_cl;

};
class correlations :public plot_corr2d{
public:
  correlations(const char* name, axis_ref* x, axis_ref* y) :plot_corr2d(name, x, y){}
  virtual void processHit(double x, double y) override{
    pushHit(x, y);
  }

};

class residual : public plot_corr2d {
public:
  residual(const char* name, axis_ref* x, axis_ref* y) :plot_corr2d(name, x, y){}
  virtual void processHit(double x, double y) override{
    pushHit(x - y, ++m_index);
  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "x") override{
    return  m_outTree->Draw(axis, cuts, options);
  }
private:
  double m_cut = -1;
  int m_index = 0;
};

plot* create_plot(const char* type, const char* name, axis_ref* x, axis_ref* y)
{
  std::string s_type(type);

  if (s_type.compare(sct::plot_hitmap()) == 0)
  {
    return dynamic_cast<plot*>(new hitmap(name, x, y));

  }

  if (s_type.compare(sct::plot_correlation()) == 0)
  {
    return dynamic_cast<plot*>(new correlations(name, x, y));
  }


  if (s_type.compare(sct::plot_residual()) == 0)
  {
    return dynamic_cast<plot*>(new residual(name, x, y));
  }

  if (s_type.compare(sct::plot_clusterSize()) == 0)
  {
    return dynamic_cast<plot*> (new clusterSize(name, x, y));
  }
  if (s_type.compare(sct::plot_projectOnPixel()) == 0)
  {
    return dynamic_cast<plot*>(new ProjectOnPixel(name, x, y));
  }
  return nullptr;
}

plot* create_plot(const char* type, const char* name, S_plane* x, S_plane* y){
  std::string s_type(type);
  if (s_type.compare(sct::plot_find_correspondingX()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_find_correspondingX(name, x, y));

  }

  if (s_type.compare(sct::plot_find_correspondingXY()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_find_correspondingXY(name, x, y));

  }


  if (s_type.compare(sct::plot_Event_size()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_Event_size(name, x, y));

  }

  if (s_type.compare(sct::plot_find_nearest()) == 0)
  {

    return dynamic_cast<plot*>(new  find_nearest(name, x, y));

  }


  if (s_type.compare(sct::plot_plane_distance()) == 0)
  {

    return dynamic_cast<plot*>(new  plane_distance(name, x, y));

  }



  return nullptr;
}

S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis)
{

}

S_DrawOption::S_DrawOption(const char* options /*= "colz"*/, const char* cuts /*= ""*/, const char * axis /*= "y:x"*/) : m_options(options), m_cuts(cuts), m_axis(axis)
{

}

S_plot_collection::S_plot_collection(TFile* file) 
{

  addFile(file);

}


void S_plot_collection::addFile(TFile* file)
{
  m_file.push_back(file);
}

void S_plot_collection::reset()
{

  m_planes.clear();
  m_plots.clear();
  m_drawOption.clear();
  m_trees.clear();
}

void S_plot_collection::addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis)
{
  addPlot(PlotType, name, x_axis, y_axis, S_DrawOption());
}

void S_plot_collection::addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis, S_DrawOption option)
{
  m_plots.push_back(std::make_pair(name, S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis))));
  //m_plots[name] std::move(S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis)));
  m_drawOption[name] = option;
}

void S_plot_collection::addPlot(const char* name, S_plot pl)
{
  m_plots.push_back(std::make_pair(name, std::move(pl)));
  m_drawOption[name] = S_DrawOption();
}

void S_plot_collection::addPlot(const char* PlotType, const char* name, S_plane p1, S_plane p2)
{
  auto p1_pointer = pushPlane(std::move(p1));
  auto p2_pointer = pushPlane(std::move(p2));
  if (p1_pointer && p2_pointer)
  {
    addPlot(name, S_plot(PlotType, name, p1_pointer, p2_pointer));
  }

}

void S_plot_collection::Draw()
{
  auto c = new TCanvas();

  c->Divide(m_plots.size(), 1);
  int i = 0;
  for (auto& e : m_plots){
    c->cd(i++);
    Draw(e.first.c_str());
  }
}

Long64_t S_plot_collection::Draw(const char* name)
{
  return Draw(name, m_drawOption[name]);
}

Long64_t S_plot_collection::Draw(const char* name, const S_DrawOption& option)
{
  for (auto &e:m_plots)
  {
    if (e.first==name)
    {
    return  e.second.Draw(option.m_options, option.m_cuts, option.m_axis);
    }
  }
  return 0;
}

void S_plot_collection::loop(Int_t last /*= -1*/, Int_t start /*= 0*/)
{
  if (last == -1)
  {
    last = kMaxInt;
    for (auto & e : m_trees)
    {
      auto l = e.second->GetEntries();
      if (last < kMaxInt && l != last)
      {
        std::cout << "trees must have the same length" << std::endl;


      }
      if (l < last)
      {
        last = l;
      }
    }
  }

  for (Int_t i = start; i < last; ++i)
  {
    for (auto& e : m_trees)
    {
      e.second->GetEntry(i);
    }

    for (auto& current_plot : m_plots){
      current_plot.second.fill();

    }
  }
}

axis_ref* S_plot_collection::getAxis_ref(const S_Axis & axis)
{
  if (axis.m_axis == x_axis_def)
  {
    return getPlane(axis.m_planeID, getCollection(axis.m_collectionName.Data()))->getX();
  }

  if (axis.m_axis == y_axis_def)
  {
    return getPlane(axis.m_planeID, getCollection(axis.m_collectionName.Data()))->getY();
  }

  return nullptr;
}

treeCollection* S_plot_collection::getCollection(const char* name)
{
  for (auto&e : m_trees)
  {
    if (e.first == name)
    {
      return e.second;
    }
  }

  if (isClobalCollection(name))
  {
    treeCollection* tree_pointer = new treeCollection(name);
    m_trees.push_back(std::make_pair(std::string(name), tree_pointer));
    return tree_pointer;
  }

  if (m_file.empty())
  {
    std::cout << "file empty" << std::endl;
    return nullptr;
  }
  TTree *collection = NULL;

  for (auto& e : m_file)
  {
    e->GetObject(name, collection);
    if (collection)
    {
      break;
    }
  }



  if (!collection)
  {
    std::cout << "collection not found! Collection name: \"" << name << "\"" << std::endl;
    return nullptr;
  }


  treeCollection* tree_pointer = new treeCollection(collection);
  m_trees.push_back(std::make_pair(std::string(name), tree_pointer));
  return tree_pointer;

}

S_plane* S_plot_collection::getPlane(double ID, treeCollection* coll)
{
  if (!coll)
  {
    return nullptr;
  }

  m_planes.push_back(std::make_shared<S_plane>(ID, coll));
  return m_planes.back().get();
}

S_plane* S_plot_collection::pushPlane(S_plane pl)
{
  if (!pl.isSetTreeCollectionSet())
  {
    auto pl_pointer = getCollection(pl.getName());
    if (!pl_pointer)
    {
      return nullptr;
    }
    pl.setTreeCollection(pl_pointer);

  }
  m_planes.push_back(std::make_shared<S_plane>(pl));

  return m_planes.back().get();
}

S_plot::S_plot(const char* type, const char* name, axis_ref* x, axis_ref* y)
{
  m_plot = std::shared_ptr<plot>(create_plot(type, name, x, y));
}

S_plot::S_plot() :m_plot(nullptr)
{
  std::cout << "[S_plot] unsupported default constructor" << std::endl;
}



S_plot::S_plot(const char* type, const char* name, S_plane* x, S_plane* y)
{
  m_plot = std::shared_ptr<plot>(create_plot(type, name, x, y));
}

S_plot::S_plot(const S_plot& pl)
{
  m_plot = pl.m_plot;
}

void S_plot::fill()
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return;
  }
  m_plot->fill();
}

Long64_t S_plot::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return -1;
  }
  return m_plot->Draw(options, cuts, axis);
}

S_plane::S_plane(double ID, treeCollection* hits) : m_ID(ID)
{
  setTreeCollection(hits);
}

S_plane::S_plane() : m_plane(nullptr)
{
  std::cout << "[s_plane] unsupported default constructor do not use" << std::endl;
}

S_plane::S_plane(double ID, S_treeCollection* hits) : S_plane(ID, hits->m_tree.get())
{

}



S_plane::S_plane(const char* name, Double_t ID) :m_name(name), m_ID(ID)
{

}

bool S_plane::isSetTreeCollectionSet() const
{
  if (m_plane)
  {
    return true;
  }
  return false;
}

void S_plane::setTreeCollection(S_treeCollection* hits)
{

  setTreeCollection(hits->m_tree.get());


}

void S_plane::setTreeCollection(treeCollection* hits)
{

  m_plane = std::make_shared<plane>(m_ID, hits);
}

const char * S_plane::getName() const
{
  return m_name.c_str();
}

bool S_plane::next()
{

  if (getX()->next()){
    if (!getY()->next())
    {
      std::cout << "vector have different length" << std::endl;
      return false;
    }

    return true;
  }

  if (getY()->next())
  {
    std::cout << "vector have different length" << std::endl;
    return false;
  }
  return false;
}

plane_hit S_plane::get() const
{
  return plane_hit(getX()->get(), getY()->get());
}

axis_ref* S_plane::getX() const
{
  if (!m_plane)
  {
    std::cout << "[s_plane] plane not set " << std::endl;
    return nullptr;
  }
  return m_plane->getX();
}

axis_ref* S_plane::getY() const
{
  if (!m_plane)
  {
    std::cout << "[s_plane] plane not set " << std::endl;
    return nullptr;
  }
  return m_plane->getY();
}

const char* sct::col_hit()
{
  return "hit";
}

const char* sct::col_zsdata_strip()
{
  return "zsdata_strip";
}

const char* sct::col_zsdata_m26()
{
  return "zsdata_m26";
}

const char* sct::col_zsdata_apix()
{
  return "zsdata_apix";
}

const char* sct::col_fitpoints()
{
  return "fitpoints";
}

const char* sct::col_fitpoints_local()
{
  return "fitpoints_local";
}

const char* sct::col_local_hit()
{
  return "local_hit";
}

const char* sct::plot_hitmap()
{
  return "hitmap";
}

const char* sct::plot_correlation()
{
  return "correlation";
}

const char* sct::plot_residual()
{
  return "residual";
}

const char* sct::plot_clusterSize()
{
  return "clusterSize";
}

const char* sct::plot_projectOnPixel()
{
  return "ProjectOnPixel";
}

const char* sct::plot_find_correspondingX()
{
  return  "plot_find_correspondingX";
}

const char* sct::plot_find_correspondingXY()
{
  return  "plot_find_correspondingXY";
}

const char* sct::plot_Event_size()
{
  return "plot_Event_size_____";
}

const char* sct::plot_find_nearest()
{
  return "plot_find_nearest";
}

const char* sct::plot_plane_distance()
{
  return "plot_plane_distance____";
}

S_treeCollection::S_treeCollection(TTree *tree) :m_tree(new treeCollection(tree))
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



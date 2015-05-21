#include "sct_plots.h"

#include "treeCollection.h"
#include <iostream>


class plot{
public:
  virtual void fill() = 0;
  virtual void Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
};

plot* create_plot(const char* type, const char* name, axis_ref* x, axis_ref* y);



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

class plot2d :public plot{

public:
  plot2d(const char* name, axis_ref* x, axis_ref* y) :m_x(x), m_y(y){
    tree = new TTree(name, name);
    tree->Branch("x", &m_x_point, "x/D");
    tree->Branch("y", &m_y_point, "y/D");
  }
  virtual void Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    tree->Draw(axis, cuts, options);
  }

protected:
  axis_ref* m_x;
  axis_ref* m_y;
  double m_x_point = 0, m_y_point = 0;
  TTree *tree;
};


class hitmap :public plot2d{
public:
  hitmap(const char* name, axis_ref* x, axis_ref* y) :plot2d(name, x, y){}

  virtual void fill() override{
    while (m_y->next()){
      if (!m_x->next())
      {
        std::cout << "vector have different length" << std::endl;
        return;
      }
      //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
      m_x_point = m_x->get();
      m_y_point = m_y->get();
      tree->Fill();
    }

    if (m_x->next())
    {
      std::cout << "vector have different length" << std::endl;
    }
  }

};
class correlations :public plot2d{
public:
  correlations(const char* name, axis_ref* x, axis_ref* y) :plot2d(name, x, y){}
  virtual void fill() override{
    while (m_y->next()){
      while (m_x->next()){
        //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
        m_x_point = m_x->get();
        m_y_point = m_y->get();
        tree->Fill();
      }
    }
  }

};

class residual : public plot2d {
public:
  residual(const char* name, axis_ref* x, axis_ref* y) :plot2d(name, x, y){}

  virtual void fill() override{
    while (m_y->next()){
      while (m_x->next()){
        //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
        m_x_point = m_x->get() - m_y->get();
        ++m_y_point;
        tree->Fill();

      }
    }
  }
  virtual void Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    tree->Draw("x", "", options);
  }
private:
  double m_cut = -1;
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
    return dynamic_cast<plot*>(new correlations(name, x, y));
  }



  return nullptr;
}

S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis)
{

}

S_DrawOption::S_DrawOption(const char* options /*= "colz"*/, const char* cuts /*= ""*/, const char * axis /*= "y:x"*/) : m_options(options), m_cuts(cuts), m_axis(axis)
{

}

S_plot_collection::S_plot_collection(TFile* file) : m_file(file)
{

}


void S_plot_collection::addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis)
{
  addPlot(PlotType, name, x_axis, y_axis, S_DrawOption());
}

void S_plot_collection::addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis, S_DrawOption option)
{
  m_plots.emplace(std::make_pair(name, S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis))));
  //m_plots[name] std::move(S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis)));
  m_drawOption[name] = option;
}

void S_plot_collection::Draw()
{
  for (auto& e : m_plots){
    Draw(e.first.c_str());
  }
}

void S_plot_collection::Draw(const char* name)
{
  Draw(name, m_drawOption[name]);
}

void S_plot_collection::Draw(const char* name, const S_DrawOption& option)
{
  m_plots[name].Draw(option.m_options, option.m_cuts, option.m_axis);
}

void S_plot_collection::loop(Int_t last /*= -1*/, Int_t start /*= 0*/)
{
  if (last == -1)
  {
    for (auto & e : m_trees)
    {
      auto l = e.second->GetEntries();
      if (last > 0 && l != last)
      {
        std::cout << "trees must have the same length" << std::endl;
      }
      last = l;
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
  auto it = m_trees.find(name);
  if (it != m_trees.end())
  {
    return it->second;
  }


  if (!m_file)
  {
    std::cout << "file empty" << std::endl;
    return nullptr;
  }
  TTree *collection = NULL;

  m_file->GetObject(name, collection);
  if (!collection)
  {
    std::cout << "collection not found" << std::endl;
    return nullptr;
  }

  m_trees[name] = new treeCollection(collection);

  return m_trees[name];
}

S_plane* S_plot_collection::getPlane(double ID, treeCollection* coll)
{
  if (!coll)
  {
    return nullptr;
  }

  m_planes.emplace_back(ID, coll);
  return &m_planes.back();
}

S_plot::S_plot(const char* type, const char* name, axis_ref* x, axis_ref* y)
{
  m_plot = std::shared_ptr<plot>(create_plot(type, name, x, y));
}

S_plot::S_plot() :m_plot(nullptr)
{
  std::cout << "[S_plot] unssupported default constructor" << std::endl;
}



void S_plot::fill()
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return;
  }
  m_plot->fill();
}

void S_plot::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "y:x"*/)
{
  if (!m_plot) {
    std::cout << "[S_plot] plot not set" << std::endl;
    return;
  }
  m_plot->Draw(options, cuts, axis);
}

S_plane::S_plane(double ID, treeCollection* hits)
{
  m_plane = std::make_shared<plane>(ID, hits);
}

S_plane::S_plane() :m_plane(nullptr)
{
  std::cout << "[s_plane] unsupported default constructor do not use" << std::endl;
}

S_plane::S_plane(double ID, S_treeCollection* hits) : S_plane(ID, hits->m_tree.get())
{

}



axis_ref* S_plane::getX()
{
  if (!m_plane)
  {
    std::cout << "[s_plane] plane not set " << std::endl;
    return nullptr;
  }
  return m_plane->getX();
}

axis_ref* S_plane::getY()
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

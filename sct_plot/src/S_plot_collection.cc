
#include "sct_plots.h"
#include "sct_plots_internal.h"
#include "treeCollection.h"
#include <iostream>
#include "TCanvas.h"

#include "sct_event_buffer.h"
#include "sct_global.h"


S_plot_collection::S_plot_collection(TFile* file) :m_eventBuffer(std::make_shared<sct_event_buffer>())
{

  addFile(file);
  setGlobalPlotCollection(m_eventBuffer.get());
}


void S_plot_collection::addFile(TFile* file)
{
  m_file.push_back(file);
}

void S_plot_collection::reset()
{
  m_eventBuffer->reset();
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
  addPlot(S_plot_def(PlotType, name), std::move(p1), std::move(p2));

}

void S_plot_collection::addPlot(S_plot_def plot_def, S_plane p1, S_plane p2)
{
  auto p1_pointer = pushPlane(std::move(p1));
  auto p2_pointer = pushPlane(std::move(p2));

  if (p1_pointer && p2_pointer)
  {
    addPlot(plot_def.m_name.c_str(), S_plot(plot_def, p1_pointer, p2_pointer));
  }
  else{
    std::cout << "planes not set correctly!! \n";
  }
}

void S_plot_collection::addPlot(S_plot_def plot_def, S_Axis x_axis, S_Axis y_axis)
{
  m_plots.push_back(std::make_pair(plot_def.m_name, S_plot(plot_def, getAxis_ref(x_axis), getAxis_ref(y_axis))));
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
  for (auto &e : m_plots)
  {
    if (e.first == name)
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
        if (l<kMaxInt)
        {
          std::cout << "trees must have the same length" << std::endl;
        }


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

  if (m_eventBuffer->IsCollection(name))
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

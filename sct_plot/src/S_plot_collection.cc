
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

s_plane_collection S_plot_collection::addPlot(const char* PlotType, const char* name, const S_Axis& x_axis, const S_Axis& y_axis)
{
  return addPlot(PlotType, name, x_axis, y_axis, S_DrawOption());
}

s_plane_collection S_plot_collection::addPlot(const char* PlotType, const char* name, S_Axis x_axis, S_Axis y_axis, S_DrawOption option)
{
  Buffer_accessor buffer(m_eventBuffer.get());
  m_drawOption[name] = option;
  return addPlot(name, S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis)));


  //m_plots.push_back(std::make_pair(name, S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis))));
  //m_plots[name] std::move(S_plot(PlotType, name, getAxis_ref(x_axis), getAxis_ref(y_axis)));

}

s_plane_collection S_plot_collection::addPlot(const char* name, const S_plot& pl)
{
  m_plots.push_back(std::make_pair(name, std::move(pl)));
  m_drawOption[name] = S_DrawOption();
  return m_plots.back().second.getOutputcollection();
}

s_plane_collection S_plot_collection::addPlot(const char* PlotType, const char* name, const S_plane_def& p1, const S_plane_def & p2)
{
  return addPlot(S_plot_def(PlotType, name), p1, p2);

}


s_plane_collection S_plot_collection::addPlot(S_plot_def plot_def, const s_plane_collection& p1)
{
  Buffer_accessor buffer(m_eventBuffer.get());
  for (auto &e : p1.m_planes)
  {
    auto p1_pointer = pushPlane(e.second);
    if (!p1_pointer)
    {
      std::cout << "[S_plot_collection] planes not set correctly!!" << std::endl;
      return s_plane_collection();
    }
    plot_def.m_planes.push_back(p1_pointer);
  }

  return addPlot_internal(plot_def);
}

s_plane_collection S_plot_collection::addPlot(const S_plot_def& plot_def, const  S_plane_def& p1)
{
  return addPlot(plot_def, p1.getX_def(), p1.getY_def());
}

s_plane_collection S_plot_collection::addPlot(const  S_plot_def& plot_def, const S_plane_def& p1, const S_plane_def & p2)
{
  Buffer_accessor buffer(m_eventBuffer.get());
  auto p1_pointer = pushPlane(p1);
  auto p2_pointer = pushPlane(p2);

  if (p1_pointer && p2_pointer)
  {

    return addPlot(plot_def.m_name.c_str(), std::move(S_plot(plot_def, p1_pointer, p2_pointer)));
  }
  else{
    std::cout << "planes not set correctly!! \n";
  }

  return s_plane_collection();
}

s_plane_collection S_plot_collection::addPlot(S_plot_def plot_def, const S_Axis& x_axis, const S_Axis& y_axis)
{
  Buffer_accessor  buffer(m_eventBuffer.get());
  plot_def.m_axis.push_back(getAxis_ref(x_axis));
  plot_def.m_axis.push_back(getAxis_ref(y_axis));

  return addPlot_internal(plot_def);
}

s_plane_collection S_plot_collection::addPlot_internal(const S_plot_def& plot_def)
{
  m_plots.push_back(std::make_pair(plot_def.m_name, S_plot(plot_def)));
  return m_plots.back().second.getOutputcollection();
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

Long64_t S_plot_collection::Draw(const S_plane_def& name, const S_DrawOption& option)
{

  S_DrawOption local(option);



  if (option.m_cuts.Length() == 0){

    local.m_cuts = TString("ID==") + TString(std::to_string(name.getID()).c_str());
  }
  else{
    local.m_cuts = TString("(") + option.m_cuts + TString(") && ID == ") + TString(std::to_string(name.getID()).c_str());
  }
  return Draw(name.getName(), local);

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
        if (l < kMaxInt)
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

  m_planes.push_back(std::make_shared<S_plane>(S_plane_def(coll->getName(), ID), coll));
  return m_planes.back().get();
}

S_plane* S_plot_collection::pushPlane(const S_plane_def& pl)
{

  auto pl_pointer = getCollection(pl.getName());
  if (!pl_pointer)
  {
    return nullptr;
  }


  m_planes.push_back(std::make_shared<S_plane>(pl, pl_pointer));

  return m_planes.back().get();
}

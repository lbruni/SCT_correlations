
#include "s_plot_collection.h"
#include "sct_plots_internal.h"
#include "treeCollection.h"
#include <iostream>
#include "TCanvas.h"

#include "sct_event_buffer.h"
#include "sct_global.h"
#include "internal/plotsBase.hh"
#include "s_DrawOption.h"
#include "s_plane.h"

S_plot_collection::S_plot_collection(TFile* file) :m_eventBuffer(std::make_shared<sct_corr::sct_event_buffer>())
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

s_plane_collection S_plot_collection::addPlot( S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis)
{
  sct_corr::Buffer_accessor buffer(m_eventBuffer.get());
  plot_def.m_plot->pushAxis(getAxis_ref(x_axis));
  plot_def.m_plot->pushAxis(getAxis_ref(y_axis));
  return addPlot_internal(std::move(plot_def));
}

s_plane_collection S_plot_collection::addPlot_internal(S_plot plot_def)
{
  m_plots.push_back(std::make_pair(plot_def.getName(), std::move(plot_def)));
  if (!m_plots.back().second.m_plot->isReady())
  {
    std::cout << "[S_plot_collection]  unable to create plot " << plot_def.getType() << ":"<< plot_def.getName()<< "\n";
   return s_plane_collection();
  }
   
  return m_plots.back().second.getOutputcollection();
}

s_plane_collection S_plot_collection::addPlot(S_plot plot_def, const S_plane_def& p1)
{
  return addPlot(std::move(plot_def), s_plane_collection(p1));
}

s_plane_collection S_plot_collection::addPlot(S_plot plot_def, const S_plane_def& p1, const S_plane_def & p2)
{
  return addPlot(std::move(plot_def), p1 + p2);
}
s_plane_collection S_plot_collection::addPlot(S_plot plot_def, const s_plane_collection& p1)
{
  sct_corr::Buffer_accessor buffer(m_eventBuffer.get());
  for (auto &e : p1.m_planes)
  {
    auto p1_pointer = pushPlane(e.second);
    if (!p1_pointer)
    {
      std::cout << "[S_plot_collection] planes not set correctly!!" << std::endl;
      return s_plane_collection();
    }
    plot_def.m_plot->pushPlane(p1_pointer);
  }

  return addPlot_internal(plot_def);
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
      return  e.second.Draw(option);
    }
  }
  return 0;
}
Long64_t S_plot_collection::Draw(const s_plane_collection& name, const S_DrawOption& option)
{
  if (name.m_planes.size()>1)
  {
    std::cout << "[S_plot_collection] multiple planes defined. use DrawAll to Draw all planes. this function only draws the first plane" << std::endl;
  }
  if (name.m_planes.empty())
  {
    std::cout << "[S_plot_collection] no planes defined" << std::endl;
    return -1;
  }
  return Draw(name.get(0), option);

}
Long64_t S_plot_collection::DrawAll(const s_plane_collection& name, const S_DrawOption& option)
{
  if (name.m_planes.empty())
  {
    std::cout << "[S_plot_collection] no planes defined" << std::endl;
    return -1;
  }

  for (auto&e : name.m_planes){

    Draw(e.second, option);
  }
}

Long64_t S_plot_collection::Draw(const S_plane_def& name, const S_DrawOption& option)
{

  S_DrawOption local(option);

  TCut dummy = ("ID == " + std::to_string(name.getID())).c_str();
  local.cut_add(dummy);

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

sct_corr::axis_ref* S_plot_collection::getAxis_ref(const S_Axis & axis)
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

sct_corr::treeCollection* S_plot_collection::getCollection(const char* name)
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
    sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(name);

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


  sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(collection);

  m_trees.push_back(std::make_pair(std::string(name), tree_pointer));
  return tree_pointer;

}

S_plane* S_plot_collection::getPlane(double ID, sct_corr::treeCollection* coll)
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

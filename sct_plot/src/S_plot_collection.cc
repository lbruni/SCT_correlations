
#include "s_plot_collection.h"
#include "sct_plots_internal.h"
#include "treeCollection.h"
#include <iostream>
#include "TCanvas.h"

#include "sct_event_buffer.h"
#include "sct_global.h"
#include "internal/plotsBase.hh"
#include "s_DrawOption.h"
#include "s_plane_def.h"
#include "TTree.h"
#include "s_plane_def.h"
#include <ctime>

S_plot_collection::S_plot_collection(TFile* file) :m_eventBuffer(std::make_shared<sct_corr::sct_event_buffer>())
{

  addFile(file);

}


S_plot_collection::S_plot_collection(const char* file_name) :m_eventBuffer(std::make_shared<sct_corr::sct_event_buffer>()) {
  m_main_file = std::make_shared<TFile>(file_name);
  addFile(m_main_file.get());
}

void S_plot_collection::addFile(TFile* file)
{
  m_file.push_back(file);
}

void S_plot_collection::setOutputFile(TFile* file)
{
  if (file)
  {
    addFile(file);
  
  }
  m_eventBuffer->setOutputFile(file);
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
  plot_def.m_plot->pushAxis(getAxis_ref(x_axis));
  plot_def.m_plot->pushAxis(getAxis_ref(y_axis));
  return addPlot_internal(std::move(plot_def));
}


s_plane_collection S_plot_collection::addPlot_internal(S_plot plot_def)
{
  m_plots.push_back(std::make_pair(plot_def.getName(), std::move(plot_def)));
  if (!m_plots.back().second.m_plot->MakeReadyForData(m_eventBuffer.get()))
  {
    std::cout << "[S_plot_collection]  unable to create plot " << plot_def.getType() << ":"<< plot_def.getName()<< "\n";
   return s_plane_collection();
  }
  auto ret = m_plots.back().second.getOutputcollection();
  ret.set_s_plot_collection(m_self);
  return ret;
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
  auto tree_ = getTTree(name);
  if (tree_)
  {
    return option.Draw(tree_);
  }
  return -1;
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
  Long64_t n = 0;
  for (auto&e : name.m_planes){

    n+=Draw(e.second, option);
  }
  return n;
}


Long64_t S_plot_collection::Draw(const S_plane_def& name, const S_DrawOption& option)
{

  S_DrawOption local(option);

  TCut dummy = ("ID == " + std::to_string(name.getID())).c_str();
  local.cut_add(dummy);

  return Draw(name.getName(), local);

}
Int_t S_plot_collection::getMaxEntriesFromTree(Int_t last)
{
  bool external_last_event = true;
  if (last == -1)
  {
    last = kMaxInt;
    external_last_event = false;
  }
    for (auto & e : m_trees)
    {
      auto l = e.second->GetEntries();
      if (last < kMaxInt && l != last)
      {
        if (l < kMaxInt && external_last_event==false)
        {
          std::cout << "trees must have the same length" << std::endl;
        }


      }
      if (l < last)
      {
        last = l;
      }
    }
  
  return last;
}

void S_plot_collection::loop(Int_t last /*= -1*/, Int_t start /*= 0*/)
{

//  std::clock_t    start_time = std::clock();
  last=getMaxEntriesFromTree(last);
  for (Int_t i = start; i < last; ++i)
  {
#ifdef _DEBUG

    if (i%1000==0)
    {
      std::cout << "processing event: " << i << " of " << last <<"\n";
    }
#endif // _DEBUG

    for (auto& e : m_trees)
    {
      e.second->GetEntry(i);
    }

    for (auto& current_plot : m_plots){
      if (!current_plot.second.fill())
      {
        std::cout << "run terminated by plot: " << current_plot.first << std::endl;
        return;
      }
      

    }
  }

//   auto end_clock = clock();
//   std::cout << "processed: " << last << " events in  ";
//   std::cout << (std::clock() - start_time) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

bool S_plot_collection::collectionExist(const char* name) const{


  if (getTTree(name))
  {
    return true;
  }
  return false;
}

void S_plot_collection::set_self_weak_pointer(std::weak_ptr<S_plot_collection> self_) {
  m_self = self_;
}

const sct_corr::axis_ref* S_plot_collection::getAxis_ref(const S_Axis & axis)
{

  return getPlane(axis.m_planeID, getCollection(axis.m_collectionName.c_str()))->getAxis(axis.m_axis);
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
    sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(name,m_eventBuffer.get());

    m_trees.push_back(std::make_pair(std::string(name), tree_pointer));
    return tree_pointer;
  }

  if (m_file.empty())
  {
    std::cout << "file empty" << std::endl;
    return nullptr;
  }
  TTree *collection = getTTree(name);



  if (!collection)
  {
    std::cout << "collection not found! Collection name: \"" << name << "\"" << std::endl;
    return nullptr;
  }


  sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(collection);

  m_trees.push_back(std::make_pair(std::string(name), tree_pointer));
  return tree_pointer;

}

TTree* S_plot_collection::getTTree(const char* name) const {
  TTree *collection = NULL;

  for (auto& e : m_file) {
    e->GetObject(name, collection);
    if (collection) {
      return collection;
    }
  }
  return nullptr;
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

r_plot_collection::r_plot_collection(TFile* file) {
  m_plot = std::make_shared<S_plot_collection>(file);
  m_plot->set_self_weak_pointer(m_plot);
}

s_plane_collection r_plot_collection::addPlot(S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis) {
  
  s_plane_collection ret = m_plot->addPlot(plot_def, x_axis, y_axis);

  return ret;
}

s_plane_collection r_plot_collection::addPlot(S_plot plot_def, const S_plane_def& p1, const S_plane_def & p2) {
  s_plane_collection ret=m_plot->addPlot(plot_def, p1, p2);


  return ret;
}

s_plane_collection r_plot_collection::addPlot(S_plot plot_def, const S_plane_def& p1) {
  s_plane_collection ret = m_plot->addPlot(plot_def, p1);


  return ret;
}

s_plane_collection r_plot_collection::addPlot(S_plot plot_def, const s_plane_collection& p1) {
  s_plane_collection ret = m_plot->addPlot(plot_def, p1);


  return ret;
}

Long64_t r_plot_collection::Draw(const char* name) {
 return m_plot->Draw(name);
}

Long64_t r_plot_collection::Draw(const char* name, const S_DrawOption& option) {
return  m_plot->Draw(name,option);
}

Long64_t r_plot_collection::Draw(const S_plane_def& name, const S_DrawOption& option) {
return  m_plot->Draw(name, option);
}

Long64_t r_plot_collection::Draw(const s_plane_collection& name, const S_DrawOption& option) {
return  m_plot->Draw(name, option);
}

Long64_t r_plot_collection::DrawAll(const s_plane_collection& name, const S_DrawOption& option) {
 return m_plot->DrawAll(name, option);
}

void r_plot_collection::loop(Int_t last /*= -1*/, Int_t start /*= 0*/) {
  m_plot->loop(last, start);
}

S_plot_collection& r_plot_collection::get_plot_collection() {
  return *m_plot.get();
}

std::shared_ptr<S_plot_collection> r_plot_collection::get_plot_collection_ptr() {
  return m_plot;
}

void r_plot_collection::addFile(TFile* file) {
  m_plot->addFile(file);
}

void r_plot_collection::setOutputFile(TFile* file) {
  m_plot->setOutputFile(file);
}

void r_plot_collection::reset() {
  m_plot->reset();
}

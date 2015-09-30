
#include "s_plot_collection.h"
#include "sct_plots_internal.h"
#include "treeCollection.h"
#include <iostream>
#include "TCanvas.h"

#include "sct_event_buffer.h"

#include "internal/plotsBase.hh"
#include "s_DrawOption.h"
#include "plane_def.h"
#include "TTree.h"
#include "plane_def.h"
#include <ctime>
#include "internal/plot_collection_impl.hh"


namespace sct_corr {






sct_corr::plot_collection::plot_collection()  {

}
plot_collection::~plot_collection() {

}




plot_collection_impl::plot_collection_impl() : m_eventBuffer(std::make_shared<sct_corr::sct_event_buffer>()) {

}

plot_collection_impl::~plot_collection_impl() {

}

void plot_collection_impl::addFile(TFile* file) {
  m_file.push_back(file);
}

void plot_collection_impl::setOutputFile(TFile* file) {
  if (file) {
    addFile(file);

  }
  m_eventBuffer->setOutputFile(file);
}

void plot_collection_impl::reset() {
  m_eventBuffer->reset();
  m_planes.clear();
  m_plots.clear();
  m_trees.clear();
}

s_plane_collection plot_collection_impl::addPlot(S_plot plot_def, const S_Axis& x_axis, const S_Axis& y_axis) {
  plot_def.m_plot->pushAxis(getAxis_ref(x_axis));
  plot_def.m_plot->pushAxis(getAxis_ref(y_axis));
  return addPlot_internal(std::move(plot_def));
}


s_plane_collection plot_collection_impl::addPlot_internal(S_plot plot_def) {
  m_plots.push_back(std::make_pair(plot_def.getName(), std::move(plot_def)));
  if (!m_plots.back().second.m_plot->MakeReadyForData(m_eventBuffer.get())) {
    std::cout << "[S_plot_collection]  unable to create plot " << plot_def.getType() << ":" << plot_def.getName() << "\n";
    return s_plane_collection();
  }
  auto ret = m_plots.back().second.getOutputcollection();
  ret.set_plot_collection(m_self);
  return ret;
}

s_plane_collection plot_collection_impl::addPlot(S_plot plot_def, const plane_def& p1) {
  return addPlot(std::move(plot_def), s_plane_collection(p1));
}

s_plane_collection plot_collection_impl::addPlot(S_plot plot_def, const plane_def& p1, const plane_def & p2) {
  return addPlot(std::move(plot_def), p1 + p2);
}


s_plane_collection plot_collection_impl::addPlot(S_plot plot_def, const s_plane_collection& p1) {
  for (auto &e : p1.m_planes) {
    auto p1_pointer = pushPlane(e.second);
    if (!p1_pointer) {
      std::cout << "[S_plot_collection] planes not set correctly!!" << std::endl;
      return s_plane_collection();
    }
    plot_def.m_plot->pushPlane(p1_pointer);
  }

  return addPlot_internal(plot_def);
}




Long64_t plot_collection_impl::Draw(const char* name, const S_DrawOption& option) {
  for (auto &e : m_plots) {
    if (e.first == name) {
      return  e.second.Draw(option);
    }
  }
  auto tree_ = getTTree(sct_type::collectionName_t(name));
  if (tree_) {
    return option.Draw(tree_);
  }
  return -1;
}



Long64_t plot_collection_impl::Draw(const sct_corr::plane_def& name, const S_DrawOption& option) {

  S_DrawOption local(option);

  TCut dummy = ("ID == " + std::to_string( necessary_CONVERSION(name.getID()))).c_str(); //external library 
  local.cut_add(dummy);

  return Draw(Un_necessary_CONVERSION(name.getName()).c_str(), local);

}
Long64_t plot_collection_impl::getMaxEntriesFromTree(Long64_t last) {
  bool external_last_event = true;
  if (last == -1) {
    last = kMaxInt;
    external_last_event = false;
  }
  for (auto & e : m_trees) {
    auto l = e.second->GetEntries();
    if (last < kMaxInt && l != last) {
      if (l < kMaxInt && external_last_event == false) {
        std::cout << "trees must have the same length" << std::endl;
      }


    }
    if (l < last) {
      last = l;
    }
  }

  return last;
}

void plot_collection_impl::loop(Long64_t last /*= -1*/, Long64_t start /*= 0*/) {

  //  std::clock_t    start_time = std::clock();
  last = getMaxEntriesFromTree(last);
  for (Long64_t i = start; i < last; ++i) {
#ifdef _DEBUG

    if (i % 1000 == 0) {
      std::cout << "processing event: " << i << " of " << last << "\n";
    }
#endif // _DEBUG

    for (auto& e : m_trees) {
      e.second->GetEntry(i);
    }

    for (auto& current_plot : m_plots) {
      if (!current_plot.second.fill()) {
        std::cout << "run terminated by plot: " << current_plot.first << std::endl;
        return;
      }


    }
  }

  //   auto end_clock = clock();
  //   std::cout << "processed: " << last << " events in  ";
  //   std::cout << (std::clock() - start_time) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

bool plot_collection_impl::collectionExist(const sct_type::collectionName_t& name) const {


  if (getTTree(name)) {
    return true;
  }
  return false;
}

void plot_collection_impl::set_self_weak_pointer(std::weak_ptr<sct_corr::plot_collection> self_) {
  m_self = self_;
}

const sct_corr::axis_ref* plot_collection_impl::getAxis_ref(const S_Axis & axis) {

  return getPlane(axis.m_planeID, getCollection(axis.m_collectionName))->getAxis(axis.m_axis);
}

sct_corr::treeCollection* plot_collection_impl::getCollection(const sct_type::collectionName_t& name) {
  for (auto&e : m_trees) {
    if (Un_necessary_CONVERSION(e.first) == Un_necessary_CONVERSION(name)) { // mising opperator == 
      return e.second;
    }
  }

  if (m_eventBuffer->IsCollection(name)) {
    sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(name, m_eventBuffer.get());

    m_trees.push_back(std::make_pair(name, tree_pointer));
    return tree_pointer;
  }

  if (m_file.empty()) {
    std::cout << "file empty" << std::endl;
    return nullptr;
  }
  TTree *collection = getTTree(name);



  if (!collection) {
    std::cout << "collection not found! Collection name: \"" << necessary_CONVERSION(name) << "\"" << std::endl;
    return nullptr;
  }


  sct_corr::treeCollection* tree_pointer = new sct_corr::treeCollection(collection);

  m_trees.push_back(std::make_pair(name, tree_pointer));
  return tree_pointer;

}

TTree* plot_collection_impl::getTTree(const sct_type::collectionName_t& name) const {
  TTree *collection = NULL;

  for (auto& e : m_file) {
    e->GetObject(necessary_CONVERSION(name).c_str(), collection); ////external library 
    if (collection) {
      return collection;
    }
  }
  return nullptr;
}

S_plane* plot_collection_impl::getPlane(const sct_type::ID_t&  ID, sct_corr::treeCollection* coll) {
  if (!coll) {
    return nullptr;
  }

  m_planes.push_back(std::make_shared<S_plane>(plane_def(coll->getName(), ID), coll));
  return m_planes.back().get();
}

S_plane* plot_collection_impl::pushPlane(const plane_def& pl) {

  auto pl_pointer = getCollection(pl.getName());
  if (!pl_pointer) {
    return nullptr;
  }


  m_planes.push_back(std::make_shared<S_plane>(pl, pl_pointer));

  return m_planes.back().get();
}

std::shared_ptr<plot_collection> create_plot_collection() {
  auto  ret = std::make_shared<plot_collection_impl>();

  ret->set_self_weak_pointer(ret);

  return std::dynamic_pointer_cast<plot_collection>(ret);
}





}
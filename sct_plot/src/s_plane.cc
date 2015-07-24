#include "s_plane.h"
#include <iostream>
#include "sct_plots_internal.h"
#include "internal/plane.hh"
#include "S_Axis.h"
#include "s_treeCollection.h"
#include "treeCollection.h"
#include "internal/plane_hit.hh"
#include "s_DrawOption.h"

  S_plane::S_plane() : m_plane(nullptr), m_plane_def("error", 0)
  {
    std::cout << "[s_plane] unsupported default constructor do not use" << std::endl;
  }


  S_plane::S_plane(const S_plane_def& plane_def, S_treeCollection* hits) : S_plane(plane_def, hits->m_tree.get())
  {


  }

  S_plane::S_plane(const S_plane_def& plane_def, sct_corr::treeCollection* hits) : m_plane_def(plane_def)
  {

    m_plane = hits->m_rootBuffer.createPlane(m_plane_def.getID());


  }





  sct_corr::plane* S_plane::getPlane() const
  {
    return m_plane.get();
  }

  const char * S_plane::getName() const
  {
    return m_plane_def.getName();
  }
  Double_t S_plane::getID() const
  {
    return m_plane_def.getID();
  }

  bool S_plane::next()
  {

    return m_plane->next();
  }

 


 const sct_corr::axis_ref* S_plane::getX() const
  {
    if (!m_plane)
    {
      std::cout << "[s_plane] plane not set " << std::endl;
      return nullptr;
    }
    return m_plane->getAxis(x_axis_def);
  }

  const sct_corr::axis_ref* S_plane::getY() const
  {
    if (!m_plane)
    {
      std::cout << "[s_plane] plane not set " << std::endl;
      return nullptr;
    }
    return m_plane->getAxis(y_axis_def);
  }



  S_plane_def::S_plane_def(const char* name, Double_t ID) :m_name(name), m_ID(ID)
  {

  }
  Double_t S_plane_def::getID() const
  {
    return m_ID;
  }

  const char* S_plane_def::getName() const
  {
    return m_name.c_str();
  }


  S_Axis S_plane_def::getX_def() const
  {
    return S_Axis(getName(), getID(), x_axis_def);
  }
  S_Axis S_plane_def::getY_def() const
  {
    return S_Axis(getName(), getID(), y_axis_def);
  }

  s_plane_collection_correlations::s_plane_collection_correlations():s_plane_collection() {

  }

  void s_plane_collection_correlations::setResidual(const S_plane_def& plane_) {
    push_back("Residual", plane_);
  }

  void s_plane_collection_correlations::setResidualVsMissing(const S_plane_def& plane_) {
    push_back("ResidualVsMissing", plane_);
  }

  void s_plane_collection_correlations::setTotalTrueHits(const S_plane_def& plane_) {
    push_back("TotalTrueHits", plane_);
  }

  void s_plane_collection_correlations::setTrueHitsWithDUT(const S_plane_def& plane_) {
    push_back("TrueHitsWithDUT", plane_);
  }

  S_plane_def s_plane_collection_correlations::getResidual() const {
    return getByType("Residual")();
  }

  S_plane_def s_plane_collection_correlations::getResidualVSmissing() const {
    return getByType("ResidualVsMissing")();
  }

  S_plane_def s_plane_collection_correlations::getTotalTrueHits() const {
    return getByType("TotalTrueHits")();
  }

  S_plane_def s_plane_collection_correlations::getTrueHitsWithDUT() const {
    return getByType("TrueHitsWithDUT")();
  }

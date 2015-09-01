#include "S_Axis.h"
#include <iostream>
#include "s_plot_collection.h"


S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis), m_isValid(true)
{
    
}

S_Axis::S_Axis():m_isValid(false) {
  std::cout << "invalid axis" << std::endl;
}

S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis, std::weak_ptr<S_plot_collection> plot_collection_)
  : S_Axis(collctionName, planeID, axis) 
{
  m_plot_collection = plot_collection_;
}

std::shared_ptr<S_plot_collection> S_Axis::get_plot() const {
  if (std::shared_ptr<S_plot_collection> ret = m_plot_collection.lock()) {

    return ret;
  } else {
    std::cout << "plot is expired\n";
  }
  return nullptr;
}

void S_Axis::set_plot_collection(std::weak_ptr<S_plot_collection> plot_collection_) {
#ifdef _DEBUG
  if (m_plot_collection.lock())
  {
    std::cout << "m_plot_collection was already defined " << std::endl;
  }

#endif // _DEBUG

  m_plot_collection = plot_collection_;
}

bool S_Axis::isValid() const {
  return m_isValid;
}

const char* axis2String(axis_def ax)
{
    if (ax==x_axis_def)
    {
        return "x";
    }
    
    if (ax==y_axis_def)
    {
        return "y";
    }
    if (ax==Ndf_axis_def)
    {
      return "ndf";
    }
    if (ax == chi2_axis_def )
    {
        return "chi2";
    }
    if (ax == phi_axis_def)
    {
        return "phi";
    }
    if (ax == theta_axis_def)
    {
        return "theta";
    }
    if (ax==Occupancy_axis_def)
    {
        return "Occupancy";
    }
    if (ax==Occupancy_error_axis_def)//lb
    {
        return "Occupancy_error";
    }
    
    if (ax==NumOfEvents_axis_def)
    {
        return "NumOfEvents";
    }
    return "\0";
}

const char* getIDString()
{
    return "ID";
}

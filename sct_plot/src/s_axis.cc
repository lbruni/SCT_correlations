#include "s_axis.h"
#include <iostream>


S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis), m_isValid(true)
{
    
}

S_Axis::S_Axis():m_isValid(false) {
  std::cout << "invalid axis" << std::endl;
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

#include "s_axis.h"


S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis)
{

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

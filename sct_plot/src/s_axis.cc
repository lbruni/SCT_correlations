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

  if (ax=y_axis_def)
  {
    return "y";
  }
  return "\0";
}

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
    if (ax==Occupancy_error_axis_def)
    {
        return "Occupancy_error";
    }
    
    if (ax==NumOfEvents_axis_def)
    {
        return "NumOfEvents";
    }
    
    if (ax==x_axis_def_substrip)
    {
        return "x_substrip";
    }
    
    if (ax==y_axis_def_substrip)
    {
        return "y_substrip";
    }
    
    if (ax==Occupancy_axis_def_substrip)
    {
        return "Occupancy_substrip";
    }
    if (ax==Occupancy_error_axis_def_substrip)
    {
        return "Occupancy_error_substrip";
    }
    
    if (ax==NumOfEvents_axis_def_substrip)
    {
        return "NumOfEvents_substrip";
    }

    if (ax==NumOfEventsDUT_axis_def_substrip)
    {
        return "NumOfEventsDUT_substrip";
    }

    
    return "\0";
}

const char* getIDString()
{
    return "ID";
}

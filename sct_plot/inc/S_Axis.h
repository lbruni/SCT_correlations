#ifndef s_axis_h__
#define s_axis_h__
#include "internal/platform.hh"
#include "TString.h"

enum  axis_def
#ifndef __CINT__
:int
#endif // !__CINT__
{
    x_axis_def,
    y_axis_def,
    chi2_axis_def,
    phi_axis_def,
    theta_axis_def,
    Occupancy_axis_def,
    Occupancy_error_axis_def,
    NumOfEvents_axis_def,
    Occupancy_axis_def_substrip,
    Occupancy_error_axis_def_substrip,
    NumOfEvents_axis_def_substrip,
    NumOfEventsDUT_axis_def_substrip,
    x_axis_def_substrip,
    y_axis_def_substrip
};
const char* axis2String(axis_def ax);
const char* getIDString();
class DllExport S_Axis{
public:
    S_Axis(const char* collctionName, double planeID, axis_def axis);
    
    TString m_collectionName;
    double m_planeID;
    axis_def m_axis;
    ClassDef(S_Axis, 0);
};
#ifdef __CINT__
#pragma link C++ enum axis_def;

#pragma link C++ class S_Axis;
#endif // !__CINT__

#endif // s_axis_h__

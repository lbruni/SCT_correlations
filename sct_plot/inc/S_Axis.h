#ifndef s_axis_h__
#define s_axis_h__
#include "internal/platform.hh"
#include "TString.h"
#include <memory>

enum  axis_def
#ifndef __CINT__
  :int
#endif // !__CINT__
{
  x_axis_def,
  y_axis_def,
  chi2_axis_def,
  Ndf_axis_def,
  phi_axis_def,
  theta_axis_def,
  Occupancy_axis_def,
  Occupancy_error_axis_def,
  NumOfEvents_axis_def
};
const char* axis2String(axis_def ax);
const char* getIDString();

class S_plot_collection;
class DllExport S_Axis{
public:
  S_Axis(const char* collctionName, double planeID, axis_def axis);
#ifndef __CINT__
  S_Axis(const char* collctionName, double planeID, axis_def axis,std::weak_ptr<S_plot_collection> plot_collection_);
  std::shared_ptr<S_plot_collection> get_plot() const;
  void set_plot_collection(std::weak_ptr<S_plot_collection> plot_collection_);
#endif // !__CINT__

  S_Axis();
  bool isValid() const;
  std::string  m_collectionName;
  double m_planeID;
  axis_def m_axis;
private:
  std::weak_ptr<S_plot_collection> m_plot_collection;
  bool m_isValid;
  ClassDef(S_Axis, 0);
};
#ifdef __CINT__
#pragma link C++ enum axis_def;

#pragma link C++ class S_Axis;
#endif // !__CINT__

#endif // s_axis_h__

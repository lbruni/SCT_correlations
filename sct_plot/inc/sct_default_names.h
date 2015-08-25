#ifndef sct_default_names_h__
#define sct_default_names_h__
#include "internal/platform.hh"

class DllExport sct {
public:
  static const char* col_hit();
  static const char* col_zsdata_strip();
  static const char* col_zsdata_m26();
  static const char* col_zsdata_apix();
  static const char* col_fitpoints();
  static const char* col_fitpoints_local();
  static const char* col_local_hit();
  static const char* col_DUT_TTC();
  static const char* col_GBL_fitted_points();
};

#ifdef __CINT__
  #pragma link C++ class sct;
#endif

#endif // sct_default_names_h__

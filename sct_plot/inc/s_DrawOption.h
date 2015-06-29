#ifndef s_DrawOption_h__
#define s_DrawOption_h__

#include "internal/platform.hh"
#include "Rtypes.h"
#include "TCut.h"
#include <string>

class TTree;
class TObject;
class DllExport S_DrawOption{
public:

  S_DrawOption(const char* options, const char* cuts = "", const char * axis = "y:x");
  S_DrawOption(const char* options, TCut cuts, const char * axis = "y:x");
  S_DrawOption();
  S_DrawOption& options(const char* option);
  S_DrawOption& opt_colz();
  S_DrawOption& cut(const char* cut_);
  S_DrawOption& cut_add(const TCut& cut_);
  S_DrawOption& cut_x(Double_t min_, Double_t max_);
  S_DrawOption& cut_x_min(Double_t min_);
  S_DrawOption& cut_x_max(Double_t max_);
  S_DrawOption& cut_y(Double_t min_, Double_t max_);
  S_DrawOption& cut_y_min(Double_t min_);
  S_DrawOption& cut_y_max(Double_t max_);
  S_DrawOption& draw_axis(const char* axis_);
  S_DrawOption& draw_x();
  S_DrawOption& draw_y();
  S_DrawOption& draw_x_VS_y();
  S_DrawOption& draw_y_VS_x();
  S_DrawOption& output_object(TObject* out_);
  Long64_t Draw(TTree * tree) const;
  const char* getOptions() const;
  const char* getAxis() const;
  TCut getCut() const;
private:
#ifndef __CINT__

  std::string m_options = "colz", m_axis = "x:y";
  TCut m_cut;
  TObject* m_output_object = nullptr;
  mutable std::string m_axis_dummy;
#endif // !__CINT__

  ClassDef(S_DrawOption, 0);
};
#ifdef __CINT__ 
#pragma link C++ class S_DrawOption;
#endif
#endif // s_DrawOption_h__

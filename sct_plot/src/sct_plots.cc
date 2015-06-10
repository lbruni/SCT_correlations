#include "sct_plots.h"

#include "treeCollection.h"
#include <iostream>
#include "cluster.h"
#include "TCanvas.h"
#include "TVector.h"
#include "TMath.h"
#include "sct_plots_internal.h"
#include "TH2.h"
#include "TH2D.h"
#include "TAxis.h"













S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis)
{

}

S_DrawOption::S_DrawOption(const char* options /*= "colz"*/, const char* cuts /*= ""*/, const char * axis /*= "y:x"*/) : m_options(options), m_cut(cuts), m_axis(axis)
{

}


S_DrawOption::S_DrawOption()
{

}








S_DrawOption::S_DrawOption(const char* options, TCut cuts, const char * axis /*= "y:x"*/) : m_options(options), m_cut(cuts), m_axis(axis)
{

}

S_DrawOption& S_DrawOption::options(const char* option)
{

  m_options = option;
  return *this;
}

S_DrawOption& S_DrawOption::opt_colz()
{

  return options("colz");
}







S_DrawOption& S_DrawOption::cut(const char* cut_)
{
  m_cut = cut_;
  return *this;
}

S_DrawOption& S_DrawOption::cut_add(const TCut& cut_)
{
  m_cut = m_cut&& cut_;
  return *this;
}

S_DrawOption& S_DrawOption::cut_x(Double_t min_, Double_t max_)
{
  cut_x_min(min_);
  return cut_x_max(max_);
  
}

S_DrawOption& S_DrawOption::cut_x_min(Double_t min_)
{
  std::string dummy_str = "x>" + std::to_string(min_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::cut_x_max(Double_t max_)
{
  std::string dummy_str = "x<" + std::to_string(max_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::cut_y(Double_t min_, Double_t max_)
{
  cut_y_min(min_);
  return cut_y_max(max_);
}

S_DrawOption& S_DrawOption::cut_y_min(Double_t min_)
{
  std::string dummy_str = "y>" + std::to_string(min_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::cut_y_max(Double_t max_)
{
  std::string dummy_str = "y<" + std::to_string(max_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::draw_axis(const char* axis_)
{
  m_axis = axis_;
  return *this;
}

S_DrawOption& S_DrawOption::draw_x()
{
  
  return draw_axis("x");
}

S_DrawOption& S_DrawOption::draw_y()
{
  return draw_axis("y");
}

S_DrawOption& S_DrawOption::draw_x_VS_y()
{
  return draw_axis("x:y");
}

S_DrawOption& S_DrawOption::draw_y_VS_x()
{
  return draw_axis("y:x");
}

S_DrawOption& S_DrawOption::output_object(TObject* out_)
{
  m_output_object = out_;
  return *this;
}

const char* S_DrawOption::getOptions() const
{
  return m_options.c_str();
}

const char* S_DrawOption::getAxis() const
{


  m_axis_dummy = m_axis;
  if (m_output_object)
  {
    m_axis_dummy += ">>"+std::string(m_output_object->GetName());
  }
  return m_axis_dummy.c_str();
}

TCut S_DrawOption::getCut() const
{
  return m_cut;
}

void SCT_helpers::CutTH2(TH2* h, S_XCut x_cut, S_YCut y_cut, S_ZCut z_cut)
{
  bool set2zero_x =false,set2zero_y =false, set2zero_z = false;
  Double_t x = 0, y = 0,z=0;

  for (Int_t x_bin = 0; x_bin <= h->GetNbinsX(); ++x_bin)
  {
    set2zero_x= false;
    x = h->GetXaxis()->GetBinCenter(x_bin);
    if (x_cut.isOutOfRange(x))
    {
      set2zero_x = true;
    }
    
    for (Int_t y_bin = 0; y_bin <= h->GetNbinsY(); ++y_bin)
    {
      auto bin = h->GetBin(x_bin, y_bin);
      if (set2zero_x)
      {
        h->SetBinContent(bin, 0);
        continue;
      }
      set2zero_y = false;
      set2zero_z = false;
      y = h->GetYaxis()->GetBinCenter(y_bin);
      if (y_cut.isOutOfRange(y))
      {
        set2zero_y = true;
      }


      z = h->GetBinContent(bin);
      if (z_cut.isOutOfRange(z))
      {
        set2zero_z= true;
      }

      if (set2zero_y || set2zero_z)
      {
        h->SetBinContent(bin, 0);
      }
    }

  }
}

void SCT_helpers::CutTH2(TH2* h2, S_ZCut z)
{
  CutTH2(h2, S_XCut(), S_YCut(), z);
}

S_Cut::S_Cut(Double_t min_, Double_t max_) :m_min(min_), m_max(max_), m_cut_min(true), m_cut_max(true)
{

}

S_Cut::S_Cut(Double_t min_) : m_min(min_), m_max(0), m_cut_min(true), m_cut_max(false)
{

}

S_Cut::S_Cut() : m_min(0), m_max(0), m_cut_min(false), m_cut_max(false)
{

}

bool S_Cut::isOutOfRange(Double_t x)
{
  if (m_cut_min && m_min > x)
  {
    return true;
  }

  if (m_cut_max && m_max < x)
  {
    return true;
  }
  return false;
}

S_XCut::S_XCut(Double_t min_, Double_t max_) : S_Cut(min_, max_)
{

}

S_XCut::S_XCut(Double_t min_) : S_Cut(min_)
{

}

S_XCut::S_XCut()
{

}

S_YCut::S_YCut(Double_t min_, Double_t max_) : S_Cut(min_, max_)
{

}

S_YCut::S_YCut(Double_t min_) : S_Cut(min_)
{

}

S_YCut::S_YCut()
{

}

S_ZCut::S_ZCut(Double_t min_, Double_t max_) : S_Cut(min_, max_)
{

}

S_ZCut::S_ZCut(Double_t min_) : S_Cut(min_)
{

}

S_ZCut::S_ZCut()
{

}


 









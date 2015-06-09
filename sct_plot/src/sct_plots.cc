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

S_DrawOption::S_DrawOption(const char* options /*= "colz"*/, const char* cuts /*= ""*/, const char * axis /*= "y:x"*/) : m_options(options), m_cuts(cuts), m_axis(axis)
{

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



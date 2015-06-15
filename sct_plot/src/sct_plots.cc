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
#include "TH1.h"
#include "TTree.h"













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

Long64_t S_DrawOption::Draw(TTree * tree) const
{
  return tree->Draw(getAxis(), getCut(), getOptions());
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
    m_axis_dummy += ">>" + std::string(m_output_object->GetName());
  }
  return m_axis_dummy.c_str();
}

TCut S_DrawOption::getCut() const
{
  return m_cut;
}






void SCT_helpers::CutTH2(TH2* h, const S_Cut& cut_)
{

  Double_t x = 0, y = 0, binContent = 0;

  for (Int_t y_bin = 0; y_bin <= h->GetNbinsY(); ++y_bin)
  {
    for (Int_t x_bin = 0; x_bin <= h->GetNbinsX(); ++x_bin)
    {
      x = h->GetXaxis()->GetBinCenter(x_bin);
      auto bin = h->GetBin(x_bin, y_bin);
      y = h->GetYaxis()->GetBinCenter(y_bin);
      binContent = h->GetBinContent(bin);


      if (cut_.isOutOfRange(binContent, x, y))
      {
        h->SetBinContent(bin, 0);
      }
    }

  }
}

void SCT_helpers::CutTH1(TH1* h1, const S_Cut& cut_)
{
  Double_t x = 0,binContent = 0;


    for (Int_t x_bin = 0; x_bin <= h1->GetNbinsX(); ++x_bin)
    {
      x = h1->GetXaxis()->GetBinCenter(x_bin);
      auto bin = h1->GetBin(x_bin);
 
      binContent = h1->GetBinContent(bin);


      if (cut_.isOutOfRange(binContent, x))
      {
        h1->SetBinContent(bin, 0);
      }
    }


}

TH1* SCT_helpers::HistogrammSilhouette(TH2* h2, axis_def ax)
{

  if (ax==x_axis_def)
  {

    std::string title = std::string(h2->GetTitle()) + std::string("_sil_x");
    std::string name = std::string(h2->GetName()) + std::string("_sil_x");
    TH1* ret = new TH1D(name.c_str(), title.c_str(), h2->GetNbinsX(), h2->GetXaxis()->GetBinCenter(0), h2->GetXaxis()->GetBinCenter(h2->GetNbinsX()));


      for (Int_t x_bin = 0; x_bin <= h2->GetNbinsX(); ++x_bin)
      {
        Double_t max_binContent = 0;
        for (Int_t y_bin = 0; y_bin <= h2->GetNbinsY(); ++y_bin)
        {
          
         
        auto bin = h2->GetBin(x_bin, y_bin);
         max_binContent =std::max( h2->GetBinContent(bin) ,max_binContent);

        }

        ret->SetBinContent(x_bin, max_binContent);
      }

      return ret;
  }
  else if(ax==y_axis_def){

    std::string title = std::string(h2->GetTitle()) + std::string("_sil_y");
    std::string name = std::string(h2->GetName()) + std::string("_sil_y");
    TH1* ret = new TH1D(name.c_str(), title.c_str(), h2->GetNbinsY(), h2->GetYaxis()->GetBinCenter(0), h2->GetYaxis()->GetBinCenter(h2->GetNbinsY()));

    for (Int_t y_bin = 0; y_bin <= h2->GetNbinsY(); ++y_bin)
    {
      Double_t max_binContent = 0;
    for (Int_t x_bin = 0; x_bin <= h2->GetNbinsX(); ++x_bin)
    {
    



        auto bin = h2->GetBin(x_bin, y_bin);
        max_binContent = std::max(h2->GetBinContent(bin), max_binContent);

      }

      ret->SetBinContent(y_bin, max_binContent);
    }

    return ret;
  }
  std::cout << "[SCT_helpers::HistogrammSilhouette] Unknown axis \n";
  return 0;
}

S_XCut::S_XCut(Double_t min_, Double_t max_) :S_Cut_min_max(min_, max_)
{

}

S_XCut::S_XCut(Double_t min_) : S_Cut_min_max(min_)
{

}



bool S_XCut::isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const
{
  return isOutOfRange_intern(x);
}

bool S_XCut::isOutOfRange(Double_t BinContent) const
{
  return false;
}

bool S_XCut::isOutOfRange(Double_t BinContent, Double_t x) const
{
  return isOutOfRange_intern(x);
}

S_YCut::S_YCut(Double_t min_, Double_t max_) :S_Cut_min_max(min_, max_)
{

}

S_YCut::S_YCut(Double_t min_) : S_Cut_min_max(min_)
{

}


bool S_YCut::isOutOfRange(Double_t BinContent, Double_t x) const
{
  return false;
}

bool S_YCut::isOutOfRange(Double_t BinContent) const
{
  return false;
}

bool S_YCut::isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const
{
  return isOutOfRange_intern(y);
}

S_Cut_BinContent::S_Cut_BinContent(Double_t min_, Double_t max_) :S_Cut_min_max(min_, max_)
{

}

S_Cut_BinContent::S_Cut_BinContent(Double_t min_) : S_Cut_min_max(min_)
{

}











bool S_Cut_BinContent::isOutOfRange(Double_t BinContent, Double_t y, Double_t z) const
{
  return isOutOfRange_intern(BinContent);
}

bool S_Cut_BinContent::isOutOfRange(Double_t BinContent, Double_t y) const
{
  return isOutOfRange_intern(BinContent);
}

bool S_Cut_BinContent::isOutOfRange(Double_t BinContent) const
{
  return isOutOfRange_intern(BinContent);
}

S_Cut_min_max::S_Cut_min_max(Double_t min_, Double_t max_) :m_min(min_), m_max(max_), m_cut_min(true), m_cut_max(true)
{

}

S_Cut_min_max::S_Cut_min_max(Double_t min_) : m_min(min_), m_max(0), m_cut_min(true), m_cut_max(false)
{

}

bool S_Cut_min_max::isOutOfRange_intern(Double_t val) const
{
  if (m_cut_min && m_min >val)
  {
    return true;
  }

  if (m_cut_max && m_max <val)
  {
    return true;
  }
  return false;
}

S_CutCoollection::S_CutCoollection()
{

}

bool S_CutCoollection::isOutOfRange(Double_t BinContent, Double_t x, Double_t y) const
{
  for (auto&e : m_cuts)
  {
    if (e->isOutOfRange(BinContent, x, y))
    {
      return true;
    }
  }
  return false;
}

bool S_CutCoollection::isOutOfRange(Double_t BinContent, Double_t x) const
{
  for (auto&e : m_cuts)
  {
    if (e->isOutOfRange(BinContent, x))
    {
      return true;
    }
  }
  return false;
}

bool S_CutCoollection::isOutOfRange(Double_t BinContent) const
{
  for (auto&e : m_cuts)
  {
    if (e->isOutOfRange(BinContent))
    {
      return true;
    }
  }
  return false;
}

void S_CutCoollection::add_Cut(const S_Cut& cut_)
{
  auto coll = dynamic_cast<const S_CutCoollection*>(&cut_);
  if (coll)
  {
   return add_Cut(*coll);
  }
  m_cuts.push_back(cut_.copy());
}

void S_CutCoollection::add_Cut(const S_CutCoollection& cut_)
{


  m_cuts.insert(m_cuts.end(), cut_.m_cuts.begin(), cut_.m_cuts.end());


}

S_CutCoollection operator+(const S_Cut& cut_a, const S_Cut& cut_b)
{



  S_CutCoollection ret;

  ret.add_Cut(cut_a);
  ret.add_Cut(cut_b);
  return ret;
}

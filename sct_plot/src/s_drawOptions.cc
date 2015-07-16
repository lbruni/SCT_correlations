#include "s_DrawOption.h"
#include "TTree.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <iostream>
#include "TVirtualPad.h"

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







S_DrawOption& S_DrawOption::opt_star()
{
  return options("*");
}

S_DrawOption& S_DrawOption::opt_bar()
{
    return options("AP");
}


S_DrawOption& S_DrawOption::cut(const char* cut_)
{
  TCut dummy = cut_;
  m_cut =m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::cut(axis_def ax, Double_t min_, Double_t max_)
{
  cut_min(ax, min_);
  return cut_max(ax,max_);
}

S_DrawOption& S_DrawOption::cut_min(axis_def ax, Double_t min_)
{
  std::string dummy_str = std::string(axis2String(ax)) + ">" + std::to_string(min_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
  return *this;
}

S_DrawOption& S_DrawOption::cut_max(axis_def ax, Double_t max_)
{
  std::string dummy_str = std::string(axis2String(ax)) + "<" + std::to_string(max_);
  TCut dummy = dummy_str.c_str();
  m_cut = m_cut&& dummy;
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

S_DrawOption& S_DrawOption::draw_axis(axis_def ax)
{
  return draw_axis(axis2String(ax));
}

S_DrawOption& S_DrawOption::draw_axis(axis_def y, axis_def x)
{

  auto dummy = std::string(axis2String(y)) + std::string(":") + std::string(axis2String(x));
  return draw_axis( dummy.c_str() );
}

S_DrawOption& S_DrawOption::draw_axis(axis_def  x1, axis_def x2, axis_def x3)
{
  auto dummy = std::string(axis2String(x1)) + std::string(":") + std::string(axis2String(x2)) + std::string(":") + std::string(axis2String(x3));
  return draw_axis(dummy.c_str());
}

S_DrawOption& S_DrawOption::draw_x()
{
  m_numOfAxis = 1;
  return draw_axis("x");
}

S_DrawOption& S_DrawOption::draw_y()
{
  m_numOfAxis = 1;
  return draw_axis("y");
}

S_DrawOption& S_DrawOption::draw_x_VS_y()
{
  m_numOfAxis = 2;
  return draw_axis("x:y");
}

S_DrawOption& S_DrawOption::draw_y_VS_x()
{
  m_numOfAxis = 2;
  return draw_axis("y:x");
}

S_DrawOption& S_DrawOption::output_object(TObject* out_)
{
  m_output_object = out_;
  return *this;
}
////*********Set points to a TGraph
/*void S_DrawOption::push_output_to_TGraph(Long64_t numberOfPoints,TTree * tree) const
{
    if (m_output_object)
    {
        auto graph_ = dynamic_cast<TGraph*>(m_output_object);
        if (graph_)
        {
            if (m_numOfAxis == 1)
            {
                graph_->Set(0);
                TH1* h = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
                if (h)
                {
                    graph_->Set(0);
                    for (Int_t i = 0; i < h->GetNbinsX(); ++i)
                    {
                        graph_->SetPoint(i, h->GetBinCenter(i), h->GetBinContent(i));
                    }
                    graph_->SetEditable(false);
                }
            }
            else if (m_numOfAxis == 2)
            {
                graph_->Set(0);
                for (Int_t i = 0; i < numberOfPoints; ++i)
                {
                    graph_->SetPoint(i, tree->GetV2()[i], tree->GetV1()[i]);
                }
                graph_->SetEditable(false);
            }
        }
    }
}*/
////******** Set Points of a TGraphErrors
void S_DrawOption::push_output_to_TGraphErrors(Long64_t numberOfPoints,TTree * tree) const
{
    if (m_output_object)
    {
        auto graph_ = dynamic_cast<TGraphErrors*>(m_output_object);
        if (graph_)
        {
            if (m_numOfAxis == 1)
            {
                graph_->Set(0);
                TH1* h = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
                if (h)
                {
                    graph_->Set(0);
                    for (Int_t i = 0; i < h->GetNbinsX(); ++i)
                    {
                        graph_->SetPoint(i, h->GetBinCenter(i), h->GetBinContent(i));
                    }
                    graph_->SetEditable(false);
                }
            }
            else if (m_numOfAxis == 2)
            {
                graph_->Set(0);
                for (Int_t i = 0; i < numberOfPoints; ++i)
                {
                    graph_->SetPoint(i, tree->GetV2()[i], tree->GetV1()[i]);
                    graph_->SetPointError(i,0,tree->GetV3()[i]);//<-------------------
                }
                graph_->SetEditable(false);
            }
        }
    }
}
///////******Draw TGraphErrors / TGraph
Long64_t S_DrawOption::Draw(TTree * tree) const
{
    auto n = tree->Draw(getAxis(), getCut(), getOptions());
    
    //push_output_to_TGraph(n, tree);
    push_output_to_TGraphErrors(n, tree);
    return n;
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
        
        if ( dynamic_cast<TH1*>(m_output_object) )
        {
            m_axis_dummy += ">>" + std::string(m_output_object->GetName());
        }
    }
    return m_axis_dummy.c_str();
}

TCut S_DrawOption::getCut() const
{
  return m_cut;
}




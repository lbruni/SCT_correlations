#include "s_DrawOption.h"
#include "TTree.h"


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


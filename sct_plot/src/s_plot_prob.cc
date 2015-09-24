#include "s_plot_prob.h"
s_plot_prob::s_plot_prob()
{

}

s_plot_prob::s_plot_prob(const char* name) :m_name(name)
{

}

s_plot_prob& s_plot_prob::setName(const char* name)
{
  m_name = name;
  return *this;
}

s_plot_prob& s_plot_prob::setSaveOptione(plot_save_option_def save_)
{
  m_save = save_;
  return *this;
}

s_plot_prob& s_plot_prob::doNotSaveToDisk()
{
  return setSaveOptione(do_not_save_to_disk);
}

s_plot_prob& s_plot_prob::SaveToDisk()
{
  return setSaveOptione(save_to_disk);
}

const char* s_plot_prob::getName() const
{
  return m_name.c_str();
}

size_t  s_plot_prob::getNameSize() const
{
  return m_name.size();
}

plot_save_option_def s_plot_prob::getPlotSaveOption() const
{
  return m_save;
}

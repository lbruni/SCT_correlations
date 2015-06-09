#include "internal/plotsBase.hh"
#include <string>

int g_plot_count_intern = 0;
plot::plot(const char* name, bool save2disk) :m_name(name), m_save2disk(save2disk)
{
}

void plot::setParameter(const char* tag, const char * value)
{

}

s_plane_collection plot::getOutputcollection()
{
  s_plane_collection ret;
  return ret;
}

const char* plot::getName() const
{
  if (m_name.empty())
  {
       m_name = getType() + std::to_string(g_plot_count_intern++);
  }
  return m_name.c_str();
}

bool plot::getSave2disk() const
{
  return m_save2disk;
}


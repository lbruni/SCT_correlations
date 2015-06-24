#include "internal/plotsBase.hh"
#include <string>
namespace sct_corr{
  int g_plot_count_intern = 0;
  plot::plot(const s_plot_prob& plot_proberties) :m_prob(plot_proberties)
  {
    m_name = m_prob.getName();
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
    return m_prob.getPlotSaveOption()==save_to_disk;
  }

}
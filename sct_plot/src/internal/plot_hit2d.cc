#include "internal/plot_hit2d.hh"
#include "internal/plane.hh"
#include <iostream>

namespace sct_corr{
  plot_hit2d::plot_hit2d(const char* name, bool save2disk) : plot2d(name, save2disk)
  {

  }

  void plot_hit2d::processEventStart()
  {

  }

  void plot_hit2d::processEventEnd()
  {

  }

  void plot_hit2d::ProcessEvent()
  {
    processEventStart();
    while (m_y->next()){
      if (!m_x->next())
      {
        std::cout << "vector have different length" << std::endl;
        return;
      }
      //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
      processHit(m_x->get(), m_y->get());

    }

    if (m_x->next())
    {
      std::cout << "vector have different length" << std::endl;
    }
    processEventEnd();
  }
}
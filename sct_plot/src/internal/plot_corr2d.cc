#include "internal/plot_corr2d.hh"
#include "internal/plane.hh"


plot_corr2d::plot_corr2d(const char*name, bool save2disk) : plot2d(name,save2disk)
{

}

void plot_corr2d::processEventStart()
{

}

void plot_corr2d::processEventEnd()
{

}

void plot_corr2d::ProcessEvent()
{
  processEventStart();
  while (m_y->next()){
    while (m_x->next()){
      //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
      processHit(m_x->get(), m_y->get());
    }
  }
  processEventEnd();
}

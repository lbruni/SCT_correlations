#include "internal/plot_hit2d.hh"


class hitMultiplizity :public plot_hit2d{
public:
  hitMultiplizity(const S_plot_def& plot_def);

  virtual void processEventStart();
  virtual void processHit(double x, double y);;
  virtual void processEventEnd();

  Int_t m_counter = 0;
  virtual s_plane_collection getOutputcollection();
};

registerPlot(hitMultiplizity, sct::plot_hitMultiplizity());

hitMultiplizity::hitMultiplizity(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{

}

void hitMultiplizity::processEventStart()
{
  m_counter = 0;
}

void hitMultiplizity::processHit(double x, double y)
{
  ++m_counter;
}

void hitMultiplizity::processEventEnd()
{
  pushHit(m_counter, 0);
}

s_plane_collection hitMultiplizity::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("HitMultiplicity"), S_plane(getOutputName(), 0)));
  return ret;
}

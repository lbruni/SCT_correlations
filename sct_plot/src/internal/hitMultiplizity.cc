#include "internal/plot_hit2d.hh"


class hitMultiplizity :public plot_hit2d{
public:
  hitMultiplizity(const char* name, bool save2disk);

  virtual void processEventStart();
  virtual void processHit(double x, double y);;
  virtual void processEventEnd();

  Int_t m_counter = 0;
  virtual s_plane_collection getOutputcollection();

  virtual const char* getType() const override;
};




hitMultiplizity::hitMultiplizity(const char* name, bool save2disk) : plot_hit2d(name, save2disk)
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
  ret.m_planes.push_back(std::make_pair(std::string("HitMultiplicity"), S_plane_def(getOutputName(), 0)));
  return ret;
}

const char* hitMultiplizity::getType() const
{
  return sct::plot_hitMultiplizity();
}

S_plot sct_plot::s_hitMultiplizity(const char* name, bool save2disk /*= true*/)
{
  return S_plot(new hitMultiplizity(name, save2disk));
}

#include "internal/plot_corr2d.hh"
#include "treeCollection.h"


class residual : public plot_corr2d {
public:
  residual(const char* name, bool save2disk);
  virtual void processHit(double x, double y) override;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "x") override;

  virtual s_plane_collection getOutputcollection();

  virtual const char* getType() const override;
private:
  int m_index = 0;
};


residual::residual(const char* name, bool save2disk) : plot_corr2d(name, save2disk)
{

}

void residual::processHit(double x, double y)
{
  pushHit(x - y, ++m_index);
}

Long64_t residual::Draw(const char* options, const char* cuts /*= ""*/, const char* axis /*= "x"*/)
{
  return  m_outTree->Draw(axis, cuts, options);
}

s_plane_collection residual::getOutputcollection()
{
  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("residual"), S_plane_def(getOutputName(), 0)));
  return ret;
}

const char* residual::getType() const
{
  return sct::plot_residual();
}

S_plot sct_plot::s_residual(const char* name, bool save2disk)
{
  return S_plot(new residual(name, save2disk));
}

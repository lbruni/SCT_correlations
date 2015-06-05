#include "internal/plot_corr2d.hh"
#include "treeCollection.h"


class residual : public plot_corr2d {
public:
  residual(const S_plot_def& plot_def);
  virtual void processHit(double x, double y) override;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "x") override;

  virtual s_plane_collection getOutputcollection();
private:
  double m_cut = -1;
  int m_index = 0;
};

residual::residual(const S_plot_def& plot_def) :plot_corr2d(plot_def)
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
  ret.m_planes.push_back(std::make_pair(std::string("residual"), S_plane(getOutputName(), 0)));
  return ret;
}

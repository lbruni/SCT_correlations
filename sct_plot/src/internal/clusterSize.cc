
#include "internal/plot_hit2d.hh"
#include "cluster.h"


class clusterSize :public plot_hit2d{
public:
  static const char* pixelDistance_name();
  clusterSize(const S_plot_def& plot_def);
  virtual void processEventStart() override;

  virtual void processHit(double x, double y) override;
  virtual void processEventEnd() override;
  clusterMaker<Double_t> m_cl;
  virtual s_plane_collection getOutputcollection();
};
registerPlot(clusterSize, sct::plot_clusterSize());


const char* clusterSize::pixelDistance_name()
{
  return "pixelDistance___";
}

clusterSize::clusterSize(const S_plot_def& plot_def) :plot_hit2d(plot_def)
{
  m_cl = clusterMaker<Double_t>(atof(m_plot_def.getParameter(clusterSize::pixelDistance_name(), "2")));
}

void clusterSize::processEventStart()
{
  m_cl.reset();
}

void clusterSize::processHit(double x, double y)
{
  m_cl.push_pixel(pixelHit<Double_t>(x, y));
}

void clusterSize::processEventEnd()
{
  for (size_t i = 0; i < m_cl.NumOfCluster(); ++i){
    pushHit(m_cl.getCluster(i).getPos().m_x, m_cl.getCluster(i).getPos().m_y, 0);
    pushHit(m_cl.getCluster(i).m_hits.size(), 1);
  }
}

s_plane_collection clusterSize::getOutputcollection()
{

  s_plane_collection ret;
  ret.m_planes.push_back(std::make_pair(std::string("clusterPos"), S_plane(getOutputName(), 0)));
  ret.m_planes.push_back(std::make_pair(std::string("clusterSize"), S_plane(getOutputName(), 1)));
  return ret;
}

S_plot_def sct_plot::s_clustering(const char* name, Double_t Pixel_distance/*=2*/, bool save2disk /*=true*/)
{


  auto ret = S_plot_def(sct::plot_clusterSize(), name, save2disk);

  ret.setParameter(clusterSize::pixelDistance_name(), std::to_string(Pixel_distance));
  return ret;


}
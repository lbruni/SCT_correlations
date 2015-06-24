
#include "internal/plot_hit2d.hh"
#include "cluster.h"
namespace sct_corr{

  class clusterSize :public plot_hit2d{
  public:
    clusterSize(double clusterSize_, const s_plot_prob& = "");
    virtual void processEventStart() override;
    virtual const char* getType() const override;
    virtual void processHit(double x, double y) override;
    virtual void processEventEnd() override;
    clusterMaker<Double_t> m_cl;
    virtual s_plane_collection getOutputcollection();
  };



  clusterSize::clusterSize(double clusterSize_, const s_plot_prob& plot_prob) : plot_hit2d(plot_prob)
  {
    m_cl = clusterMaker<Double_t>(clusterSize_);
  }

  void clusterSize::processEventStart()
  {
    m_cl.reset();
  }

  const char* clusterSize::getType() const
  {
    return sct::plot_clusterSize();
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
    ret.m_planes.push_back(std::make_pair(std::string("clusterPos"), S_plane_def(getOutputName(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("clusterSize"), S_plane_def(getOutputName(), 1)));
    return ret;
  }
}
S_plot sct_plot::clustering(Double_t Pixel_distance/*=2*/, const s_plot_prob& plot_prob)
  {

    
    return S_plot(new sct_corr::clusterSize(Pixel_distance, plot_prob));
  }


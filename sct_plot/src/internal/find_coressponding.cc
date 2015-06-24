#include "internal/find_coressponding.hh"


namespace sct_corr{
  plot_find_correspondingX::plot_find_correspondingX(const s_plot_prob& plot_prob) : plotPlaneVsPlane(plot_prob)
  {

  }

  void plot_find_correspondingX::processHit(const plane_hit& p1, const plane_hit& p2)
  {
    if (TMath::Abs(p1.x - p2.x) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }
  }

  s_plane_collection plot_find_correspondingX::getOutputcollection()
  {
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("plot_find_correspondingX"), S_plane_def(getOutputName(), 0)));
    return ret;
  }




  const char* plot_find_correspondingX::getType() const
  {
    return sct::plot_find_correspondingX();
  }

  plot_find_correspondingXY::plot_find_correspondingXY(const s_plot_prob& plot_prob) : plotPlaneVsPlane(plot_prob)
  {

  }




  void plot_find_correspondingXY::processHit(const plane_hit&  p1, const plane_hit&  p2) {

    if (TMath::Abs(p1.x - p2.x) < 0.1 && TMath::Abs(p1.y - p2.y) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }

  }

  s_plane_collection plot_find_correspondingXY::getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("correspondingXY"), S_plane_def(getOutputName(), 0)));
    return ret;
  }



  const char* plot_find_correspondingXY::getType() const
  {
    return sct::plot_find_correspondingXY();
  }
}
S_plot sct_plot::find_correspondingX(const s_plot_prob& plot_prob)
 {
   return S_plot(new sct_corr::plot_find_correspondingX(plot_prob));
 }

S_plot sct_plot::find_correspondingXY(const s_plot_prob& plot_prob)
 {  
   return S_plot(new sct_corr::plot_find_correspondingXY(plot_prob));
 }

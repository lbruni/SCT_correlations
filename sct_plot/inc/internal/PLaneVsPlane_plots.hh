#ifndef PLaneVsPlane_plots_h__
#define PLaneVsPlane_plots_h__


#include "internal/plotsBase.hh"
#include <memory>

class treeCollection_ouput;


class plotPlaneVsPlane :public plot{
public:
  
  
  plotPlaneVsPlane(const char* name, bool save2disk);
  virtual bool isReady()override;
  virtual void pushAxis(axis_ref* axis) override;
  virtual void pushPlane(S_plane* axis) override;


  virtual void processEventStart();
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) = 0;
  virtual void processEventEnd();
  virtual void fill() override final;
  void pushHit(Double_t x, Double_t y);
  void pushHit(Double_t x, Double_t y, Double_t ID);
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override;
  virtual const char* getOutputName() const ;
  std::vector<double> m_x_points, m_y_points, m_id;
  
  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;


  S_plane *m_x =nullptr, *m_y=nullptr;

  Int_t m_size_x = 0, m_size_y = 0;
};


class plane_distance :public plotPlaneVsPlane {
public:
  plane_distance(const char* name, bool save2disk);
  virtual void processEventStart();
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2);
  static double hit_abs(const plane_hit& h);
  double m__id = 0, m_status = 0;
  virtual const char* getType() const override;
  virtual s_plane_collection getOutputcollection();
};


#endif // PLaneVsPlane_plots_h__

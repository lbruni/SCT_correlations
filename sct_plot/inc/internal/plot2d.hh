#ifndef plot2d_h__
#define plot2d_h__


#include "internal/plotsBase.hh"
class treeCollection_ouput;
class axis_ref;
class plot2d :public plot{

public:
  plot2d(const S_plot_def& plot_def);
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override;
  virtual void ProcessEvent() = 0;
  virtual void fill() override final;
  void pushHit(Double_t x, Double_t y);
  void pushHit(Double_t x, Double_t y, Double_t ID);

  virtual const char* getOutputName()  const;
protected:
  axis_ref* m_x;
  axis_ref* m_y;
  std::vector<double> m_x_points, m_y_points, m_id;

  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;
};
#endif // plot2d_h__

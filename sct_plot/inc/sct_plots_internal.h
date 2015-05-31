#ifndef sct_plots_internal_h__
#define sct_plots_internal_h__
#include "sct_plots.h"
#include "TMath.h"

plane_hit rotate(const  plane_hit& h, double Angle);


class axis_ref{
public:
  virtual bool next() = 0;
  virtual double get() const = 0;

};

class plot{
public:
  virtual void fill() = 0;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
  virtual void setParameter(const char* tag, const char * value){}
  virtual s_plane_collection getOutputcollection() { 
    s_plane_collection ret;
    return ret;
  }
};

plot* create_plot(const char* type, const char* name, axis_ref* x, axis_ref* y);

plot* create_plot(const char* type, const char* name, S_plane* x, S_plane* y);
plot* create_plot(const S_plot_def& plot_def, S_plane* x, S_plane* y);
plot* create_plot(const S_plot_def& plot_def, axis_ref* x, axis_ref* y);
class plane{
public:
  plane(double ID, treeCollection* hits);


  axis_ref* getX();
  axis_ref* getY();

private:
  class axis_vector :public axis_ref{
  public:
    axis_vector(std::vector<double>*  axis, std::vector<double>* ID, double planeID);
    virtual bool next() override;
    virtual double get() const override;

    std::vector<double>* m_axis;
    std::vector<double>* m_ID;
    double m_planeID = 0;
    Int_t m_curr = -1;
  };

  axis_vector m_x, m_y;
  double m_ID = 0;
  treeCollection* m_hits = nullptr;

};
#endif // sct_plots_internal_h__

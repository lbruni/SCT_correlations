#ifndef plane_h__
#define plane_h__

#include <vector>

#include "internal/axis_ref.hh"
#include "sct_plots.h"


namespace sct_corr{
  class treeCollection;
  class plane{
  public:
    plane(double ID, treeCollection* hits);


    axis_ref* getX();
    axis_ref* getY();
    bool next();
    plane_hit get() const;

  private:
    class axis_vector :public axis_ref{
    public:
      axis_vector(std::vector<double>*  axis, std::vector<double>* ID, double planeID);
      virtual bool next() override;
      virtual double get() const override;

      std::vector<double>* m_axis;
      std::vector<double>* m_ID;
      double m_planeID = 0;
      int m_curr = -1;
    };

    axis_vector m_x, m_y;
    double m_ID = 0;
    treeCollection* m_hits = nullptr;

  };

}
#endif // plane_h__

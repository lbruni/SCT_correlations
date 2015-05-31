#include "sct_plots.h"

#include "treeCollection.h"
#include <iostream>
#include "cluster.h"
#include "TCanvas.h"
#include "TVector.h"
#include "TMath.h"
#include "sct_plots_internal.h"


plane_hit rotate(const  plane_hit& h, double Angle){

  double x = h.x*TMath::Cos(Angle) - h.y*TMath::Sin(Angle);
  double y = h.x*TMath::Sin(Angle) + h.y*TMath::Cos(Angle);
  return plane_hit(x, y);
}





plane::plane(double ID, treeCollection* hits) :m_hits(hits), m_ID(ID), m_x(hits->m_buffer.m_x, hits->m_buffer.m_ID, ID), m_y(hits->m_buffer.m_y, hits->m_buffer.m_ID, ID)
{

}

axis_ref* plane::getX()
{
  return dynamic_cast<axis_ref*>(&m_x);
}

axis_ref* plane::getY()
{
  return dynamic_cast<axis_ref*>(&m_y);
}

plane::axis_vector::axis_vector(std::vector<double>* axis, std::vector<double>* ID, double planeID) :m_axis(axis), m_ID(ID), m_planeID(planeID)
{

}

bool plane::axis_vector::next()
{
  do{
    if (++m_curr >= m_ID->size())
    {
      m_curr = -1;
      return false;
    }

  } while ((m_ID->at(m_curr) != m_planeID));

  return true;
}

double plane::axis_vector::get() const
{
  return m_axis->at(m_curr);
}




class plotPlaneVsPlane :public plot{
public:
  plotPlaneVsPlane(const  S_plot_def& plot_def, S_plane* x, S_plane* y) :m_plot_def(plot_def),m_x(x), m_y(y){
    m_outTree = std::make_shared<treeCollection_ouput>(plot_def.m_name.c_str(), &m_x_points, &m_y_points, &m_id, &m_current,plot_def.m_save2disk);

  }

  virtual void processEventStart() {}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) = 0;
  virtual void processEventEnd() {}
  virtual void fill() override final{
    m_size_x = 0, m_size_y = 0;
    m_x_points.clear();
    m_y_points.clear();
    m_id.clear();

    bool first = true;
    processEventStart();
    while (m_y->next()){
      ++m_size_x;
      while (m_x->next()){
        if (first)
        {
          ++m_size_y;
        }
        processHit(m_x->get(), m_y->get());
      }
      first = false;
    }
    processEventEnd();
    m_outTree->fill();
    ++m_current;
  }
  inline void pushHit(Double_t x, Double_t y){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(0);
    
  }
  inline void pushHit(Double_t x, Double_t y,Double_t ID){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(ID);

  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    return m_outTree->Draw(axis, cuts, options);
  }

  std::vector<double> m_x_points, m_y_points, m_id;

  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;


  S_plane *m_x, *m_y;
  S_plot_def m_plot_def;
  Int_t m_size_x = 0, m_size_y = 0;
};

class plane_distance :public plotPlaneVsPlane {
public:
  plane_distance(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processEventStart() {
  
    m__id = 0;
    m_status = 0;
  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {
    

    if (m__id==0)
    {
    
    if (hit_abs(p2) < 0.1){
      if (abs(p1.y)<0.2)
      {
        m_status = 0;
      }
      else
      {
        m_status = 1;
      }
    }
    else{
      if (abs(p1.y) < 0.2)
      {
        m_status = 10;
      }
      else
      {
        m_status = 11;
        
      }
    }
    }
    pushHit(p1.x, p1.y, m__id*100 + m_status);
      ++m__id;
    //pushHit(hit_abs(p1), hit_abs(p2));
  }
  static double hit_abs(const plane_hit& h){

    return sqrt(h.x*h.x + h.y * h.y);
  }
  double m__id = 0 , m_status =0;

  virtual s_plane_collection getOutputcollection() {
  
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("A_and_b"),S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};
class plot_find_correspondingX :public plotPlaneVsPlane{
public:
  plot_find_correspondingX(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {



    if (abs(p1.x - p2.x) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }

  }
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("plot_find_correspondingX"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};
class plot_Event_size :public plotPlaneVsPlane{
public:
  plot_Event_size(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processEventEnd() {

    pushHit(m_size_x, m_size_y);

  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {};

  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("Event_size"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};
class plot_find_correspondingXY :public plotPlaneVsPlane{
public:
  plot_find_correspondingXY(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {

    if (abs(p1.x - p2.x) < 0.1 && abs(p1.y - p2.y) < 0.1)
    {
      pushHit(p1.x, p1.y);
    }

  }
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("correspondingXY"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};

class plot_a_if_b_has_a_hit :public plotPlaneVsPlane{
public:
  plot_a_if_b_has_a_hit(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processEventStart() {
    int i = 0;
  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) override {

    pushHit(p1.x, p1.y, 0);
    pushHit(p2.x, p2.y, 1);
  }
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_A"), S_plane(m_outTree->m_name.c_str(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("A_if_B_has_a_hit_get_B"), S_plane(m_outTree->m_name.c_str(), 1)));
    return ret;
  }
};
class find_nearest_strip :public plotPlaneVsPlane{
public:
  find_nearest_strip(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){
    try {
      auto ax = atoi(m_plot_def.getParameter(std::string(axis_name()), std::string("0")).c_str());
      if (ax==x_axis_def )
      {
        m_axis = x_axis_def;
      }
      else if (ax == y_axis_def){
        m_axis = y_axis_def;

      }
      else
      {
        std::cout << "[find_nearest_strip] unable to convert to axis def" << std::endl;
      }
    }
    catch (...){
      std::cout << "[find_nearest_strip] unable to convert" << std::endl;
    }

  }
  static  const char* axis_name(){ return "axis____"; }
  virtual void processEventStart() {
    m_hit1.clear();
    m_hit2.clear();
    m_dist.clear();
  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {


    m_dist.emplace_back((p1.x - p2.x), (p1.y - p2.y));
    m_hit1.emplace_back(p1.x, p1.y);
    m_hit2.emplace_back(p2.x, p2.y);
  }

  virtual void processEventEnd() {
    const double no_hit = 10000000000;
    double r = no_hit;
    plane_hit dist(0, 0), h1(0, 0), h2(0, 0);

    for (size_t i = 0; i < m_dist.size(); ++i)
    {
      auto e = m_dist.at(i);
      double r1 = 0;
      if (m_axis == x_axis_def)
      {
        r1 = abs(e.x);
      }
      else if (m_axis == y_axis_def) {
        r1 = abs(e.y);
      }
      else{
        std::cout << "unknown axis" << std::endl;
      }
      if (r1 > 0 && r1 < r)
      {
        r = r1;
        dist = e;
        h1 = m_hit1.at(i);
        h2 = m_hit2.at(i);
      }

    }

    if (r < no_hit)
    {
      pushHit(dist.x, dist.y,0);
      pushHit(h1.x, h1.y,1);
      pushHit(h2.x, h2.y,2);
    }
  }
  std::vector<plane_hit> m_dist,m_hit1, m_hit2;
  axis_def m_axis;
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_distance"), S_plane(m_outTree->m_name.c_str(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_plane1"), S_plane(m_outTree->m_name.c_str(), 1)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_strip_plane2"), S_plane(m_outTree->m_name.c_str(), 2)));
    return ret;
  }

};
class find_nearest :public plotPlaneVsPlane{
public:
  find_nearest(const  S_plot_def& plot_def , S_plane* x, S_plane* y) : plotPlaneVsPlane(plot_def, x, y){}
  virtual void processEventStart() {
    m_hit1.clear();
    m_hit2.clear();
    m_dist.clear();
  }
  virtual void processHit(const plane_hit&  p1, const plane_hit&  p2) {


    m_dist.emplace_back((p1.x - p2.x), (p1.y - p2.y));
    m_hit1.emplace_back(p1.x, p1.y);
    m_hit2.emplace_back(p2.x, p2.y);
  }

  virtual void processEventEnd() {

    double r = 10000000000000;
    plane_hit dist(0, 0),h1(0,0),h2(0,0);

    for (size_t i = 0; i < m_dist.size();++i)
    {
      auto e = m_dist.at(i);
      auto r1 = sqrt((e.x)*(e.x) + (e.y)*(e.y));

      if (r1 > 0 && r1 < r)
      {
        r = r1;
        dist = e;
        h1 = m_hit1.at(i);
        h2 = m_hit2.at(i);
      }

    }

    if (r < 100000000000)
    {
      pushHit(dist.x, dist.y,0);
      pushHit(h1.x, h1.y,1);
      pushHit(h2.x, h1.y,2);
    }
  }
  std::vector<plane_hit> m_dist, m_hit1, m_hit2;

  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("nearest_distance"), S_plane(m_outTree->m_name.c_str(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_plane1"), S_plane(m_outTree->m_name.c_str(), 1)));
    ret.m_planes.push_back(std::make_pair(std::string("nearest_plane2"), S_plane(m_outTree->m_name.c_str(), 2)));
    return ret;
  }
};
class plot2d :public plot{

public:
  plot2d(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :m_x(x), m_y(y), m_plot_def(plot_def){
   
    m_outTree = std::make_shared<treeCollection_ouput>(plot_def.m_name.c_str(), &m_x_points, &m_y_points, &m_id, &m_current,plot_def.m_save2disk);
  
  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") override{
    return m_outTree->Draw(axis, cuts, options);
  }
  virtual void ProcessEvent() = 0;
  virtual void fill() override final{
    m_x_points.clear();
    m_y_points.clear();
    m_id.clear();
    

    ProcessEvent();

    m_outTree->fill();
    ++m_current;
  }
  inline void pushHit(Double_t x, Double_t y){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(0);

  
  }
  inline void pushHit(Double_t x, Double_t y ,Double_t ID){
    m_x_points.push_back(x);
    m_y_points.push_back(y);
    m_id.push_back(ID);


  }
protected:
  axis_ref* m_x;
  axis_ref* m_y;
  std::vector<double> m_x_points, m_y_points, m_id;
  S_plot_def m_plot_def;
  std::shared_ptr<treeCollection_ouput> m_outTree;
  Int_t m_current = 0;
};
class plot_corr2d :public plot2d{
public:
  plot_corr2d(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot2d(plot_def, x, y){}
  virtual void processEventStart() {}
  virtual void processHit(double x, double y) = 0;
  virtual void processEventEnd() {}
  virtual void ProcessEvent() override final{
    processEventStart();
    while (m_y->next()){
      while (m_x->next()){
        //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
        processHit(m_x->get(), m_y->get());
      }
    }
    processEventEnd();
  }
};
class plot_hit2d :public plot2d{
public:
  plot_hit2d(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot2d(plot_def, x, y){}
  virtual void processEventStart() {}
  virtual void processHit(double x, double y) = 0;
  virtual void processEventEnd() {}
  virtual void ProcessEvent() override final{
    processEventStart();
    while (m_y->next()){
      if (!m_x->next())
      {
        std::cout << "vector have different length" << std::endl;
        return;
      }
      //      std::cout << m_x->get()<< "  "<< m_y->get() << std::endl;
      processHit(m_x->get(), m_y->get());

    }

    if (m_x->next())
    {
      std::cout << "vector have different length" << std::endl;
    }
    processEventEnd();
  }
};
class rotated_plane: public plot_hit2d {
public:
  static const char* Angle_name(){ return "ANGLE___"; }
  rotated_plane(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_hit2d(plot_def, x, y){
    std::string a;
    try {

      a = m_plot_def.getParameter(std::string(Angle_name()), std::string("0"));
   
      angele = atof(a.c_str());
    }
    catch (...){
      std::cout << "unable to convert \"" << a << "\"" << std::endl;
    }

  }
  virtual void processHit(double x, double y) override{
    auto h = rotate(plane_hit(x, y), angele);
    pushHit(h.x,h.y);
  }
  double angele=0;

  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("rotated"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};

class coordinate_transform : public plot_hit2d {
public:
  static const char* x_slope_name(){ return "x_slope___"; }
  static const char* y_slope_name(){ return "y_slope___"; }
  static const char* x_offset_name(){ return "x_offset_"; }
  static const char* y_offset_name(){ return "y_offset_"; }
  coordinate_transform(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_hit2d(plot_def, x, y){
    try {

      m_x_offset = atof(m_plot_def.getParameter(std::string(x_offset_name()), std::string("0")).c_str());
      m_y_offset = atof(m_plot_def.getParameter(std::string(y_offset_name()), std::string("0")).c_str());
      m_x_slope= atof(m_plot_def.getParameter(std::string(x_slope_name()), std::string("0")).c_str());
      m_y_slope = atof(m_plot_def.getParameter(std::string(y_slope_name()), std::string("0")).c_str());
    }
    catch (...){
      std::cout << "[coordinate_transform] unable to convert" <<std::endl;
    }

  }
  virtual void processHit(double x, double y) override{

    
    pushHit( x*m_x_slope +m_x_offset, y*m_y_slope + m_y_offset);
  }
  double m_x_slope = 0;
  double m_y_slope = 0;
  double m_x_offset = 0;
  double m_y_offset = 0;

  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("coordinate_transform"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};


class hitmap :public plot_hit2d{
public:
  hitmap(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_hit2d(plot_def, x, y){}
  virtual void processHit(double x, double y) override{
    pushHit(m_x->get(), m_y->get());
  }
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("hitmap"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};
class ProjectOnPixel :public plot_hit2d{
public:
  ProjectOnPixel(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_hit2d(plot_def, x, y){}

  virtual void processHit(double x, double y) override{
    pushHit((int)(m_x->get() / m_x_pixelsize * 100) % 100, (int)(m_y->get() / m_ypixelsize * 100) % 100);
  }

  double m_x_pixelsize = 0.074, m_ypixelsize = 100000;
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("ProjectOnPixel"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};
class clusterSize :public plot_hit2d{
public:
  static const char* pixelDistance_name(){ return "pixelDistance___"; }
  clusterSize(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_hit2d(plot_def, x, y) {
    m_cl = clusterMaker<Double_t>(atof(m_plot_def.getParameter(clusterSize::pixelDistance_name(), "2")));
  }
  virtual void processEventStart() override {
    m_cl.reset();
  }

  virtual void processHit(double x, double y) override{

    m_cl.push_pixel(pixelHit<Double_t>(x, y));
  }
  virtual void processEventEnd() override {


    for (size_t i = 0; i < m_cl.NumOfCluster(); ++i){
      pushHit(m_cl.getCluster(i).getPos().m_x, m_cl.getCluster(i).getPos().m_y, 0);
      pushHit(m_cl.getCluster(i).m_hits.size(), 1);
    }
  }
  clusterMaker<Double_t> m_cl;
  virtual s_plane_collection getOutputcollection() {

    
    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("clusterPos"), S_plane(m_outTree->m_name.c_str(), 0)));
    ret.m_planes.push_back(std::make_pair(std::string("clusterSize"), S_plane(m_outTree->m_name.c_str(), 1)));
    return ret;
  }
};
class correlations :public plot_corr2d{
public:
  correlations(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_corr2d(plot_def, x, y){}
  
#ifdef _DEBUG
  virtual void processEventStart() override {
    int BreakPoint = 0;
  }

#endif // _DEBUG

  virtual void processHit(double x, double y) override{
    pushHit(x, y);
  }
  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("correlations"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
};

class residual : public plot_corr2d {
public:
  residual(const S_plot_def& plot_def, axis_ref* x, axis_ref* y) :plot_corr2d(plot_def, x, y){}
  virtual void processHit(double x, double y) override{
    pushHit(x - y, ++m_index);
  }
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "x") override{
    return  m_outTree->Draw(axis, cuts, options);
  }

  virtual s_plane_collection getOutputcollection() {

    s_plane_collection ret;
    ret.m_planes.push_back(std::make_pair(std::string("residual"), S_plane(m_outTree->m_name.c_str(), 0)));
    return ret;
  }
private:
  double m_cut = -1;
  int m_index = 0;
};

plot* create_plot(const S_plot_def& plot_def, axis_ref* x, axis_ref* y){

  std::string s_type(plot_def.m_type);

  if (s_type.compare(sct::plot_hitmap()) == 0)
  {
    return dynamic_cast<plot*>(new hitmap(plot_def, x, y));

  }

  if (s_type.compare(sct::plot_correlation()) == 0)
  {
    return dynamic_cast<plot*>(new correlations(plot_def, x, y));
  }


  if (s_type.compare(sct::plot_residual()) == 0)
  {
    return dynamic_cast<plot*>(new residual(plot_def, x, y));
  }

  if (s_type.compare(sct::plot_clusterSize()) == 0)
  {
    return dynamic_cast<plot*> (new clusterSize(plot_def, x, y));
  }
  if (s_type.compare(sct::plot_projectOnPixel()) == 0)
  {
    return dynamic_cast<plot*>(new ProjectOnPixel(plot_def, x, y));
  }

  if (s_type.compare(sct::plot_rotated()) == 0)
  {

    return dynamic_cast<plot*>(new  rotated_plane(plot_def, x, y));

  }
  if (s_type.compare(sct::plot_coordinate_transform()) == 0)
  {

    return dynamic_cast<plot*>(new  coordinate_transform(plot_def, x, y));

  }
  std::cout << "cant find plot type : \"" << s_type << "\"" << std::endl;
  return nullptr;
}

plot* create_plot(const char* type, const char* name, axis_ref* x, axis_ref* y)
{
  return create_plot(S_plot_def(type, name), x, y);
}

plot* create_plot(const S_plot_def& plot_def, S_plane* x, S_plane* y){
  std::string s_type(plot_def.m_type);
  if (s_type.compare(sct::plot_find_correspondingX()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_find_correspondingX(plot_def, x, y));

  }

  if (s_type.compare(sct::plot_find_correspondingXY()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_find_correspondingXY(plot_def, x, y));

  }


  if (s_type.compare(sct::plot_Event_size()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_Event_size(plot_def, x, y));

  }

  if (s_type.compare(sct::plot_find_nearest()) == 0)
  {

    return dynamic_cast<plot*>(new  find_nearest(plot_def, x, y));

  }


  if (s_type.compare(sct::plot_plane_distance()) == 0)
  {

    return dynamic_cast<plot*>(new  plane_distance(plot_def, x, y));

  }

  if (s_type.compare(sct::plot_find_nearest_strip()) == 0)
  {

    return dynamic_cast<plot*>(new  find_nearest_strip(plot_def, x, y));

  }


  if (s_type.compare(sct::plot_A_if_B()) == 0)
  {

    return dynamic_cast<plot*>(new  plot_a_if_b_has_a_hit(plot_def, x, y));

  }


  std::cout << "cant find plot type : \"" << s_type << "\"" << std::endl;
  return nullptr;
}
plot* create_plot(const char* type, const char* name, S_plane* x, S_plane* y){

  return create_plot(S_plot_def(type, name), x, y);
}

S_Axis::S_Axis(const char* collctionName, double planeID, axis_def axis) :m_collectionName(collctionName), m_planeID(planeID), m_axis(axis)
{

}

S_DrawOption::S_DrawOption(const char* options /*= "colz"*/, const char* cuts /*= ""*/, const char * axis /*= "y:x"*/) : m_options(options), m_cuts(cuts), m_axis(axis)
{

}


S_plot_def sct_plot::s_rotated(const char* name, Double_t angle,  bool save2disk)
{
  auto ret = S_plot_def(sct::plot_rotated(), name,save2disk);
  ret.setParameter(rotated_plane::Angle_name(), std::to_string(angle));

  return ret;
}




S_plot_def sct_plot::s_coordinate_transform(const char* name, Double_t x_slope, Double_t x_offset, Double_t y_slope, Double_t y_offset, bool save2disk /*= true*/)
{
  auto ret = S_plot_def(sct::plot_coordinate_transform(), name, save2disk);
  ret.setParameter(coordinate_transform::x_offset_name(), std::to_string(x_offset));
  ret.setParameter(coordinate_transform::y_offset_name(), std::to_string(y_offset));
  ret.setParameter(coordinate_transform::x_slope_name(), std::to_string(x_slope));
  ret.setParameter(coordinate_transform::y_slope_name(), std::to_string(y_slope));
  return ret;
}

const char* sct::plot_coordinate_transform()
{
  return "coordinate_transform____";
}

S_plot_def sct_plot::s_clustering(const char* name, Double_t Pixel_distance/*=2*/, bool save2disk /*=true*/)
{


   auto ret= S_plot_def(sct::plot_clusterSize(), name, save2disk);

   ret.setParameter(clusterSize::pixelDistance_name(), std::to_string(Pixel_distance));
   return ret;
  

}

S_plot_def sct_plot::s_find_nearest_strip(const char* name, axis_def search_axis, bool save2disk)
{

  auto ret = S_plot_def(sct::plot_find_nearest_strip(), name, save2disk);
  ret.setParameter(find_nearest_strip::axis_name(), std::to_string(search_axis));
  return ret;
}
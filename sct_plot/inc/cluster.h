#ifndef cluster_h__
#define cluster_h__

template <typename T>
class pixelHit{
public:
  pixelHit(T x, T y) :m_x(x), m_y(y){}

  T m_x, m_y;
};

template <typename  T>
T hitDistance(const pixelHit<T>& h1, const pixelHit<T>& h2){
  T xdiff = (h1.m_x - h2.m_x);
  T ydiff = (h1.m_y - h2.m_y);
  T dif = sqrt(xdiff*xdiff + ydiff*ydiff);
  return dif;
}


template <typename T>
class Cluster{
public:
  using pixelHit_t = pixelHit < T > ;
  Cluster(const pixelHit_t& h){
    m_hits.push_back(h);
  }
  bool pushHit(const pixelHit_t& h){

    for (auto& e : m_hits){
      if (hitDistance(h, e) < 2)
      {
        m_hits.push_back(h);
        return true;
      }
    }
    return false;
  }
  const  pixelHit_t & getPos(){
    return m_hits.front();
  }
  std::vector<pixelHit_t> m_hits;
};



template <typename T>
class clusterMaker{
public:
  using cluster_t = Cluster < T > ;
  void reset(){
    m_cluster.clear();
  }
  size_t NumOfCluster(){
    return m_cluster.size();

  }

  cluster_t& getCluster(size_t i){
    return m_cluster.at(i);
  }


  size_t biggestCluster(){
    size_t s = 0;
    for (auto& e : m_cluster)
    {
      if (e.m_hits.size() > s)
      {
        s = e.m_hits.size();
      }
    }
    return s;
  }
  size_t smalestCluster(){
    size_t s = (size_t)-1;
    for (auto& e : m_cluster)
    {
      if (e.m_hits.size() < s)
      {
        s = e.m_hits.size();
      }
    }
    return s;
  }
  void push_pixel(const pixelHit<T>& h){
    bool clusterFound = false;
    for (auto& e : m_cluster)
    {

      if (e.pushHit(h))
      {
        clusterFound = true;
      }

    }

    if (!clusterFound)
    {
      m_cluster.emplace_back(h);
    }
  }




  std::vector<cluster_t> m_cluster;
};

#endif // cluster_h__

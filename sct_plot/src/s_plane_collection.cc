#include "sct_plots.h"
#include <iostream>


S_plane_def s_plane_collection::get(Int_t i) const
{
  if (i< static_cast<Int_t>(m_planes.size()))
  {
    return m_planes[i].second;
  }
  std::cout << "[s_plane_collection] out of range" << std::endl;
  return S_plane_def("error",0);
}


S_plane_def s_plane_collection::get(const char* name) const
{
  for (auto& e: m_planes)
  {
    if (e.first== name) {
      return e.second;
    }
    
  }
  std::cout << "[s_plane_collection] unknown name = \"" << name << "\"" << std::endl;
  return S_plane_def("error", 0);

}
S_plane_def s_plane_collection::operator()() const
{
  return get(0);
}
s_plane_collection::s_plane_collection(const S_plane_def& plane_)
{
  push_back(plane_);
}

const char* s_plane_collection::getName(Int_t i) const
{
  if (i< static_cast<Int_t>(m_planes.size()))
  {
    return m_planes.at(i).first.c_str();
  }
  return "collection not found";
}


void s_plane_collection::showNames() const
{
  for (auto& e: m_planes)
  {
    std::cout << e.first<< std::endl;
  }
}
Int_t s_plane_collection::size() const
{
  return m_planes.size();
}

void s_plane_collection::push_back(const char* name, const S_plane_def& pl)
{
  m_planes.push_back(std::make_pair(std::string(name), pl));
}

void s_plane_collection::push_back(const S_plane_def& pl)
{
  push_back(pl.getName(), pl);
}






 s_plane_collection operator+(s_plane_collection pl1, const s_plane_collection& pl2)
{
  pl1.m_planes.insert(pl1.m_planes.end(), pl2.m_planes.begin(), pl2.m_planes.end());


  return pl1;
}

  s_plane_collection operator+(s_plane_collection pl1, const S_plane_def& pl2)
 {
   pl1.push_back(pl2);
   return pl1;
 }






 s_plane_collection operator+(const S_plane_def& pl1, const S_plane_def& pl2)
  {
    s_plane_collection ret;
    ret.push_back(pl1);
    ret.push_back(pl2);
    return ret;
  }


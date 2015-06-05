#include "sct_plots.h"
#include <iostream>


S_plane s_plane_collection::get(Int_t i)
{
  if (i< static_cast<Int_t>(m_planes.size()))
  {
    return m_planes[i].second;
  }
  std::cout << "[s_plane_collection] out of range" << std::endl;
  return S_plane();
}


S_plane s_plane_collection::get(const char* name)
{
  for (auto& e: m_planes)
  {
    if (e.first== name) {
      return e.second;
    }
    
  }
  std::cout << "[s_plane_collection] unknown name = \"" << name << "\"" << std::endl;
  return S_plane();

}
S_plane s_plane_collection::operator()()
{
  return get(0);
}

const char* s_plane_collection::getName(Int_t i)
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

void s_plane_collection::push_back(const char* name, const S_plane& pl)
{
  m_planes.push_back(std::make_pair(std::string(name), pl));
}

void s_plane_collection::push_back(const S_plane& pl)
{
  push_back(pl.getName(), pl);
}

s_plane_collection& operator+(s_plane_collection& pl1, const s_plane_collection& pl2)
{
  pl1.m_planes.insert(pl1.m_planes.end(), pl2.m_planes.begin(), pl2.m_planes.end());


  return pl1;


}

s_plane_collection operator+(const s_plane_collection& pl1, const s_plane_collection& pl2)
{
  s_plane_collection ret;
  ret.m_planes.insert(ret.m_planes.end(), pl1.m_planes.begin(), pl1.m_planes.end());
  ret.m_planes.insert(ret.m_planes.end(), pl2.m_planes.begin(), pl2.m_planes.end());
  return ret;
}



s_plane_collection& operator+(s_plane_collection& pl1, const S_plane& pl2)
{

  
  pl1.push_back(pl2);
  return pl1;
}
s_plane_collection operator+(const s_plane_collection& pl1, const S_plane& pl2)
{
  s_plane_collection ret;
  ret.m_planes.insert(ret.m_planes.end(), pl1.m_planes.begin(), pl1.m_planes.end());
  ret.push_back(pl2);
  return ret;
}
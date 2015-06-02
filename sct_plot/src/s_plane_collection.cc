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


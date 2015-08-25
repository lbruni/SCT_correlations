
#include <iostream>
#include "s_plane_def.h"
#include "s_DrawOption.h"
#include "s_plane_def.h"
// class output_streamer;
// 
// class output_lock{
// public:
//   output_lock(output_streamer& out_streamer_collection, std::ostream* local_outstreamer);
//   ~output_lock();
//   std::ostream* m_local_outstreamer = nullptr;
//   output_streamer& m_out_streamer_collection;
// };
// 
// std::ostream* out=nullptr;
// class output_streamer{
// public:
//   output_streamer(){ m_out.push_back(&(std::cout));
//   out = &(std::cout);
//   }
//   
//   output_lock lock(std::ostream* local_outstreamer);
//   std::vector<std::ostream*> m_out;
// } out_collection;
// 
// 
// output_lock::output_lock(output_streamer& out_streamer_collection, std::ostream* local_outstreamer) :m_local_outstreamer(local_outstreamer), m_out_streamer_collection(out_streamer_collection)
// {
//   m_out_streamer_collection.m_out.push_back(m_local_outstreamer);
// }
// 
// output_lock::~output_lock()
// {
//   if (m_out_streamer_collection.m_out.back() == m_local_outstreamer){
//     m_out_streamer_collection.m_out.pop_back();
//   }
//   else
//   {
//     std::cout << "stream collection mixed up" << std::endl;
// 
//   }
// 
// }

S_plane_def s_plane_collection::get(Int_t i) const
{
  if (i< static_cast<Int_t>(m_planes.size()))
  {
    return m_planes[i].second;
  }
  std::cout << "[s_plane_collection] out of range" << std::endl;
  return S_plane_def("error",0);
}

s_plane_collection s_plane_collection::getByType(const char* type) const
{


  s_plane_collection ret;
  for (auto& e : m_planes)
  {
    if (std::string(e.first) == std::string(type)) {
      ret.push_back(e.first.c_str(), e.second);
    }

  }
//   if (ret.m_planes.empty()){
//     std::cout << "[s_plane_collection] unknown type = \"" << type << "\"" << std::endl;
// 
//   }
  return ret;




}
s_plane_collection s_plane_collection::getByName(const char* name) const
{
  s_plane_collection ret;
  for (auto& e : m_planes)
  {
    if (std::string(e.second.getName()) == std::string(name)) {
      ret.push_back(e.first.c_str(), e.second);
    }

  }
//   if (ret.m_planes.empty()){
//     std::cout << "[s_plane_collection] unknown name = \"" << name << "\"" << std::endl;
// 
//   }
  return ret;
}
s_plane_collection s_plane_collection::get(const char* nameOrType) const
{
  auto pl = getByName(nameOrType);
  auto pl_type = getByType(nameOrType);
 
  return pl + pl_type;

}

S_plane_def s_plane_collection::get(const char* name, const char* type) const
{
  auto pl=getByName(name).getByType(type);
  if (pl.m_planes.empty())
  {
      std::cout << "[s_plane_collection] unknown name = \"" << name << "\"" << std::endl;
      return S_plane_def("error", 0);
  }
  if (pl.m_planes.size()>1)
  {
    std::cout << "[s_plane_collection] multiple combinations of type = "<<type<< " and name = "<<name << std::endl;
  }
  return pl.get(0);
}
S_plane_def s_plane_collection::operator()() const
{
  
  return get(0);
}
s_plane_collection::s_plane_collection(const S_plane_def& plane_)
{
  push_back(plane_);
}
void s_plane_collection::set_s_plot_collection(std::weak_ptr<S_plot_collection> plot_collection) {
  for (auto & e : m_planes) {
    e.second.set_s_plot_collection(plot_collection);
  }
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


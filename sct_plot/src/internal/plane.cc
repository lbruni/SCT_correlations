#include "internal/plane.hh"

#include "treeCollection.h"
#include <iostream>
#include "internal/plane_hit.hh"
#include "factoryDef.hh"

namespace sct_corr{


  plane::plane(Parameter_ref pl) : m_axis(*pl.buffer, pl.ID), m_ID(pl.ID)
  {

  }






  std::unique_ptr<plane> plane::create(double ID, rootEventBase* buffer) {
    const plane_struct pl(ID, buffer);
    plane::MainType type = buffer->getDataNames();
    return Class_factory_Utilities::Factory<plane>::Create( type,pl);
  }

  const axis_ref* plane::getAxis(const char* name) const
  {
    return m_axis.getAxis(name);
  }

 const axis_ref* plane::getAxis(axis_def ax) const
  {
    return m_axis.getAxis(ax);
  }


 bool isPermutation(const std::vector<std::string>& a1, const std::vector<std::string>& a2) {
 
 if (a1.size()!= a2.size())
 {
   return false;
 }

 return std::is_permutation(a1.begin(), a1.end(), a2.begin());
 }
 bool operator==(const axis_names& a1, const axis_names& a2) {
   return isPermutation(a1.m_names, a2.m_names);
 }

 bool operator==(const axis_names& a1, const std::vector<std::string>& a2) {
   return isPermutation(a1.m_names, a2);
 }

 bool operator==(const std::vector<std::string>& a1, const axis_names& a2) {
   return isPermutation(a1, a2.m_names);
 }


 bool operator<(const axis_names&  a1, const axis_names& a2) {
   return a1.m_names < a2.m_names;
 }

 std::string operator+(const axis_names& a1, const std::string& str) {
   std::string ret;

   for (const auto& e :a1.m_names)
   {
     ret += " " + e + " ";
   }

   ret += str;
   return ret;
 }

 std::string operator+(const std::string& str, const axis_names& a1) {
   std::string ret=str;

   for (const auto& e : a1.m_names) {
     ret += " " + e + " ";
   }

   return ret;
 }

 registerBaseClassDef(plane);
}


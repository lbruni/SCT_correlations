#include "internal/plotsBase.hh"
#include "factoryDef.hh"

plot::plot(Parameter_ref plot_def) :m_plot_def(plot_def)
{

}

void plot::setParameter(const char* tag, const char * value)
{

}

s_plane_collection plot::getOutputcollection()
{
  s_plane_collection ret;
  return ret;
}




std::unique_ptr < plot > plot::create(MainType name, Parameter_ref para)
{
return  Class_factory_Utilities::Factory<plot>::Create(name, para);

}

std::unique_ptr < plot > plot::create(Parameter_ref para)
{
  return  Class_factory_Utilities::Factory<plot>::Create(para.m_type, para);
}

registerBaseClassDef(plot);
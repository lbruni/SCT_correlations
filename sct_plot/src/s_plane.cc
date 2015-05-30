#include "sct_plots.h"
#include <iostream>
#include "sct_plots_internal.h"

S_plane::S_plane(double ID, treeCollection* hits) : m_ID(ID)
{
  setTreeCollection(hits);
}

S_plane::S_plane() : m_plane(nullptr)
{
  std::cout << "[s_plane] unsupported default constructor do not use" << std::endl;
}

S_plane::S_plane(double ID, S_treeCollection* hits) : S_plane(ID, hits->m_tree.get())
{

}



S_plane::S_plane(const char* name, Double_t ID) :m_name(name), m_ID(ID)
{

}

bool S_plane::isSetTreeCollectionSet() const
{
  if (m_plane)
  {
    return true;
  }
  return false;
}

void S_plane::setTreeCollection(S_treeCollection* hits)
{

  setTreeCollection(hits->m_tree.get());


}

void S_plane::setTreeCollection(treeCollection* hits)
{

  m_plane = std::make_shared<plane>(m_ID, hits);
}

const char * S_plane::getName() const
{
  return m_name.c_str();
}

bool S_plane::next()
{

  if (getX()->next()){
    if (!getY()->next())
    {
      std::cout << "vector have different length" << std::endl;
      return false;
    }

    return true;
  }

  if (getY()->next())
  {
    std::cout << "vector have different length" << std::endl;
    return false;
  }
  return false;
}

plane_hit S_plane::get() const
{
  return plane_hit(getX()->get(), getY()->get());
}

S_Axis S_plane::getX_def() const
{
  return S_Axis(getName(), m_ID, x_axis_def);
}

S_Axis S_plane::getY_def() const
{
  return S_Axis(getName(), m_ID, y_axis_def);
}

axis_ref* S_plane::getX() const
{
  if (!m_plane)
  {
    std::cout << "[s_plane] plane not set " << std::endl;
    return nullptr;
  }
  return m_plane->getX();
}

axis_ref* S_plane::getY() const
{
  if (!m_plane)
  {
    std::cout << "[s_plane] plane not set " << std::endl;
    return nullptr;
  }
  return m_plane->getY();
}
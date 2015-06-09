#ifndef plotsBase_h__
#define plotsBase_h__
#include "sct_plots.h"




class plot{
public:
  
  plot(const char* name,bool save2disk);
  virtual const char* getType() const = 0;
  virtual bool isReady()=0 ;
  virtual void pushAxis(axis_ref* axis)=0;
  virtual void pushPlane(S_plane* axis)=0;
  virtual void fill() = 0;
  virtual Long64_t Draw(const char* options, const char* cuts = "", const char* axis = "y:x") = 0;
  virtual void setParameter(const char* tag, const char * value);
  virtual s_plane_collection getOutputcollection();

  virtual const char* getOutputName()  const = 0 ;
  const char* getName() const;
  bool getSave2disk() const;
private:
  
  mutable std::string m_name;
  bool m_save2disk=true;
};



#endif // plotsBase_h__

#ifndef factoryDefinitionen_h__
#define factoryDefinitionen_h__

#include "factory.hh"
#include <iostream>
#include <string>


#ifndef  Class_factory_Utilities_THROW
  #define  Class_factory_Utilities_THROW(msg) std::cout<< "[Factory<baseClassType>::Create" <<":" << __LINE__<<"]: \n"<<msg<<std::endl;
#endif


#define registerBaseClassDef(BaseClass) namespace{\
                                         void dummy_register_function_##BaseClass(BaseClass::MainType mType, BaseClass::Parameter_ref pType){\
                                           Class_factory_Utilities::Factory<BaseClass>::Create(mType,pType );       \
                                           Class_factory_Utilities::Factory<BaseClass>::GetTypes();          \
                                           Class_factory_Utilities::Factory<BaseClass>::getInstance();\
                                           }}  \
                                     int Class_factory_VARIABLE_DO_NOT_USE##BaseClass=0

namespace Class_factory_Utilities{
  template <typename baseClassType>
  typename Factory<baseClassType>::MainType_V Factory<baseClassType>::GetTypes()
  {
    std::vector<MainType> result;
    for (auto& e : getInstance()) {
      result.push_back(e.first);
    }
    return result;
  }

  template <typename baseClassType>
  typename Factory<baseClassType>::u_pointer  Factory<baseClassType>::Create(const MainType & name, Parameter_ref params /*= ""*/)
  {
    
    for (const auto& e : getInstance()) {
      if (e.first == name) {
        return (e.second)(params);
      }
    
    }
    
    Class_factory_Utilities_THROW(std::string("unknown class: <") + name + std::string(">"));
    return nullptr;

  }

  template <typename baseClassType>
  typename Factory<baseClassType>::map_t& Factory<baseClassType>::getInstance(){
    static map_t m;
    return m;
  }


//   template <typename baseClassType>
//   void Factory<baseClassType>::do_register(const MainType & name, factoryfunc func)
//   {
//   
//   }

}
#endif // factoryDefinitionen_h__

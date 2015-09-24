#ifndef sct_types_h__
#define sct_types_h__
#include <string>

#define necessary_CONVERSION(x) x.value
#define Un_necessary_CONVERSION(x) x.value

#define TYPE_CLASS(name,type) \
class name { \
public: \
  explicit name(const type& param_) :value(param_) {}\
  type value; \
}

using type = int;
namespace sct_type {

  TYPE_CLASS(modulo_x, double);
  TYPE_CLASS(modulo_y, double);
  TYPE_CLASS(modulo_t, double);


  TYPE_CLASS(ID_t, double);
  TYPE_CLASS(residualCut_t, double);
  TYPE_CLASS(rot_angle_t, double);

  TYPE_CLASS(move_t, double);
  TYPE_CLASS(collectionName_t, std::string);
  TYPE_CLASS(AxesName_t, std::string);


}

#endif // sct_types_h__

#ifndef sct_types_h__
#define sct_types_h__


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


}

#endif // sct_types_h__

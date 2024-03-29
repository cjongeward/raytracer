#ifndef COLOR_H_
#define COLOR_H_

#include "vec.h"

class Color {
  vec color_vec;

public:
  Color(float red, float green, float blue) : color_vec{ red, green, blue } {}
  void clamp() {
    color_vec.x = std::min(1.f, std::max(0.f, color_vec.x));
    color_vec.y = std::min(1.f, std::max(0.f, color_vec.y));
    color_vec.z = std::min(1.f, std::max(0.f, color_vec.z));
  }
  operator unsigned() const {
    Color temp_color = *this;
    temp_color.clamp();
    return (static_cast<unsigned>(255. * temp_color.color_vec.z) << 8) + 
      (static_cast<unsigned>(255. * temp_color.color_vec.y) << 16) + 
      (static_cast<unsigned>(255. * temp_color.color_vec.x) << 24);
  }

  Color operator+(const Color& rhs) const {
    Color ret = *this;
    ret.color_vec += rhs.color_vec;
    return ret;
  }
  Color operator*(float rhs) const {
    Color ret = *this;
    ret.color_vec *= rhs;
    return ret;
  }
};




#endif

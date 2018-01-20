
#ifndef __ZXING_COMMON_DETECTOR_MATH_H__
#define __ZXING_COMMON_DETECTOR_MATH_H__

#include <cmath>

namespace qcv {
namespace math {

class Math {
 private:
  Math();
  ~Math();
 public:

  // Java standard Math.round
  static inline int round(float a) {
    return (int)std::floor(a +0.5f);
  }

};

}
}

#endif

#ifndef __ZXING_COMMON_DETECTOR_MATHUTILS_H__
#define __ZXING_COMMON_DETECTOR_MATHUTILS_H__

#include <cmath>

namespace qcv {
namespace math {

class MathUtils {
 private:
  MathUtils();
  ~MathUtils();
 public:

  /**
   * Ends up being a bit faster than {@link Math#round(float)}. This merely rounds its
   * argument to the nearest int, where x.5 rounds up to x+1.
   */
  static inline int round(float d) {
    return (int) (d + 0.5f);
  }

  static inline float distance(float aX, float aY, float bX, float bY) {
    float xDiff = aX - bX;
    float yDiff = aY - bY;
    return sqrt(xDiff * xDiff + yDiff * yDiff);
  }

  static inline float distance(int aX, int aY, int bX, int bY) {
    int xDiff = aX - bX;
    int yDiff = aY - bY;
    return sqrt(float(xDiff * xDiff + yDiff * yDiff));
  }
};

}
}

#endif

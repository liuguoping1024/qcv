#include <common/DetectorResult.h>

namespace qcv {

DetectorResult::DetectorResult(Ref<BitMatrix> bits,
                               ArrayRef< Ref<ResultPoint> > points)
  : bits_(bits), points_(points) {
}

Ref<BitMatrix> DetectorResult::getBits() {
  return bits_;
}

ArrayRef< Ref<ResultPoint> > DetectorResult::getPoints() {
  return points_;
}

}

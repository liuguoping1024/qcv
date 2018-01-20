#ifndef __DETECTOR_RESULT_H__
#define __DETECTOR_RESULT_H__

#include <common/Counted.h>
#include <common/Array.h>
#include <common/BitMatrix.h>
#include <ResultPoint.h>

namespace qcv {

class DetectorResult : public Counted {
private:
  Ref<BitMatrix> bits_;
  ArrayRef< Ref<ResultPoint> > points_;

public:
  DetectorResult(Ref<BitMatrix> bits, ArrayRef< Ref<ResultPoint> > points);
  Ref<BitMatrix> getBits();
  ArrayRef< Ref<ResultPoint> > getPoints();
};

}

#endif // __DETECTOR_RESULT_H__

#ifndef __MONOCHROMERECTANGLEDETECTOR_H__
#define __MONOCHROMERECTANGLEDETECTOR_H__

#include <vector>
#include <exception/NotFoundException.h>
#include <ResultPoint.h>
#include <common/BitMatrix.h>
#include <common/Counted.h>
#include <ResultPoint.h>

namespace qcv {

struct TwoInts: public Counted {
  int start;
  int end;
};

class MonochromeRectangleDetector : public Counted {
 private:
  static const int MAX_MODULES = 32;
  Ref<BitMatrix> image_;

 public:
  MonochromeRectangleDetector(Ref<BitMatrix> image) : image_(image) {  };

  std::vector<Ref<ResultPoint> > detect();

 private:
  Ref<ResultPoint> findCornerFromCenter(int centerX, int deltaX, int left, int right,
                                        int centerY, int deltaY, int top, int bottom, int maxWhiteRun);

  Ref<TwoInts> blackWhiteRange(int fixedDimension, int maxWhiteRun, int minDim, int maxDim,
                               bool horizontal);

  int max(int a, float b) { return (float) a > b ? a : (int) b;};
};

}

#endif // __MONOCHROMERECTANGLEDETECTOR_H__

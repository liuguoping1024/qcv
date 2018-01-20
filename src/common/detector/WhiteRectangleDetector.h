#ifndef __WHITERECTANGLEDETECTOR_H__
#define __WHITERECTANGLEDETECTOR_H__

#include <vector>

#include <ResultPoint.h>
#include <common/BitMatrix.h>
#include <common/Counted.h>
#include <exception/ReaderException.h>
#include <ResultPoint.h>


namespace qcv {

class WhiteRectangleDetector : public Counted {
  private:
    static int INIT_SIZE;
    static int CORR;
    Ref<BitMatrix> image_;
    int width_;
    int height_;
    int leftInit_;
    int rightInit_;
    int downInit_;
    int upInit_;

  public:
    WhiteRectangleDetector(Ref<BitMatrix> image);
    WhiteRectangleDetector(Ref<BitMatrix> image, int initSize, int x, int y);
    std::vector<Ref<ResultPoint> > detect();

  private: 
    Ref<ResultPoint> getBlackPointOnSegment(int aX, int aY, int bX, int bY);
    std::vector<Ref<ResultPoint> > centerEdges(Ref<ResultPoint> y, Ref<ResultPoint> z,
                                    Ref<ResultPoint> x, Ref<ResultPoint> t);
    bool containsBlackPoint(int a, int b, int fixed, bool horizontal);
};
}

#endif

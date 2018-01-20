// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-
#ifndef __RESULT_POINT_H__
#define __RESULT_POINT_H__


#include <common/Counted.h>
#include <vector>

namespace qcv {

class ResultPoint : public Counted {
protected:
  const float posX_;
  const float posY_;
  
public:
  ResultPoint();
  ResultPoint(float x, float y);
  ResultPoint(int x, int y);
  virtual ~ResultPoint();

  virtual float getX() const;
  virtual float getY() const;

  bool equals(Ref<ResultPoint> other);

  static void orderBestPatterns(std::vector<Ref<ResultPoint> > &patterns);
  static float distance(Ref<ResultPoint> point1, Ref<ResultPoint> point2);
  static float distance(float x1, float x2, float y1, float y2);

private:
  static float crossProductZ(Ref<ResultPoint> pointA, Ref<ResultPoint> pointB, Ref<ResultPoint> pointC);
};

}

#endif // __RESULT_POINT_H__

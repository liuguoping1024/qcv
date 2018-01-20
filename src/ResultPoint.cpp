
#include <ResultPoint.h>
#include <common/math/MathUtils.h>

using qcv::math::MathUtils;

namespace qcv {

ResultPoint::ResultPoint() : posX_(0), posY_(0) {}

ResultPoint::ResultPoint(float x, float y) : posX_(x), posY_(y) {}

ResultPoint::ResultPoint(int x, int y) : posX_(float(x)), posY_(float(y)) {}
  
ResultPoint::~ResultPoint() {}

float ResultPoint::getX() const {
  return posX_;
}
    
float ResultPoint::getY() const {
  return posY_;
}

bool ResultPoint::equals(Ref<ResultPoint> other) {
  return posX_ == other->getX() && posY_ == other->getY();
}

/**
 * <p>Orders an array of three ResultPoints in an order [A,B,C] such that AB < AC and
 * BC < AC and the angle between BC and BA is less than 180 degrees.
 */
void ResultPoint::orderBestPatterns(std::vector<Ref<ResultPoint> > &patterns) {
    // Find distances between pattern centers
    float zeroOneDistance = distance(patterns[0]->getX(), patterns[1]->getX(),patterns[0]->getY(), patterns[1]->getY());
    float oneTwoDistance = distance(patterns[1]->getX(), patterns[2]->getX(),patterns[1]->getY(), patterns[2]->getY());
    float zeroTwoDistance = distance(patterns[0]->getX(), patterns[2]->getX(),patterns[0]->getY(), patterns[2]->getY());

    Ref<ResultPoint> pointA, pointB, pointC;
    // Assume one closest to other two is B; A and C will just be guesses at first
    if (oneTwoDistance >= zeroOneDistance && oneTwoDistance >= zeroTwoDistance) {
      pointB = patterns[0];
      pointA = patterns[1];
      pointC = patterns[2];
    } else if (zeroTwoDistance >= oneTwoDistance && zeroTwoDistance >= zeroOneDistance) {
      pointB = patterns[1];
      pointA = patterns[0];
      pointC = patterns[2];
    } else {
      pointB = patterns[2];
      pointA = patterns[0];
      pointC = patterns[1];
    }

    // Use cross product to figure out whether A and C are correct or flipped.
    // This asks whether BC x BA has a positive z component, which is the arrangement
    // we want for A, B, C. If it's negative, then we've got it flipped around and
    // should swap A and C.
    if (crossProductZ(pointA, pointB, pointC) < 0.0f) {
      Ref<ResultPoint> temp = pointA;
      pointA = pointC;
      pointC = temp;
    }

    patterns[0] = pointA;
    patterns[1] = pointB;
    patterns[2] = pointC;
}

  float ResultPoint::distance(Ref<ResultPoint> pattern1, Ref<ResultPoint> pattern2) {
  return MathUtils::distance(pattern1->posX_,
                             pattern1->posY_,
                             pattern2->posX_,
                             pattern2->posY_);
}

float ResultPoint::distance(float x1, float x2, float y1, float y2) {
  float xDiff = x1 - x2;
  float yDiff = y1 - y2;
  return (float) sqrt((double) (xDiff * xDiff + yDiff * yDiff));
}

float ResultPoint::crossProductZ(Ref<ResultPoint> pointA, Ref<ResultPoint> pointB, Ref<ResultPoint> pointC) {
  float bX = pointB->getX();
  float bY = pointB->getY();
  return ((pointC->getX() - bX) * (pointA->getY() - bY)) - ((pointC->getY() - bY) * (pointA->getX() - bX));
}
}

#include <exception/NotFoundException.h>
#include <common/detector/MonochromeRectangleDetector.h>
#include <sstream>
#include <algorithm>

using std::vector;
using qcv::Ref;
using qcv::ResultPoint;
using qcv::TwoInts;
using qcv::MonochromeRectangleDetector;

vector<Ref<ResultPoint> > MonochromeRectangleDetector::detect() {
  int height = image_->getHeight();
  int width = image_->getWidth();
  int halfHeight = height / 2;
  int halfWidth = width / 2;
  int deltaY = std::max(1, height / (MAX_MODULES * 8));
  int deltaX = std::max(1, width / (MAX_MODULES * 8));

  int top = 0;
  int bottom = height;
  int left = 0;
  int right = width;
  Ref<ResultPoint> pointA(findCornerFromCenter(halfWidth, 0, left, right,
                                               halfHeight, -deltaY, top, bottom, halfWidth / 2));
  top = (int) pointA->getY() - 1;;
  Ref<ResultPoint> pointB(findCornerFromCenter(halfWidth, -deltaX, left, right,
                                               halfHeight, 0, top, bottom, halfHeight / 2));
  left = (int) pointB->getX() - 1;
  Ref<ResultPoint> pointC(findCornerFromCenter(halfWidth, deltaX, left, right,
                                               halfHeight, 0, top, bottom, halfHeight / 2));
  right = (int) pointC->getX() + 1;
  Ref<ResultPoint> pointD(findCornerFromCenter(halfWidth, 0, left, right,
                                               halfHeight, deltaY, top, bottom, halfWidth / 2));
  bottom = (int) pointD->getY() + 1;

  // Go try to find point A again with better information -- might have been off at first.
  pointA.reset(findCornerFromCenter(halfWidth, 0, left, right,
                                    halfHeight, -deltaY, top, bottom, halfWidth / 4));

  vector<Ref<ResultPoint> > corners(4);
  corners[0].reset(pointA);
  corners[1].reset(pointB);
  corners[2].reset(pointC);
  corners[3].reset(pointD);
  return corners;
}

Ref<ResultPoint> MonochromeRectangleDetector::findCornerFromCenter(int centerX, int deltaX, int left, int right,
                                                                   int centerY, int deltaY, int top, int bottom, int maxWhiteRun) {
  Ref<TwoInts> lastRange(NULL);
  for (int y = centerY, x = centerX;
       y < bottom && y >= top && x < right && x >= left;
       y += deltaY, x += deltaX) {
    Ref<TwoInts> range(NULL);
    if (deltaX == 0) {
      // horizontal slices, up and down
      range = blackWhiteRange(y, maxWhiteRun, left, right, true);
    } else {
      // vertical slices, left and right
      range = blackWhiteRange(x, maxWhiteRun, top, bottom, false);
    }
    if (range == NULL) {
      if (lastRange == NULL) {
        throw NotFoundException("Couldn't find corners (lastRange = NULL) ");
      } else {
        // lastRange was found
        if (deltaX == 0) {
          int lastY = y - deltaY;
          if (lastRange->start < centerX) {
            if (lastRange->end > centerX) {
              // straddle, choose one or the other based on direction
              Ref<ResultPoint> result(new ResultPoint(deltaY > 0 ? lastRange->start : lastRange->end, lastY));
              return result;
            }
            Ref<ResultPoint> result(new ResultPoint(lastRange->start, lastY));
            return result;
          } else {
            Ref<ResultPoint> result(new ResultPoint(lastRange->end, lastY));
            return result;
          }
        } else {
          int lastX = x - deltaX;
          if (lastRange->start < centerY) {
            if (lastRange->end > centerY) {
              Ref<ResultPoint> result(new ResultPoint(lastX, deltaX < 0 ? lastRange->start : lastRange->end));
              return result;
            }
            Ref<ResultPoint> result(new ResultPoint(lastX, lastRange->start));
            return result;
          } else {
            Ref<ResultPoint> result(new ResultPoint(lastX, lastRange->end));
            return result;
          }
        }
      }
    }
    lastRange = range;
  }   
  throw NotFoundException("Couldn't find corners");
}

Ref<TwoInts> MonochromeRectangleDetector::blackWhiteRange(int fixedDimension, int maxWhiteRun, int minDim, int maxDim,
                                                          bool horizontal) {
    
  int center = (minDim + maxDim) / 2;

  // Scan left/up first
  int start = center;
  while (start >= minDim) {
    if (horizontal ? image_->get(start, fixedDimension) : image_->get(fixedDimension, start)) {
      start--;
    } else {
      int whiteRunStart = start;
      do {
        start--;
      } while (start >= minDim && !(horizontal ? image_->get(start, fixedDimension) :
                                    image_->get(fixedDimension, start)));
      int whiteRunSize = whiteRunStart - start;
      if (start < minDim || whiteRunSize > maxWhiteRun) {
        start = whiteRunStart;
        break;
      }
    }
  }
  start++;

  // Then try right/down
  int end = center;
  while (end < maxDim) {
    if (horizontal ? image_->get(end, fixedDimension) : image_->get(fixedDimension, end)) {
      end++;
    } else {
      int whiteRunStart = end;
      do {
        end++;
      } while (end < maxDim && !(horizontal ? image_->get(end, fixedDimension) :
                                 image_->get(fixedDimension, end)));
      int whiteRunSize = end - whiteRunStart;
      if (end >= maxDim || whiteRunSize > maxWhiteRun) {
        end = whiteRunStart;
        break;
      }
    }
  }
  end--;
  Ref<TwoInts> result(NULL);
  if (end > start) {
    result = new TwoInts;
    result->start = start;
    result->end = end;
  }
  return result;
}

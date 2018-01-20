#ifndef __POINT_H__
#define __POINT_H__


namespace qcv {
class PointI {
public:
  int x;
  int y;
};

class Point {
public:
  Point() : x(0.0f), y(0.0f) {};
  Point(float x_, float y_) : x(x_), y(y_) {};

  float x;
  float y;
};

class Line {
public:
  Line(Point start_, Point end_) : start(start_), end(end_) {};

  Point start;
  Point end;
};
}
#endif // POINT_H_

#ifndef __GREYSCALE_LUMINANCE_SOURCE__
#define __GREYSCALE_LUMINANCE_SOURCE__

#include <common/LuminanceSource.h>

namespace qcv {

class GreyscaleLuminanceSource : public LuminanceSource {

private:
  typedef LuminanceSource Super;
  ArrayRef<char> greyData_;
  const int dataWidth_;
  const int dataHeight_;
  const int left_;
  const int top_;

public:
  GreyscaleLuminanceSource(ArrayRef<char> greyData, int dataWidth, int dataHeight, int left,
                           int top, int width, int height);

  ArrayRef<char> getRow(int y, ArrayRef<char> row) const;
  ArrayRef<char> getMatrix() const;

  bool isCropSupported() const {
    return true;
  }

  Ref<LuminanceSource> crop(int left, int top, int width, int height) const;

  bool isRotateSupported() const {
    return true;
  }

  Ref<LuminanceSource> rotateCounterClockwise() const;
};

}

#endif

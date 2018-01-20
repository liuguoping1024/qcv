#ifndef __GREYSCALE_ROTATED_LUMINANCE_SOURCE__
#define __GREYSCALE_ROTATED_LUMINANCE_SOURCE__

#include <common/LuminanceSource.h>

namespace qcv {

class GreyscaleRotatedLuminanceSource : public LuminanceSource {
 private:
  typedef LuminanceSource Super;
  ArrayRef<char> greyData_;
  const int dataWidth_;
  const int left_;
  const int top_;

public:
  GreyscaleRotatedLuminanceSource(ArrayRef<char> greyData, int dataWidth, int dataHeight,
      int left, int top, int width, int height);

  ArrayRef<char> getRow(int y, ArrayRef<char> row) const;
  ArrayRef<char> getMatrix() const;
};

}

#endif

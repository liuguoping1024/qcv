#include <qcv.h>
#include <common/InvertedLuminanceSource.h>

using qcv::boolean;
using qcv::Ref;
using qcv::ArrayRef;
using qcv::LuminanceSource;
using qcv::InvertedLuminanceSource;

InvertedLuminanceSource::InvertedLuminanceSource(Ref<LuminanceSource> const& delegate_)
    : Super(delegate_->getWidth(), delegate_->getHeight()), delegate(delegate_) {}  

ArrayRef<char> InvertedLuminanceSource::getRow(int y, ArrayRef<char> row) const {
  row = delegate->getRow(y, row);
  int width = getWidth();
  for (int i = 0; i < width; i++) {
    row[i] = (byte) (255 - (row[i] & 0xFF));
  }
  return row;
}

ArrayRef<char> InvertedLuminanceSource::getMatrix() const {
  ArrayRef<char> matrix = delegate->getMatrix();
  int length = getWidth() * getHeight();
  ArrayRef<char> invertedMatrix(length);
  for (int i = 0; i < length; i++) {
    invertedMatrix[i] = (byte) (255 - (matrix[i] & 0xFF));
  }
  return invertedMatrix;
}

qcv::boolean InvertedLuminanceSource::isCropSupported() const {
  return delegate->isCropSupported();
}

Ref<LuminanceSource> InvertedLuminanceSource::crop(int left, int top, int width, int height) const {
  return Ref<LuminanceSource>(new InvertedLuminanceSource(delegate->crop(left, top, width, height)));
}

boolean InvertedLuminanceSource::isRotateSupported() const {
  return delegate->isRotateSupported();
}

Ref<LuminanceSource> InvertedLuminanceSource::invert() const {
  return delegate;
}

Ref<LuminanceSource> InvertedLuminanceSource::rotateCounterClockwise() const {
  return Ref<LuminanceSource>(new InvertedLuminanceSource(delegate->rotateCounterClockwise()));
}


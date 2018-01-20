#include <BinaryBitmap.h>

using qcv::Ref;
using qcv::BitArray;
using qcv::BitMatrix;
using qcv::LuminanceSource;
using qcv::BinaryBitmap;
	
// VC++
using qcv::Binarizer;

BinaryBitmap::BinaryBitmap(Ref<Binarizer> binarizer) : binarizer_(binarizer) {
}
	
BinaryBitmap::~BinaryBitmap() {
}
	
Ref<BitArray> BinaryBitmap::getBlackRow(int y, Ref<BitArray> row) {
  return binarizer_->getBlackRow(y, row);
}
	
Ref<BitMatrix> BinaryBitmap::getBlackMatrix() {
  return binarizer_->getBlackMatrix();
}
	
int BinaryBitmap::getWidth() const {
  return getLuminanceSource()->getWidth();
}
	
int BinaryBitmap::getHeight() const {
  return getLuminanceSource()->getHeight();
}
	
Ref<LuminanceSource> BinaryBitmap::getLuminanceSource() const {
  return binarizer_->getLuminanceSource();
}
	

bool BinaryBitmap::isCropSupported() const {
  return getLuminanceSource()->isCropSupported();
}

Ref<BinaryBitmap> BinaryBitmap::crop(int left, int top, int width, int height) {
  return Ref<BinaryBitmap> (new BinaryBitmap(binarizer_->createBinarizer(getLuminanceSource()->crop(left, top, width, height))));
}

bool BinaryBitmap::isRotateSupported() const {
  return getLuminanceSource()->isRotateSupported();
}

Ref<BinaryBitmap> BinaryBitmap::rotateCounterClockwise() {
  return Ref<BinaryBitmap> (new BinaryBitmap(binarizer_->createBinarizer(getLuminanceSource()->rotateCounterClockwise())));
}

#include <common/BitMatrix.h>
#include <exception/IllegalArgumentException.h>

#include <iostream>
#include <sstream>
#include <string>

using std::ostream;
using std::ostringstream;

using qcv::BitMatrix;
using qcv::BitArray;
using qcv::ArrayRef;
using qcv::Ref;

void BitMatrix::init(int width, int height) {
  if (width < 1 || height < 1) {
    throw IllegalArgumentException("Both dimensions must be greater than 0");
  }
  this->width = width;
  this->height = height;
  this->rowSize = (width + bitsPerWord - 1) >> logBits;
  bits = ArrayRef<int>(rowSize * height);
}

BitMatrix::BitMatrix(int dimension) {
  init(dimension, dimension);
}

BitMatrix::BitMatrix(int width, int height) {
  init(width, height);
}

BitMatrix::~BitMatrix() {}

void BitMatrix::flip(int x, int y) {
  int offset = y * rowSize + (x >> logBits);
  bits[offset] ^= 1 << (x & bitsMask);
}

void BitMatrix::setRegion(int left, int top, int width, int height) {
  if (top < 0 || left < 0) {
    throw IllegalArgumentException("Left and top must be nonnegative");
  }
  if (height < 1 || width < 1) {
    throw IllegalArgumentException("Height and width must be at least 1");
  }
  int right = left + width;
  int bottom = top + height;
  if (bottom > this->height || right > this->width) {
    throw IllegalArgumentException("The region must fit inside the matrix");
  }
  for (int y = top; y < bottom; y++) {
    int offset = y * rowSize;
    for (int x = left; x < right; x++) {
      bits[offset + (x >> logBits)] |= 1 << (x & bitsMask);
    }
  }
}

Ref<BitArray> BitMatrix::getRow(int y, Ref<BitArray> row) {
  if (row.empty() || row->getSize() < width) {
    row = new BitArray(width);
  }
  int offset = y * rowSize;
  for (int x = 0; x < rowSize; x++) {
    row->setBulk(x << logBits, bits[offset + x]);
  }
  return row;
}

int BitMatrix::getWidth() const {
  return width;
}

int BitMatrix::getHeight() const {
  return height;
}

ArrayRef<int> BitMatrix::getTopLeftOnBit() const {
  int bitsOffset = 0;
  while (bitsOffset < bits->size() && bits[bitsOffset] == 0) {
    bitsOffset++;
  }
  if (bitsOffset == bits->size()) {
    return ArrayRef<int>();
  }
  int y = bitsOffset / rowSize;
  int x = (bitsOffset % rowSize) << 5;

  int theBits = bits[bitsOffset];
  int bit = 0;
  while ((theBits << (31-bit)) == 0) {
    bit++;
  }
  x += bit;
  ArrayRef<int> res (2);
  res[0]=x;
  res[1]=y;
  return res;
}

ArrayRef<int> BitMatrix::getBottomRightOnBit() const {
  int bitsOffset = bits->size() - 1;
  while (bitsOffset >= 0 && bits[bitsOffset] == 0) {
    bitsOffset--;
  }
  if (bitsOffset < 0) {
    return ArrayRef<int>();
  }

  int y = bitsOffset / rowSize;
  int x = (bitsOffset % rowSize) << 5;

  int theBits = bits[bitsOffset];
  int bit = 31;
  while ((theBits >> bit) == 0) {
    bit--;
  }
  x += bit;

  ArrayRef<int> res (2);
  res[0]=x;
  res[1]=y;
  return res;
}

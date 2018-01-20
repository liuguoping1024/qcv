// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-
#ifndef __IMAGE_READER_SOURCE_H_
#define __IMAGE_READER_SOURCE_H_

#include <common/LuminanceSource.h>

class ImageReaderSource : public qcv::LuminanceSource {
private:
  typedef LuminanceSource Super;

  const qcv::ArrayRef<char> image;
  const int comps;

  char convertPixel(const char* pixel) const;

public:
  static qcv::Ref<LuminanceSource> create(std::string const& filename);

  ImageReaderSource(qcv::ArrayRef<char> image, int width, int height, int comps);

  qcv::ArrayRef<char> getRow(int y, qcv::ArrayRef<char> row) const;
  qcv::ArrayRef<char> getMatrix() const;
};

#endif /* __IMAGE_READER_SOURCE_H_ */

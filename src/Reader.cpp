#include <Reader.h>

namespace qcv {

Reader::~Reader() { }

Ref<Result> Reader::decode(Ref<BinaryBitmap> image) {
  return decode(image, DecodeHints::DEFAULT_HINT);
}

}

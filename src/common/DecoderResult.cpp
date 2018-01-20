#include <common/DecoderResult.h>

using namespace std;
using namespace qcv;

DecoderResult::DecoderResult(ArrayRef<char> rawBytes,
                             Ref<String> text,
                             ArrayRef< ArrayRef<char> >& byteSegments,
                             string const& ecLevel) :
  rawBytes_(rawBytes),
  text_(text),
  byteSegments_(byteSegments),
  ecLevel_(ecLevel) {}

DecoderResult::DecoderResult(ArrayRef<char> rawBytes,
                             Ref<String> text)
  : rawBytes_(rawBytes), text_(text) {}

ArrayRef<char> DecoderResult::getRawBytes() {
  return rawBytes_;
}

Ref<String> DecoderResult::getText() {
  return text_;
}

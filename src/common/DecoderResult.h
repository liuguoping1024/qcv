#ifndef __DECODER_RESULT_H__
#define __DECODER_RESULT_H__

#include <common/Counted.h>
#include <common/Array.h>
#include <string>
#include <common/Str.h>

namespace qcv {

class DecoderResult : public Counted {
private:
  ArrayRef<char> rawBytes_;
  Ref<String> text_;
  ArrayRef< ArrayRef<char> > byteSegments_;
  std::string ecLevel_;

public:
  DecoderResult(ArrayRef<char> rawBytes,
                Ref<String> text,
                ArrayRef< ArrayRef<char> >& byteSegments,
                std::string const& ecLevel);

  DecoderResult(ArrayRef<char> rawBytes, Ref<String> text);

  ArrayRef<char> getRawBytes();
  Ref<String> getText();
};

}

#endif // __DECODER_RESULT_H__

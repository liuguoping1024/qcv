#ifndef __RESULT_H__
#define __RESULT_H__

#include <string>
#include <common/Array.h>
#include <common/Counted.h>
#include <common/Str.h>
#include <ResultPoint.h>
#include <BarcodeFormat.h>

namespace qcv {

class Result : public Counted {
private:
  Ref<String> text_;
  ArrayRef<char> rawBytes_;
  ArrayRef< Ref<ResultPoint> > resultPoints_;
  BarcodeFormat format_;

public:
  Result(Ref<String> text,
         ArrayRef<char> rawBytes,
         ArrayRef< Ref<ResultPoint> > resultPoints,
         BarcodeFormat format);
  ~Result();
  Ref<String> getText();
  ArrayRef<char> getRawBytes();
  ArrayRef< Ref<ResultPoint> > const& getResultPoints() const;
  ArrayRef< Ref<ResultPoint> >& getResultPoints();
  BarcodeFormat getBarcodeFormat() const;

  friend std::ostream& operator<<(std::ostream &out, Result& result);
};

}
#endif // __RESULT_H__

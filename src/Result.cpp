#include <Result.h>

using qcv::Result;
using qcv::Ref;
using qcv::ArrayRef;
using qcv::String;
using qcv::ResultPoint;

// VC++
using qcv::BarcodeFormat;

Result::Result(Ref<String> text,
               ArrayRef<char> rawBytes,
               ArrayRef< Ref<ResultPoint> > resultPoints,
               BarcodeFormat format) :
  text_(text), rawBytes_(rawBytes), resultPoints_(resultPoints), format_(format) {
}

Result::~Result() {
}

Ref<String> Result::getText() {
  return text_;
}

ArrayRef<char> Result::getRawBytes() {
  return rawBytes_;
}

ArrayRef< Ref<ResultPoint> > const& Result::getResultPoints() const {
  return resultPoints_;
}

ArrayRef< Ref<ResultPoint> >& Result::getResultPoints() {
  return resultPoints_;
}

qcv::BarcodeFormat Result::getBarcodeFormat() const {
  return format_;
}

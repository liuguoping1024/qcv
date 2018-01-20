#ifndef __DECODEHINTS_H_
#define __DECODEHINTS_H_


#include <BarcodeFormat.h>
#include <ResultPointCallback.h>

namespace qcv {

typedef unsigned int DecodeHintType;
class DecodeHints;
DecodeHints operator | (DecodeHints const&, DecodeHints const&);

class DecodeHints {
 private:
  DecodeHintType hints;
  Ref<ResultPointCallback> callback;

 public:
  static const DecodeHintType AZTEC_HINT = 1 << BarcodeFormat::AZTEC;
  static const DecodeHintType CODABAR_HINT = 1 << BarcodeFormat::CODABAR;
  static const DecodeHintType CODE_39_HINT = 1 << BarcodeFormat::CODE_39;
  static const DecodeHintType CODE_93_HINT = 1 << BarcodeFormat::CODE_93;
  static const DecodeHintType CODE_128_HINT = 1 << BarcodeFormat::CODE_128;
  static const DecodeHintType DATA_MATRIX_HINT = 1 << BarcodeFormat::DATA_MATRIX;
  static const DecodeHintType EAN_8_HINT = 1 << BarcodeFormat::EAN_8;
  static const DecodeHintType EAN_13_HINT = 1 << BarcodeFormat::EAN_13;
  static const DecodeHintType ITF_HINT = 1 << BarcodeFormat::ITF;
  static const DecodeHintType MAXICODE_HINT = 1 << BarcodeFormat::MAXICODE;
  static const DecodeHintType PDF_417_HINT = 1 << BarcodeFormat::PDF_417;
  static const DecodeHintType QR_CODE_HINT = 1 << BarcodeFormat::QR_CODE;
  static const DecodeHintType RSS_14_HINT = 1 << BarcodeFormat::RSS_14;
  static const DecodeHintType RSS_EXPANDED_HINT = 1 << BarcodeFormat::RSS_EXPANDED;
  static const DecodeHintType UPC_A_HINT = 1 << BarcodeFormat::UPC_A;
  static const DecodeHintType UPC_E_HINT = 1 << BarcodeFormat::UPC_E;
  static const DecodeHintType UPC_EAN_EXTENSION_HINT = 1 << BarcodeFormat::UPC_EAN_EXTENSION;

  static const DecodeHintType TRYHARDER_HINT = 1 << 31;
  static const DecodeHintType CHARACTER_SET = 1 << 30;
  // static const DecodeHintType ALLOWED_LENGTHS = 1 << 29;
  // static const DecodeHintType ASSUME_CODE_39_CHECK_DIGIT = 1 << 28;
  static const DecodeHintType  ASSUME_GS1 = 1 << 27;
  // static const DecodeHintType NEED_RESULT_POINT_CALLBACK = 1 << 26;
  
  static const DecodeHints PRODUCT_HINT;
  static const DecodeHints ONED_HINT;
  static const DecodeHints DEFAULT_HINT;

  DecodeHints();
  DecodeHints(DecodeHintType init);

  void addFormat(BarcodeFormat toadd);
  bool containsFormat(BarcodeFormat tocheck) const;
  bool isEmpty() const {return (hints==0);}
  void clear() {hints=0;}
  void setTryHarder(bool toset);
  bool getTryHarder() const;

  void setResultPointCallback(Ref<ResultPointCallback> const&);

  Ref<ResultPointCallback> getResultPointCallback() const;

  friend DecodeHints operator | (DecodeHints const&, DecodeHints const&);
};

}

#endif

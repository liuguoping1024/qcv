#ifndef __BARCODE_FORMAT_H__
#define __BARCODE_FORMAT_H__

namespace qcv {

class BarcodeFormat {
public:
  // if you update the enum, update BarcodeFormat.cpp

  enum Value {
    NONE,
    AZTEC,
    CODABAR,
    CODE_39,
    CODE_93,
    CODE_128,
    DATA_MATRIX,
    EAN_8,
    EAN_13,
    ITF,
    MAXICODE,
    PDF_417,
    QR_CODE,
    RSS_14,
    RSS_EXPANDED,
    UPC_A,
    UPC_E,
    UPC_EAN_EXTENSION
  };

  BarcodeFormat(Value v) : value(v) {}  
  const Value value;
  operator Value () const {return value;}

  static char const* barcodeFormatNames[];
};

}

#endif // __BARCODE_FORMAT_H__

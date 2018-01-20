#ifndef BINARIZER_H_
#define BINARIZER_H_

#include <common/LuminanceSource.h>
#include <common/BitArray.h>
#include <common/BitMatrix.h>
#include <common/Counted.h>

namespace qcv {

class Binarizer : public Counted {
 private:
  Ref<LuminanceSource> source_;

 public:
  Binarizer(Ref<LuminanceSource> source);
  virtual ~Binarizer();

  virtual Ref<BitArray> getBlackRow(int y, Ref<BitArray> row) = 0;
  virtual Ref<BitMatrix> getBlackMatrix() = 0;

  Ref<LuminanceSource> getLuminanceSource() const ;
  virtual Ref<Binarizer> createBinarizer(Ref<LuminanceSource> source) = 0;

  int getWidth() const;
  int getHeight() const;

};

}
#endif /* BINARIZER_H_ */

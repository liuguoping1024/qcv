#ifndef __GLOBALHISTOGRAMBINARIZER_H__
#define __GLOBALHISTOGRAMBINARIZER_H__

#include <Binarizer.h>
#include <common/BitArray.h>
#include <common/BitMatrix.h>
#include <common/Array.h>

namespace qcv {
	
class GlobalHistogramBinarizer : public Binarizer {
private:
  ArrayRef<char> luminances;
  ArrayRef<int> buckets;
public:
  GlobalHistogramBinarizer(Ref<LuminanceSource> source);
  virtual ~GlobalHistogramBinarizer();
		
  virtual Ref<BitArray> getBlackRow(int y, Ref<BitArray> row);
  virtual Ref<BitMatrix> getBlackMatrix();
  static int estimateBlackPoint(ArrayRef<int> const& buckets);
  Ref<Binarizer> createBinarizer(Ref<LuminanceSource> source);
private:
  void initArrays(int luminanceSize);
};

}
	
#endif /* GLOBALHISTOGRAMBINARIZER_H_ */

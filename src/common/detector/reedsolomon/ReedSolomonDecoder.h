#ifndef __REED_SOLOMON_DECODER_H__
#define __REED_SOLOMON_DECODER_H__

#include <memory>
#include <vector>
#include <common/Counted.h>
#include <common/Array.h>
#include <common/detector/reedsolomon/GenericGFPoly.h>
#include <common/detector/reedsolomon/GenericGF.h>

namespace qcv {
class GenericGFPoly;
class GenericGF;

class ReedSolomonDecoder {
private:
  Ref<GenericGF> field;
public:
  ReedSolomonDecoder(Ref<GenericGF> fld);
  ~ReedSolomonDecoder();
  void decode(ArrayRef<int> received, int twoS);
  std::vector<Ref<GenericGFPoly> > runEuclideanAlgorithm(Ref<GenericGFPoly> a, Ref<GenericGFPoly> b, int R);

private:
  ArrayRef<int> findErrorLocations(Ref<GenericGFPoly> errorLocator);
  ArrayRef<int> findErrorMagnitudes(Ref<GenericGFPoly> errorEvaluator, ArrayRef<int> errorLocations);
};
}

#endif // __REED_SOLOMON_DECODER_H__

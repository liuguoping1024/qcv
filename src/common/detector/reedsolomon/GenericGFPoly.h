
#ifndef GENERICGFPOLY_H
#define GENERICGFPOLY_H

#include <vector>
#include <common/Array.h>
#include <common/Counted.h>

namespace qcv {

class GenericGF;
  
class GenericGFPoly : public Counted {
private:
  GenericGF &field_;
  ArrayRef<int> coefficients_;
    
public:
  GenericGFPoly(GenericGF &field, ArrayRef<int> coefficients);
  ArrayRef<int> getCoefficients();
  int getDegree();
  bool isZero();
  int getCoefficient(int degree);
  int evaluateAt(int a);
  Ref<GenericGFPoly> addOrSubtract(Ref<GenericGFPoly> other);
  Ref<GenericGFPoly> multiply(Ref<GenericGFPoly> other);
  Ref<GenericGFPoly> multiply(int scalar);
  Ref<GenericGFPoly> multiplyByMonomial(int degree, int coefficient);
  std::vector<Ref<GenericGFPoly> > divide(Ref<GenericGFPoly> other);
    

};

}

#endif //GENERICGFPOLY_H

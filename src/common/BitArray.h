#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__


#include <qcv.h>
#include <common/Counted.h>
#include <common/Array.h>
#include <exception/IllegalArgumentException.h>
#include <vector>
#include <limits>
#include <iostream>

namespace qcv {

class BitArray : public Counted {
public:
  static const int bitsPerWord = std::numeric_limits<unsigned int>::digits;

private:
  int size;
  ArrayRef<int> bits;
  static const int logBits = QCV_LOG_DIGITS(bitsPerWord);
  static const int bitsMask = (1 << logBits) - 1;

public:
  BitArray(int size);
  ~BitArray();
  int getSize() const;

  bool get(int i) const {
    return (bits[i >> logBits] & (1 << (i & bitsMask))) != 0;
  }

  void set(int i) {
    bits[i >> logBits] |= 1 << (i & bitsMask);
  }

  int getNextSet(int from);
  int getNextUnset(int from);

  void setBulk(int i, int newBits);
  void setRange(int start, int end);
  void clear();
  bool isRange(int start, int end, bool value);
  std::vector<int>& getBitArray();
  
  void reverse();

  class Reverse {
   private:
    Ref<BitArray> array;
   public:
    Reverse(Ref<BitArray> array);
    ~Reverse();
  };

private:
  static int makeArraySize(int size);
};

std::ostream& operator << (std::ostream&, BitArray const&);

}

#endif // __BIT_ARRAY_H__

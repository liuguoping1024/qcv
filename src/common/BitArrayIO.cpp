
#include <common/BitArray.h>

using qcv::BitArray;
using std::ostream;

ostream& qcv::operator << (ostream& os, BitArray const& ba) {
  for (int i = 0, size = ba.getSize(); i < size; i++) {
    if ((i & 0x07) == 0) {
      os << ' ';
    }
    os << (ba.get(i) ? 'X' : '.');
  }
  return os;
}

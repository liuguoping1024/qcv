#include <Result.h>

using qcv::Result;
using std::ostream;

ostream& qcv::operator<<(ostream &out, Result& result) {
  if (result.text_ != 0) {
    out << result.text_->getText();
  } else {
    out << "[" << result.rawBytes_->size() << " bytes]";
  }
  return out;
}

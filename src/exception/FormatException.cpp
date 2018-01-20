#include <exception/FormatException.h>

namespace qcv {

FormatException::FormatException() {}

FormatException::FormatException(const char *msg) :
    ReaderException(msg) {
}

FormatException::~FormatException() throw() {
}

FormatException const&
FormatException::getFormatInstance() {
  static FormatException instance;
  return instance;
}

}

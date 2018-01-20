#ifndef __FORMAT_EXCEPTION_H__
#define __FORMAT_EXCEPTION_H__

#include <exception/ReaderException.h>

namespace qcv {

class FormatException : public ReaderException {
public:
  FormatException();
  FormatException(const char *msg);
  ~FormatException() throw();

  static FormatException const& getFormatInstance();
};

}
#endif // __FORMAT_EXCEPTION_H__

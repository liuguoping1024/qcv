
#ifndef __READER_EXCEPTION_H__
#define __READER_EXCEPTION_H__

#include <Exception.h>

namespace qcv {

class ReaderException : public Exception {
 public:
  ReaderException() throw() {}
  ReaderException(char const* msg) throw() : Exception(msg) {}
  ~ReaderException() throw() {}
};

}

#endif // __READER_EXCEPTION_H__

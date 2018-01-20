#ifndef __ILLEGAL_STATE_EXCEPTION_H__
#define __ILLEGAL_STATE_EXCEPTION_H__

#include <exception/ReaderException.h>

namespace qcv {

class IllegalStateException : public ReaderException {
public:
  IllegalStateException() throw() {}
  IllegalStateException(const char *msg) throw() : ReaderException(msg) {}
  ~IllegalStateException() throw() {}
};

}

#endif // __ILLEGAL_STATE_EXCEPTION_H__

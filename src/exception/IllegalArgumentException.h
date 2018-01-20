#ifndef __ILLEGAL_ARGUMENT_EXCEPTION_H__
#define __ILLEGAL_ARGUMENT_EXCEPTION_H__


#include <Exception.h>

namespace qcv {

class IllegalArgumentException : public Exception {
public:
  IllegalArgumentException();
  IllegalArgumentException(const char *msg);
  ~IllegalArgumentException() throw();
};

}

#endif // __ILLEGAL_ARGUMENT_EXCEPTION_H__

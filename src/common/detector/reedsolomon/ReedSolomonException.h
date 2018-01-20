#ifndef __REED_SOLOMON_EXCEPTION_H__
#define __REED_SOLOMON_EXCEPTION_H__

#include <Exception.h>

namespace qcv {
class ReedSolomonException : public Exception {
public:
  ReedSolomonException(const char *msg) throw();
  ~ReedSolomonException() throw();
};
}

#endif // __REED_SOLOMON_EXCEPTION_H__

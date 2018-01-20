#ifndef __CHECKSUM_EXCEPTION_H__
#define __CHECKSUM_EXCEPTION_H__

#include <exception/ReaderException.h>

namespace qcv {
  class ChecksumException : public ReaderException {
    typedef ReaderException Base;
  public:
    ChecksumException() throw();
    ChecksumException(const char *msg) throw();
    ~ChecksumException() throw();
  };
}

#endif // __CHECKSUM_EXCEPTION_H__

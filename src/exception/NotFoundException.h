// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-

#ifndef __NOT_FOUND_EXCEPTION_H__
#define __NOT_FOUND_EXCEPTION_H__

#include <exception/ReaderException.h>

namespace qcv {

class NotFoundException : public ReaderException {
public:
  NotFoundException() throw() {}
  NotFoundException(const char *msg) throw() : ReaderException(msg) {}
  ~NotFoundException() throw() {}
};

}

#endif // __NOT_FOUND_EXCEPTION_H__

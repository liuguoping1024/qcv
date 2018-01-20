#include <exception/IllegalArgumentException.h>

using qcv::IllegalArgumentException;

IllegalArgumentException::IllegalArgumentException() : Exception() {}
IllegalArgumentException::IllegalArgumentException(const char *msg) : Exception(msg) {}
IllegalArgumentException::~IllegalArgumentException() throw() {}

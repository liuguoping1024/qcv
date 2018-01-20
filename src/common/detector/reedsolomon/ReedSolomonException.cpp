
#include <common/detector/reedsolomon/ReedSolomonException.h>

namespace qcv {
ReedSolomonException::ReedSolomonException(const char *msg) throw() :
    Exception(msg) {
}
ReedSolomonException::~ReedSolomonException() throw() {
}

}

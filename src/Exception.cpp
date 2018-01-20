#include <qcv.h>
#include <Exception.h>
#include <string.h>

using qcv::Exception;

void Exception::deleteMessage() {
  delete [] message;
}

char const* Exception::copy(char const* msg) {
  char* message = 0;
  if (msg) {
    int l = strlen(msg)+1;
    if (l) {
      message = new char[l];
      strcpy(message, msg);
    }
  }
  return message;
}

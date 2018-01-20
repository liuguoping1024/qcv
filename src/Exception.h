#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <exception>

namespace qcv {

class Exception : public std::exception {
private:
  char const* const message;

public:
  Exception() throw() : message(0) {}
  Exception(const char* msg) throw() : message(copy(msg)) {}
  Exception(Exception const& that) throw() : std::exception(that), message(copy(that.message)) {}
  ~Exception() throw() {
    if(message) {
      deleteMessage();
    }
  }
  char const* what() const throw() {return message ? message : "";}

private:
  static char const* copy(char const*);
  void deleteMessage();
};

}

#endif // __EXCEPTION_H__

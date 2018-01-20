#ifndef __STR_H__
#define __STR_H__

#include <string>
#include <iostream>
#include <common/Counted.h>

namespace qcv {

class String;
std::ostream& operator << (std::ostream& out, String const& s);

class String : public Counted {
private:
  std::string text_;
public:
  explicit String(const std::string &text);
  explicit String(int);
  char charAt(int) const;
  Ref<String> substring(int) const;
  const std::string& getText() const;
  int size() const;
  void append(std::string const& tail);
  void append(char c);
  int length() const;
  friend std::ostream& qcv::operator << (std::ostream& out, String const& s);
};

}

#endif // __COMMON__STRING_H__

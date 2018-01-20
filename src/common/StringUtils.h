#ifndef __STRING_UTILS__
#define __STRING_UTILS__

#include <string>
#include <map>
#include <DecodeHints.h>

namespace qcv {
namespace common {

class StringUtils {
private:
  static char const* const PLATFORM_DEFAULT_ENCODING;

  StringUtils() {}

public:
  static char const* const ASCII;
  static char const* const SHIFT_JIS;
  static char const* const GB2312;
  static char const* const EUC_JP;
  static char const* const UTF8;
  static char const* const ISO88591;
  static const bool ASSUME_SHIFT_JIS;

  typedef std::map<DecodeHintType, std::string> Hashtable;

  static std::string guessEncoding(char* bytes, int length, Hashtable const& hints);
};

}
}

#endif

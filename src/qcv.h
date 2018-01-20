#ifndef __QCV_H_
#define __QCV_H_

#define QCV_ARRAY_LEN(v) ((int)(sizeof(v)/sizeof(v[0])))
#define QCV_LOG_DIGITS(digits) \
    ((digits == 8) ? 3 : \
     ((digits == 16) ? 4 : \
      ((digits == 32) ? 5 : \
       ((digits == 64) ? 6 : \
        ((digits == 128) ? 7 : \
         (-1))))))

#ifndef QCV_DEBUG
#define QCV_DEBUG 0
#endif

namespace qcv {
typedef char byte;
typedef bool boolean;
}

#include <limits>

#if defined(_WIN32) || defined(_WIN64)

#include <float.h>

namespace qcv {
inline bool isnan(float v) {return _isnan(v) != 0;}
inline bool isnan(double v) {return _isnan(v) != 0;}
inline float nan() {return std::numeric_limits<float>::quiet_NaN();}
}

#else

#include <cmath>

namespace qcv {
inline bool isnan(float v) {return std::isnan(v);}
inline bool isnan(double v) {return std::isnan(v);}
inline float nan() {return std::numeric_limits<float>::quiet_NaN();}
}

#endif

#if QCV_DEBUG

#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::string;
using std::ostream;

#if QCV_DEBUG_TIMER

#include <sys/time.h>

namespace qcv {

class DebugTimer {
public:
  DebugTimer(char const* string_) : chars(string_) {
    gettimeofday(&start, 0);
  }

  DebugTimer(std::string const& string_) : chars(0), string(string_) {
    gettimeofday(&start, 0);
  }

  void mark(char const* string) {
    struct timeval end;
    gettimeofday(&end, 0);
    int diff =
      (end.tv_sec - start.tv_sec)*1000*1000+(end.tv_usec - start.tv_usec);
    
    cerr << diff << " " << string << '\n';
  }

  void mark(std::string string) {
    mark(string.c_str());
  }

  ~DebugTimer() {
    if (chars) {
      mark(chars);
    } else {
      mark(string.c_str());
    }
  }

private:
  char const* const chars;
  std::string string;
  struct timeval start;
};

}

#define QCV_TIME(string) DebugTimer __timer__ (string)
#define QCV_TIME_MARK(string) __timer__.mark(string)

#endif

#endif // ZXING_DEBUG

#ifndef QCV_TIME
#define QCV_TIME(string) (void)0
#endif
#ifndef QCV_TIME_MARK
#define QCV_TIME_MARK(string) (void)0
#endif

#endif

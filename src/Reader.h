#ifndef __READER_H__
#define __READER_H__


#include <BinaryBitmap.h>
#include <Result.h>
#include <DecodeHints.h>

namespace qcv {

 class Reader : public Counted {
  protected:
   Reader() {}
  public:
   virtual Ref<Result> decode(Ref<BinaryBitmap> image);

   virtual Ref<Result> decode(Ref<BinaryBitmap> image, DecodeHints hints) = 0;
   virtual ~Reader();
};

}

#endif // __READER_H__

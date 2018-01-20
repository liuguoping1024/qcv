#ifndef __HYBRIDBINARIZER_H__
#define __HYBRIDBINARIZER_H__

#include <vector>
#include <Binarizer.h>
#include <common/GlobalHistogramBinarizer.h>
#include <common/BitArray.h>
#include <common/BitMatrix.h>

namespace qcv {
	
	class HybridBinarizer : public GlobalHistogramBinarizer {
	 private:
    Ref<BitMatrix> matrix_;
	  Ref<BitArray> cached_row_;

	public:
		HybridBinarizer(Ref<LuminanceSource> source);
		virtual ~HybridBinarizer();
		
		virtual Ref<BitMatrix> getBlackMatrix();
		Ref<Binarizer> createBinarizer(Ref<LuminanceSource> source);
  private:
    // We'll be using one-D arrays because C++ can't dynamically allocate 2D
    // arrays
    ArrayRef<int> calculateBlackPoints(ArrayRef<char> luminances,
                                       int subWidth,
                                       int subHeight,
                                       int width,
                                       int height);
    void calculateThresholdForBlock(ArrayRef<char> luminances,
                                    int subWidth,
                                    int subHeight,
                                    int width,
                                    int height,
                                    ArrayRef<int> blackPoints,
                                    Ref<BitMatrix> const& matrix);
    void thresholdBlock(ArrayRef<char>luminances,
                        int xoffset,
                        int yoffset,
                        int threshold,
                        int stride,
                        Ref<BitMatrix> const& matrix);
	};

}

#endif

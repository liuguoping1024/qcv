#ifndef __BINARYBITMAP_H__
#define __BINARYBITMAP_H__

#include <common/Counted.h>
#include <common/BitMatrix.h>
#include <common/BitArray.h>
#include <Binarizer.h>

namespace qcv {
	
	class BinaryBitmap : public Counted {
	private:
		Ref<Binarizer> binarizer_;
		
	public:
		BinaryBitmap(Ref<Binarizer> binarizer);
		virtual ~BinaryBitmap();
		
		Ref<BitArray> getBlackRow(int y, Ref<BitArray> row);
		Ref<BitMatrix> getBlackMatrix();
		
		Ref<LuminanceSource> getLuminanceSource() const;

		int getWidth() const;
		int getHeight() const;

		bool isRotateSupported() const;
		Ref<BinaryBitmap> rotateCounterClockwise();

		bool isCropSupported() const;
		Ref<BinaryBitmap> crop(int left, int top, int width, int height);

	};
	
}

#endif /* BINARYBITMAP_H_ */

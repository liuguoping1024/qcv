#include <Binarizer.h>

namespace qcv {
	
	Binarizer::Binarizer(Ref<LuminanceSource> source) : source_(source) {
  }
	
	Binarizer::~Binarizer() {
	}
	
	Ref<LuminanceSource> Binarizer::getLuminanceSource() const {
		return source_;
	}

  int Binarizer::getWidth() const {
    return source_->getWidth();
  }
	
  int Binarizer::getHeight() const {
    return source_->getHeight();
  }
	
}

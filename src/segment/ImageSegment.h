#ifndef __IMAGE_SEGMENT_H__
#define __IMAGE_SEGMENT_H__

#include <common/Counted.h>
#include <common/Array.h>
#include <common/ImageSource.h>
#include <string.h>
#include <stdint.h>

namespace qcv {

using qcv::ImageSource;

typedef struct {
	int rank;
	int p;
	int size;
} uni_elt;

class universe {
public:
	universe(int elements);
	~universe();

	int find(int x);

	void join(int x, int y);

	int size(int x) const {
		return elts[x].size;
	}

	int num_sets() const {
		return num;
	}

private:
	uni_elt *elts;
	int num;
};

ImageSource<rgb> * segment_image(ImageSource<rgb> *im, float sigma, float c, int min_size,int *num_ccs);

}

#endif

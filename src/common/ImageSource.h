#ifndef __IMAGE_SOURCE_HEADER__
#define __IMAGE_SOURCE_HEADER__

#include <common/Counted.h>
#include <common/Array.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

namespace qcv {

typedef struct {
	uint8_t r, g, b;
} rgb;

typedef struct {
	uint8_t r, g, b, a;
} rgba;


template<class T>
class ImageSource: public Counted {

public:
	int getWidth() const {
		return width_;
	}
	int getHeight() const {
		return height_;
	}


public:
	/* create an image */
	ImageSource(const int width, const int height,const bool init = true);

	/* delete an image */
	virtual ~ImageSource();

	void init(const T &val);

	/* copy an image */
	ImageSource<T> *copy() const;

public:

	inline T& imRef(int offset) {
		return data[offset];
	}

	inline T& imRef(int x, int y) {
		return access[y][x];
	}

	inline T* imPtr(int offset) {
		return &data[offset];
	}

	inline T* imPtr(int x, int y) {
		return &access[y][x];
	}

public:

	void getMinMax(T *ret_min, T *ret_max);

	//!< ???
	ImageSource<uint8_t> *threshold(int t);

	/* get the width of an image. */
	int width() const {
		return width_;
	}

	/* get the height of an image. */
	int height() const {
		return height_;
	}

protected:
	//ArrayRef<T> image_data_;
	//ArrayRef<int> image_rows_;

	/* image data. */
	T *data;

	/* row pointers. */
	T **access;

private:
	int width_;
	int height_;
};



template <class T>
ImageSource<T>::ImageSource(const int width, const int height, const bool init) {
	width_ = width;
	height_ = height;

	data = new T[width * height];  // allocate space for image data
	access = new T*[height];   // allocate space for row pointers

	// initialize row pointers
	for (int i = 0; i < height; i++) {
		access[i] = data + (i * width);
	}

	if (init) {
		memset(data, 0, width * height * sizeof(T));
	}
}

template <class T>
ImageSource<T>::~ImageSource() {
	  delete [] data;
	  delete [] access;
}

template<class T>
void ImageSource<T>::init(const T &val) {
	T *ptr = this->imPtr(0, 0);
	T *end = this->imPtr(width_ - 1, height_ - 1);
	while (ptr <= end) {
		*ptr++ = val;
	}
}

template<class T>
ImageSource<T> *ImageSource<T>::copy() const {
	ImageSource<T> *im = new ImageSource<T>(width_, height_, false);
	memcpy(im->data, data, width_ * height_ * sizeof(T));

}


template<class T>
void ImageSource<T>::getMinMax(T *ret_min, T *ret_max) {

	T min = data[0];
	T max = data[0];
	for (int y = 0; y < height_; y++) {
		for (int x = 0; x < width_; x++) {
			T val = imRef(x, y);
			if (min > val)
				min = val;
			if (max < val)
				max = val;
		}
	}

	*ret_min = min;
	*ret_max = max;
}

template<class T>
ImageSource<uint8_t> * ImageSource<T>::threshold(int t) {
	ImageSource<uint8_t> *dst = new ImageSource<uint8_t>(width_, height_);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dst->imRef(x, y) = (imRef(x, y) >= t);
		}
	}

	return dst;
}

}

#endif /* __IMAGE_SOURCE_H__ */

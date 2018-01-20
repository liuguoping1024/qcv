#include <common/ImageSource.h>

namespace qcv {


#define	RED_WEIGHT	 0.299
#define GREEN_WEIGHT 0.587
#define BLUE_WEIGHT	 0.114

//TODO
ImageSource<uint8_t> * imageRGBtoGRAY(ImageSource<rgb> *input) {
	int width = input->width();
	int height = input->height();
	ImageSource<uint8_t> *output = new ImageSource<uint8_t>(width, height, false);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output->imRef(x, y) = (uint8_t) (input->imRef(x, y).r * RED_WEIGHT + input->imRef(x, y).g * GREEN_WEIGHT + input->imRef(x, y).b * BLUE_WEIGHT);
		}
	}
	return output;
}

ImageSource<rgb> *imageGRAYtoRGB(ImageSource<uint8_t> *input) {
	int width = input->width();
	int height = input->height();
	ImageSource<rgb> *output = new ImageSource<rgb>(width, height, false);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output->imRef(x, y).r = input->imRef(x, y);
			output->imRef(x, y).g = input->imRef(x, y);
			output->imRef(x, y).b = input->imRef(x, y);
		}
	}
	return output;
}


ImageSource<float> * imageUCHARtoFLOAT(ImageSource<uint8_t> *input) {
	int width = input->width();
	int height = input->height();
	ImageSource<float> *output = new ImageSource<float>(width, height, false);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output->imRef(x, y) = input->imRef(x, y);
		}
	}
	return output;
}

static void convolveEven(ImageSource<float>* src, ImageSource<float>* dst, std::vector<float>& mask)
{
	int width = src->width();
	int height = src->height();
	int len = mask.size();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float sum = mask[0] * src->imRef(x, y);
			for (int i = 1; i < len; i++) {
				sum += mask[i]* (src->imRef(std::max(x - i, 0), y)	+ src->imRef(std::min(x + i, width - 1), y));
			}
			dst->imRef(y, x) = sum;
		}
	}
}

#if 0
static void convolveOdd(ImageSource<float> *src, ImageSource<float> *dst, std::vector<float> &mask)
{
	int width = src->width();
	int height = src->height();
	int len = mask.size();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float sum = mask[0] * src->imRef(x, y);
			for (int i = 1; i < len; i++) {
				sum += mask[i] * (src->imRef(std::max(x - i, 0), y)	- src->imRef(std::min(x + i, width - 1), y));
			}
			dst->imRef(y, x) = sum;
		}
	}
}
#endif

/* normalize mask so it integrates to one */
static void normalize(std::vector<float> &mask) {
	int len = mask.size();
	float sum = 0;
	for (int i = 1; i < len; i++) {
		sum += fabs(mask[i]);
	}
	sum = 2 * sum + fabs(mask[0]);
	for (int i = 0; i < len; i++) {
		mask[i] /= sum;
	}
}


#define WIDTH 4.0

static std::vector<float> make_fgauss(float sigma) {
	sigma = std::max(sigma, 0.01F);
	int len = (int) ceil(sigma * WIDTH) + 1;
	std::vector<float> mask(len);
	for (int i = 0; i < len; i++) {
		mask[i] = exp(-0.5 * (i / sigma) * (i / sigma));
	}
	return mask;
}

/* convolve image with gaussian filter */
static ImageSource<float> * smooth(ImageSource<float> *src, float sigma) {
	std::vector<float> mask = make_fgauss(sigma);
	normalize(mask);

	ImageSource<float> *tmp = new ImageSource<float>(src->height(), src->width(), false);
	ImageSource<float> *dst = new ImageSource<float>(src->width(), src->height(), false);

	convolveEven(src, tmp, mask);

	convolveEven(tmp, dst, mask);

	delete tmp;
	return dst;
}

ImageSource<float> *smooth(ImageSource<uint8_t> *src, float sigma) {
	ImageSource<float> *tmp = imageUCHARtoFLOAT(src);

	ImageSource<float> *dst = smooth(tmp, sigma);
	delete tmp;
	return dst;
}

}

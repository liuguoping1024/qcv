#include <stdio.h>
#include <math.h>
#include <string.h>

#include "raw_image.h"

static void getHistogramInfo(const uint8_t* raw_data, const int width,
		const int height, const int channel, int * start_index, int * end_index) {
	int first_valid_index = -1;
	int last_valid_index = 0;

	unsigned int long_data_hisgram[256 * 256];
	memset(long_data_hisgram, 0, sizeof(int)*65536);

	int length = width * height;

	uint16_t * raw_data_area = (uint16_t *) raw_data;

	for (int i = 0; i < length; i++) {
		uint16_t v = *raw_data_area++;
		long_data_hisgram[v]++;
	}

	for (int i = 0; i < 65536; i++) {

		if (first_valid_index < 0) {
			if (long_data_hisgram[i] > 0) {
				first_valid_index = i;
				last_valid_index = i;
			}
		} else {
			if (long_data_hisgram[i] > 0) {
				last_valid_index = i;
			}
		}
	}

	*start_index = first_valid_index;
	*end_index = last_valid_index;
}

int convertRawToGray(const uint8_t* raw_data, const int width, const int height,
		const int channel, uint8_t * gray8) {
	if (channel != 2) {
		printf("Current format is not supported.\n");
		return -1;
	}

	int length = width * height;
	int start_valid_index = -1;
	int stop_valid_index = 0;

	getHistogramInfo(raw_data, width, height, channel, &start_valid_index, &stop_valid_index);

	double gray_scale = 255.0 / (stop_valid_index - start_valid_index);

	uint8_t * gray_area = gray8;
	uint16_t * raw_data_area = (uint16_t*) raw_data;

	for (int i = 0; i < length; i++) {
		ushort v = *raw_data_area++;

		v = v - start_valid_index;
		int v_gray = (int) (v * gray_scale);
		if (v_gray > 255)
			v_gray = 255;

		*gray_area = v_gray;

		gray_area++;
	}

	return 0;
}

int convertRawToGrayGamma(const uint8_t* raw_data, const int width, const int height, const int channel, uint8_t * gray8, float fGamma)
{
	if (channel != 2) {
		printf("Current format is not supported.\n");
		return -1;
	}

	int length = width * height;
	int start_valid_index = -1;
	int stop_valid_index = 0;

	getHistogramInfo(raw_data, width, height, channel, &start_valid_index, &stop_valid_index);

	double gray_scale = 255.0 / (stop_valid_index - start_valid_index);

	uint8_t lut[256];
	for (int i = 0; i < 256; i++) {
		int vGamma = (int) (pow((float) (i / 255.0), fGamma) * 255.0f);
		lut[i] = vGamma > 255 ? 255 : (uint8_t) vGamma;
	}


	uint8_t * gray_area = gray8;
	uint16_t * raw_data_area = (uint16_t*) raw_data;

	for (int i = 0; i < length; i++)
	{
		ushort v = *raw_data_area++;

		v = v - start_valid_index;
		int v_gray = (int) (v * gray_scale);
		if (v_gray > 255)
		{
			v_gray = 255;
		}

		*gray_area = (uint8_t) lut[v_gray];

		gray_area++;
	}

	return 0;
}

void applyGrayGamma(const uint8_t* gray_data, const int width, const int height, float fGamma)
{
	uint8_t lut[256];
	for (int i = 0; i < 256; i++) {
		int vGamma = (int) (pow((float) (i / 255.0), fGamma) * 255.0f);
		lut[i] = vGamma > 255 ? 255 : (uint8_t) vGamma;
	}

	int length = width * height;
	uint8_t * gray_area = (uint8_t *) gray_data;
	for (int i = 0; i < length; i++) {
		*gray_area = (uint8_t) lut[*gray_area];
		gray_area++;
	}
}

int scaleDownRawDataBy2(const uint16_t* raw_data, const int width,	const int height, uint16_t * raw_data_out) {
	if (width % 2 == 1 || height % 2 == 1) {
		return -1;
	}

	int new_width = width >> 1;
	int new_height = height >> 1;

	uint16_t * src_p = (uint16_t *)raw_data;
	uint16_t * dst_p = raw_data_out;
	for (int i = 0; i < new_height; i++) {
		uint16_t * src = src_p;
		uint16_t * dst = dst_p;

		for (int j = 0; j < new_width; j++) {
			uint32_t v = *src + *(src + 1) + *(src + width) + *(src + width + 1);
			* dst = (uint16_t)(v >> 2);

			src += 2;
			dst ++;
		}

		src_p += width << 1;
		dst_p += new_width;
	}

	return 0;
}

int scaleDownRawDataBy4(const uint16_t* raw_data, const int width,	const int height, uint16_t * raw_data_out) {
	if (width % 4 != 0 || height % 4 != 0) {
		return -1;
	}

	int new_width = width >> 2;
	int new_height = height >> 2;

	uint16_t * src_p = (uint16_t *)raw_data;
	uint16_t * dst_p = raw_data_out;
	for (int i = 0; i < new_height; i++) {
		uint16_t * src = src_p;
		uint16_t * dst = dst_p;

		for (int j = 0; j < new_width; j++) {
			uint32_t v = *src + *(src + 1) + *(src + 2)
					+ *(src + width) + *(src + width + 1) + *(src + width + 2)
					+ *(src + width + width) + *(src + width + width + 1) + *(src + width + width + 2);

			* dst = (uint16_t)(v >> 4);

			src += 4;
			dst ++;
		}

		src_p += width << 2;
		dst_p += new_width;
	}

	return 0;
}

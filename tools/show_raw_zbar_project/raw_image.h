#ifndef __RAW_DATA_IMAGE_H__
#define __RAW_DATA_IMAGE_H__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif



int convertRawToGray(const uint8_t* raw_data, const int width, const int height, const int channel, uint8_t * gray8);

int convertRawToGrayGamma(const uint8_t* raw_data, const int width, const int height, const int channel, uint8_t * gray8, float gamma);

void applyGrayGamma(const uint8_t* gray_data, const int width, const int height, float gamma);


int scaleDownRawDataBy2(const uint16_t* raw_data, const int width, const int height, uint16_t * raw_data_out);

int scaleDownRawDataBy4(const uint16_t* raw_data, const int width, const int height, uint16_t * raw_data_out);

#ifdef __cplusplus
}
#endif

#endif


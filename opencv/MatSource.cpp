/*
 *  Copyright 2010-2011 Alessandro Francescon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MatSource.h"

#include <common/ImageSource.h>
#include <exception/IllegalArgumentException.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

using qcv::boolean;

qcv::Ref<qcv::LuminanceSource> MatSource::create(cv::Mat & cvImage) {
    return qcv::Ref<LuminanceSource>(new MatSource(cvImage));
}


MatSource::MatSource(cv::Mat & _cvImage) : qcv::LuminanceSource(_cvImage.cols, _cvImage.rows) {
    cvImage = _cvImage.clone();
}

qcv::ArrayRef<char> MatSource::getRow(int y, qcv::ArrayRef<char> row) const {
    // Get width
    int width = getWidth();
    if (!row) {
        // Create row
        row = qcv::ArrayRef<char>(width);
    }

    // Get pointer to row
    const char *p = cvImage.ptr<char>(y);

    for(int x = 0; x < width; ++x, ++p) {

        // Set row at index x
        row[x] = *p;

    }

    return row;

}

qcv::ArrayRef<char> MatSource::getMatrix() const {

    // Get width and height
    int width = getWidth();
    int height =  getHeight();

    // Create matrix
    qcv::ArrayRef<char> matrix = qcv::ArrayRef<char>(width * height);

    for (int y = 0; y < height; ++y) {

        // Get pointer to row
        const char *p = cvImage.ptr<char>(y);

        // Calculate y offset
        int yoffset = y * width;

        for(int x = 0; x < width; ++x, ++p) {

            // Set row at index x with y offset
            matrix[yoffset + x] = *p;
        }
    }

    return matrix;
}

qcv::ImageSource<qcv::rgb> * MatSource::createImageRGBSource(cv::Mat & cvImage)
{
	if(cvImage.channels() != 3){
		return NULL;
	}

	int width = cvImage.cols;
	int height = cvImage.rows;
	int length = width * height;

	qcv::ImageSource<qcv::rgb> * image = new qcv::ImageSource<qcv::rgb>(width, height);
	uchar *p = cvImage.data;

	int off = 0;

	for (int i = 0; i < length; i++) {
		image->imPtr(i)->r = p[off++];
		image->imPtr(i)->g = p[off++];
		image->imPtr(i)->b = p[off++];
	}

	return image;
}

qcv::ImageSource<uint8_t> * MatSource::createImageSource(cv::Mat & cvImage)
{
	if(cvImage.channels() != 1){
		return NULL;
	}

	int width = cvImage.cols;
	int height = cvImage.rows;
	int length = width * height;

	qcv::ImageSource<uint8_t> * image = new qcv::ImageSource<uint8_t>(width, height);
	uchar *p = cvImage.data;

	for (int i = 0; i < length; i++) {
		image->imRef(i) = p[i];
	}


	return image;
}

void MatSource::createMat(cv::Mat & cvImage,
		qcv::ImageSource<uint8_t> & image) {
	cvImage.create(image.height(), image.width(), CV_8U);
	uchar *p = cvImage.data;
	memcpy(p, image.imPtr(0), image.height() * image.width());
}

void MatSource::createRGBMat(cv::Mat & cvImage, qcv::ImageSource<qcv::rgb> & image)
{
	cvImage.create(image.height(), image.width(), CV_8UC3);

	uchar *p = cvImage.data;
	memcpy(p, image.imPtr(0), image.height() * image.width()*3);
}

#ifndef __MAT_SOURCE_H_
#define __MAT_SOURCE_H_

#include <qcv.h>
#include <common/LuminanceSource.h>
#include <common/ImageSource.h>
#include <opencv2/core/core.hpp>

using qcv::boolean;

class MatSource : public qcv::LuminanceSource {
private:
    cv::Mat cvImage;

public:

    static qcv::Ref<qcv::LuminanceSource> create(cv::Mat & _cvImage);

    static qcv::ImageSource<qcv::rgb> * createImageRGBSource(cv::Mat & cvImage);

    static qcv::ImageSource<uint8_t> * createImageSource(cv::Mat & cvImage);

    static void createMat(cv::Mat & cvImage, qcv::ImageSource<uint8_t> & image);

    static void createRGBMat(cv::Mat & cvImage, qcv::ImageSource<qcv::rgb> & image);

    MatSource(cv::Mat & _cvImage);

    qcv::ArrayRef<char> getRow(int y, qcv::ArrayRef<char> row) const;
    qcv::ArrayRef<char> getMatrix() const;

};

#endif /* __MAT_SOURCE_H_ */

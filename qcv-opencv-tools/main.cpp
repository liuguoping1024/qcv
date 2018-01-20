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

#include <string>
#include <exception>
#include <stdlib.h>
#include <common/Counted.h>
#include <Binarizer.h>
//#include <MultiFormatReader.h>
#include <Result.h>
#include <exception/ReaderException.h>
#include <common/GlobalHistogramBinarizer.h>
#include <Exception.h>
#include <exception/IllegalArgumentException.h>
#include <BinaryBitmap.h>
#include <DecodeHints.h>
//#include <qrcode/QRCodeReader.h>
//#include <zxing/MultiFormatReader.h>

#include <segment/ImageSegment.h>

#include <MatSource.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sys/time.h>

using namespace std;
using namespace qcv;
//using namespace zxing::qrcode;
using namespace cv;

int poiTrackBar_c = 500;
int maxValue_c = 3000;

int poiTrackBar_r = 50;
int maxValue_r = 1000;

void onChangeTrackBar(int poi,void* usrdata);

void printUsage(char** argv) {

    // Print usage
    cout << "Usage: " << argv[0] << " [-d <DEVICE>] [-w <CAPTUREWIDTH>] [-h <CAPTUREHEIGHT>]" << endl
         << "Read QR code from given video device." << endl
         << endl;

}

static long get_tick()
{
	struct timeval tv;
	gettimeofday(&tv, 0);

	return (long)(tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0);
}

Point toCvPoint(Ref<ResultPoint> resultPoint) {
    return Point(resultPoint->getX(), resultPoint->getY());
}


int main(int argc, char** argv) {

	if (argc < 2) {
		printf("%s <image>\n", argv[0]);
		return -1;
	}

#if 0
    int deviceId = 0;
    int captureWidth = 640;
    int captureHeight = 480;
    bool multi = false;

    for (int j = 0; j < argc; j++) {

        // Get arg
        string arg = argv[j];

        if (arg.compare("-d") == 0) {

            if ((j + 1) < argc) {

                // Set device id
                deviceId = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing device id after -d" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-w") == 0) {

            if ((j + 1) < argc) {

                // Set capture width
                captureWidth = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing width after -w" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-h") == 0) {

            if ((j + 1) < argc) {

                // Set capture height
                captureHeight = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing height after -h" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-m") == 0) {

            // Set multi to true
            multi = true;

        }

    }

#endif


#if 0
    // Log
    cout << "Capturing from device " << deviceId << "..." << endl;

    // Open video captire
    VideoCapture videoCapture(deviceId);

    if (!videoCapture.isOpened()) {

        // Log
        cerr << "Open video capture failed on device id: " << deviceId << endl;
        return -1;

    }

    if (!videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, captureWidth)) {

        // Log
        cerr << "Failed to set frame width: " << captureWidth << " (ignoring)" << endl;

    }

    if (!videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, captureHeight)) {

        // Log
        cerr << "Failed to set frame height: " << captureHeight << " (ignoring)" << endl;

    }
#endif

    // The captured image and its grey conversion
    Mat image, grey;

    image = imread(argv[1], IMREAD_COLOR);

    // Open output window
    namedWindow("ZXing", cv::WINDOW_AUTOSIZE);
    namedWindow("ZXing-Grey", cv::WINDOW_AUTOSIZE);

    qcv::ImageSource<qcv::rgb> * new_image = MatSource::createImageRGBSource(image);

    createTrackbar ("c", "ZXing", &poiTrackBar_c, maxValue_c, onChangeTrackBar, new_image);
    createTrackbar ("r", "ZXing", &poiTrackBar_r, maxValue_r, onChangeTrackBar, new_image);


    long tick1 = get_tick();
    int num_ccs = 0;
    qcv::ImageSource<qcv::rgb> * seg = segment_image(new_image, 0.5, 500, 50, &num_ccs);

    long tick2 = get_tick();
    printf("got %d components\n", num_ccs);
    printf("done! uff...thats hard work. %ld ticks\n", (tick2 - tick1));

    //ImageSource<uint8_t> *new_image_gray = imageRGBtoGRAY(new_image);

    MatSource::createRGBMat(grey, *seg);
    delete seg;

#if 0
    // Stopped flag will be set to -1 from subsequent wayKey() if no key was pressed
    int stopped = -1;

    while (stopped == -1) {

        // Capture image
        bool result = videoCapture.read(image);

        if (result) {

            // Convert to grayscale
            cvtColor(image, grey, CV_BGR2GRAY);

            try {

                // Create luminance  source
                Ref<LuminanceSource> source = MatSource::create(grey);

                // Search for QR code
                Ref<Reader> reader;

                if (multi) {
                    reader.reset(new MultiFormatReader);
                } else {
                    reader.reset(new QRCodeReader);
                }

                Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
                Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
                Ref<Result> result(reader->decode(bitmap, DecodeHints(DecodeHints::TRYHARDER_HINT)));

                // Get result point count
                int resultPointCount = result->getResultPoints()->size();

                for (int j = 0; j < resultPointCount; j++) {

                    // Draw circle
                    circle(image, toCvPoint(result->getResultPoints()[j]), 10, Scalar( 110, 220, 0 ), 2);

                }

                // Draw boundary on image
                if (resultPointCount > 1) {

                    for (int j = 0; j < resultPointCount; j++) {

                        // Get start result point
                        Ref<ResultPoint> previousResultPoint = (j > 0) ? result->getResultPoints()[j - 1] : result->getResultPoints()[resultPointCount - 1];

                        // Draw line
                        line(image, toCvPoint(previousResultPoint), toCvPoint(result->getResultPoints()[j]), Scalar( 110, 220, 0 ),  2, 8 );

                        // Update previous point
                        previousResultPoint = result->getResultPoints()[j];

                    }

                }

                if (resultPointCount > 0) {

                    // Draw text
                    putText(image, result->getText()->getText(), toCvPoint(result->getResultPoints()[0]), FONT_HERSHEY_PLAIN, 1, Scalar( 110, 220, 0 ));

                }

            } catch (const ReaderException& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const zxing::IllegalArgumentException& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const zxing::Exception& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const std::exception& e) {
                cerr << e.what() << " (ignoring)" << endl;
            }

            // Show captured image
            imshow("ZXing", image);

            // Wait a key for 1 millis
            stopped = waitKey(1);

        } else {

            // Log
            cerr << "video capture failed" << endl;

        }

    }

    // Release video capture
    videoCapture.release();
#endif

    imshow("ZXing", image);
    imshow("ZXing-Grey", grey);

	while (true) {
		int c = waitKey();
		if ((char) c == 27)
			break;
	}

    delete new_image;

    return 0;
}

//回调函数
void onChangeTrackBar (int poi,void* usrdata)
{
	qcv::ImageSource<qcv::rgb> * new_image = (qcv::ImageSource<qcv::rgb> *)usrdata;

    int num_ccs = 0;
    qcv::ImageSource<qcv::rgb> * seg = segment_image(new_image, 0.5, poiTrackBar_c, poiTrackBar_r, &num_ccs);

    printf("got %d components\n", num_ccs);
    //ImageSource<uint8_t> *new_image_gray = imageRGBtoGRAY(new_image);
    Mat grey;
    MatSource::createRGBMat(grey, *seg);
    delete seg;

    imshow("ZXing-Grey", grey);
}

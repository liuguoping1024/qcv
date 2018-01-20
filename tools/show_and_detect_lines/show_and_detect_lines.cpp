#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

//164 226

void drawHisgram(Mat& input, Mat & display) {
	int gray_stat[256];
	memset(gray_stat, 0, sizeof(int) * 256);

	if (input.channels() != 1) {
		return;
	}

	int length = input.cols * input.rows;
	uchar * image = input.data;

	for (int i = 0; i < length; i++) {
		gray_stat[*image]++;
		image++;
	}

	int max_count = 0;
	for (int i = 0; i < 256; i++) {
		if (gray_stat[i] > max_count)
			max_count = gray_stat[i];
	}

	double scale = 256.0 / max_count;

	for (int i = 0; i < 256; i++) {
		int v = (int) (gray_stat[i] * 256.0 / max_count);
		Point pt1, pt2;
		pt1.x = i;
		pt1.y = 255 - v;

		pt2.x = i;
		pt2.y = 255;

		cv::line(display, pt1, pt2, cv::Scalar(255, 255, 255));
	}

	printf("max_count = %d\n", max_count);

	imshow("Histogram", display);

#if 0
	for (int i = 5; i < 256 - 5; i++) {

		if ((gray_stat[i - 1] >= gray_stat[i])
				&& (gray_stat[i - 2] >= gray_stat[i])
				&& (gray_stat[i - 3] >= gray_stat[i])
				&& (gray_stat[i - 4] >= gray_stat[i])
				&& (gray_stat[i - 5] >= gray_stat[i])
				&& (gray_stat[i + 1] >= gray_stat[i])
				&& (gray_stat[i + 2] >= gray_stat[i])
				&& (gray_stat[i + 3] >= gray_stat[i])
				&& (gray_stat[i + 4] >= gray_stat[i])
				&& (gray_stat[i + 5] >= gray_stat[i])) {
			printf("-- Valley Bottom index = %d, value = %d\n", i, gray_stat[i]);

		}

		if ((gray_stat[i - 1] <= gray_stat[i])
				&& (gray_stat[i - 2] <= gray_stat[i])
				&& (gray_stat[i - 3] <= gray_stat[i])
				&& (gray_stat[i - 4] <= gray_stat[i])
				&& (gray_stat[i - 5] <= gray_stat[i])
				&& (gray_stat[i + 1] <= gray_stat[i])
				&& (gray_stat[i + 2] <= gray_stat[i])
				&& (gray_stat[i + 3] <= gray_stat[i])
				&& (gray_stat[i + 4] <= gray_stat[i])
				&& (gray_stat[i + 5] <= gray_stat[i])) {
			printf("++ Valley Top index = %d, value = %d\n", i, gray_stat[i]);

		}
	}
#endif

}

void binary_proc(Mat& input, uchar thresh)
{
	if (input.channels() != 1) {
		return;
	}

	int length = input.cols * input.rows;
	uchar * image = input.data;

	for (int i = 0; i < length; i++) {
		if (*image >= thresh) {
			*image = 0xff;
		} else {
			*image = 0x00;
		}
		image++;
	}
}

int main(int argc, char** argv) {
	unsigned char data_array[1024 * 2];

	if (argc != 2) {
		printf("useage: %s <imagefile>\n ", argv[0]);
		return -1;
	}
	char* rawName = argv[1];

	Mat image = imread(rawName, IMREAD_UNCHANGED);

	std::vector<Mat> spl;

	cv::split(image, spl);

	if(image.channels() == 3){
		imshow("1", spl[0]);
		imshow("2", spl[1]);
		imshow("3", spl[2]);
	}

	imshow(rawName, image);

	Mat gray = image.clone();

	if (image.channels() != 1) {
		cv::cvtColor(gray, gray, CV_RGB2GRAY);

		imshow("gray", gray);
	}

	Mat display;
	display.create(256, 256, CV_8U);
	drawHisgram(gray, display);

	//Mat binary = gray.clone();
	///binary_proc(binary, 164);
	//imshow("binary", binary);


    cv::Mat mask;
    //cv::threshold(gray, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

    //adaptiveThreshold(gray, mask, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 49, 10);
	adaptiveThreshold(gray, mask, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 75, 10);
	cv::bitwise_not(mask, mask);

    imshow("mask", mask);

    // find contours (if always so easy to segment as your image, you could just add the black/rect pixels to a vector)
    std::vector<std::vector<cv::Point> > contours;
    //std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//cv::cvtColor(mask, mask, CV_GRAY2RGB);

    cv::Scalar colors[] = {
    		cv::Scalar(0, 0, 255),
			cv::Scalar(0, 255, 0),
			cv::Scalar(255, 0, 0),
			cv::Scalar(0, 255, 255),
			cv::Scalar(255, 0, 255),
			cv::Scalar(255,255, 0),
			cv::Scalar(0, 0, 0),
			cv::Scalar(255, 255, 255),
    		cv::Scalar(0, 0, 128),
			cv::Scalar(0, 128, 0),
			cv::Scalar(128, 0, 0),
			cv::Scalar(0, 128, 128),
			cv::Scalar(128, 0, 128),
			cv::Scalar(128,128, 0),
			cv::Scalar(128, 128, 128),
    };

    int index = 0;
	printf("contours.size() = %d\n", contours.size());
	for (int i = 0; i < contours.size(); i++) {

		cv::Scalar color = colors[index%15];

		std::vector<cv::Point> contour = contours[i];
		printf(" -- index %d = %d\n", i, contour.size());
		if(contour.size() < 20)
		{
			continue;
		}

		printf("Color = %d\n", index);
		for(int j=0; j<contour.size() - 1; j++)
		{
			cv::line(image, contour[j], contour[j+1], color, 3);
		}

		index ++;
	}

    imshow("new_mask", image);

/*
	Mat binary = image.clone();

	binary_proc(binary, 164);
	imshow("binary", binary);

	Mat threash;
	Mat threash1;
	Mat threash2;
	cv::Size size(3, 3);
	cv::GaussianBlur(image, threash, size, 0);

	adaptiveThreshold(threash, threash1, 255, ADAPTIVE_THRESH_MEAN_C,
			CV_THRESH_BINARY, 75, 10);

	imshow("threash1", threash1);


	vector<Vec4i> lines;
	HoughLinesP(binary, lines, 1, CV_PI / 180, 80, 100, 10);

	cv::cvtColor(binary, binary, CV_GRAY2RGB);

	for (int i = 0; i < lines.size(); i++) {
		Vec4i v = lines[i];

		Point pt1, pt2;
		pt1.x = v[0];
		pt1.y = v[1];

		pt2.x = v[2];
		pt2.y = v[3];

		cv::line(binary, pt1, pt2, cv::Scalar(0, 0, 255));
	}

	//namedWindow(rawName, CV_WINDOW_AUTOSIZE);

	imshow("lines", binary);
*/

#if 0
	adaptiveThreshold(threash, threash2, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
			CV_THRESH_BINARY, 75, 10);

	 cv::bitwise_not(threash2, threash2);
	imshow("threash2", threash2);

	/*
	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	Mat detect_edge;

	Canny( image, detect_edge, lowThreshold, lowThreshold*ratio, kernel_size );

	imshow("detect_edge", detect_edge);
	*/

	cv::bitwise_not(threash2, threash2);

	vector<Vec4i> lines;
	HoughLinesP(threash2, lines, 1, CV_PI / 180, 80, 100, 10);

	cv::cvtColor(threash2, threash2, CV_GRAY2RGB);

	for (int i = 0; i < lines.size(); i++) {
		Vec4i v = lines[i];

		Point pt1, pt2;
		pt1.x = v[0];
		pt1.y = v[1];

		pt2.x = v[2];
		pt2.y = v[3];

		cv::line(threash2, pt1, pt2, cv::Scalar(0, 0, 255));
	}

	//namedWindow(rawName, CV_WINDOW_AUTOSIZE);

	imshow("lines", threash2);
#endif

	waitKey(0);

	return 0;
}

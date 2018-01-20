#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;



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

//#define RAW_WIDTH 1024
//#define RAW_HEIGHT 768

#define RAW_WIDTH 2592
#define RAW_HEIGHT 1944

int main(int argc, char** argv) {
	unsigned char data_array[RAW_WIDTH * 2];

	unsigned int data_hisgram[256];
	unsigned int long_data_hisgram[256*256];

	if (argc != 2) {
		printf("useage: %s <imagefile>\n ", argv[0]);
		return -1;
	}
	char* rawName = argv[1];

	FILE* fp = fopen(rawName, "rb");
	if (fp == NULL) {
		printf("Fail to open file %s\n", rawName);
		return -1;
	}

	Mat image;
	image.create(RAW_HEIGHT, RAW_WIDTH, CV_8UC2);

	uchar * data_area = image.data;

	size_t n_read = 0;
	int lines = 0;

	do {
		n_read = fread(data_array, RAW_WIDTH * 2, 1, fp);
		if (n_read > 0) {
			memcpy(&data_area[RAW_WIDTH * 2 * lines], data_array, RAW_WIDTH * 2);
			lines++;
		}
	} while (n_read > 0);

	fclose(fp);

	memset(data_hisgram, 0, sizeof(int) * 256);
	memset(long_data_hisgram, 0, sizeof(int) * 65536);

	int length = RAW_HEIGHT * RAW_WIDTH;

	ushort * raw_data_area = (ushort *)data_area;

#if 0
	Mat gray_origin(768, 1024, CV_8UC1);
	uchar * gray_origin_area = (uchar *)gray_origin.data;
#endif

	for (int i = 0; i < length; i++) {
		ushort v = *raw_data_area++;
		long_data_hisgram[v]++;

		uchar vv = (v >> 8) & 0xff;

#if 0
		* gray_origin_area = vv;
		gray_origin_area ++;
#endif
		data_hisgram[vv]++;
	}

	int max_count = 0;
	for (int i = 0; i < 256; i++) {
		if (data_hisgram[i] > max_count)
			max_count = data_hisgram[i];
	}

#if 0
	imshow("gray_high_area", gray_origin);
#endif

	Mat display(256, 256, CV_8U);

	double scale = 256.0 / max_count;

	int first_valid_index = -1;
	int last_valid_index = 0;
#if 0
	for (int i = 0; i < 256; i++) {

		if (first_valid_index < 0) {
			if (data_hisgram[i] > 0) {
				first_valid_index = i;
				last_valid_index = i;
			}
		} else {
			if (data_hisgram[i] > 0) {
				last_valid_index = i;
			}
		}

		//printf("%5d: %2d\n", i, data_hisgram[i]);
		int v = (int) (data_hisgram[i] * 256.0 / max_count);
		Point pt1, pt2;
		pt1.x = i;
		pt1.y = 255 - v;

		pt2.x = i;
		pt2.y = 255;

		cv::line(display, pt1, pt2, cv::Scalar(255, 255, 255));
	}

	printf("max_count = %d\n", max_count);

	printf("first_valid_index = %d\n", first_valid_index);
	printf("last_valid_index = %d\n", last_valid_index);

	imshow("Histogram", display);
#endif

	first_valid_index = -1;
	last_valid_index = 0;

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

	printf("long_data_hisgram first_valid_index = %d(%x), gray = %d(%x)\n",
			first_valid_index, first_valid_index, (first_valid_index>>8),(first_valid_index>>8) );
	printf("long_data_hisgram last_valid_index = %d(%x), gray = %d(%x)\n",
			last_valid_index, last_valid_index, (last_valid_index>>8), (last_valid_index>>8));

	double gray_scale = 255.0 / (last_valid_index - first_valid_index);

	printf("gray_scale = %f\n", gray_scale);

	printf("gray_max = %f\n", gray_scale * (last_valid_index - first_valid_index));
	printf("gray_min = %f\n", gray_scale * (first_valid_index));

	float fGamma = 0.5;
	uchar lut[256];
	for (int i = 0; i < 256; i++) {
		lut[i] = saturate_cast<uchar>(pow((float) (i / 255.0), fGamma) * 255.0f);
	}

	Mat gray;
	gray.create(RAW_HEIGHT, RAW_WIDTH, CV_8UC1);

	length = RAW_HEIGHT * RAW_WIDTH;

	uchar * gray_area = gray.data;

	raw_data_area = (ushort*)image.data;
	for (int i = 0; i < length; i++) {
		ushort v = *raw_data_area++;

		v = v - first_valid_index;
		int v_gray = (int) (v * gray_scale);
		if(v_gray > 255) v_gray = 255;

		*gray_area = (uchar)lut[v_gray];

		gray_area++;
	}

	gray_area = gray.data;
	memset(data_hisgram, 0, sizeof(int) * 256);


	for (int i = 0; i < length; i++) {
		data_hisgram[*gray_area++]++;
	}

	max_count = 0;
	for (int i = 0; i < 256; i++) {
		if (data_hisgram[i] > max_count)
			max_count = data_hisgram[i];
	}

	Mat display2(256, 256, CV_8U);

	scale = 256.0 / max_count;
	for (int i = 0; i < 256; i++) {

		int v = (int) (data_hisgram[i] * 256.0 / max_count);
		Point pt1, pt2;
		pt1.x = i;
		pt1.y = 255 - v;

		pt2.x = i;
		pt2.y = 255;

		cv::line(display2, pt1, pt2, cv::Scalar(255, 255, 255));
	}
	imshow("Histogram2", display2);

	imshow("gray", gray);
	imwrite("/home/guoping/qr.png", gray);

	Mat gray_mask;
	//cv::equalizeHist(gray, gray_mask);

	adaptiveThreshold(gray, gray_mask, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 49, 10);

#if 0
	Mat element(3,3, CV_8U, Scalar(1));
	erode(gray_mask, gray_mask, element, Point(-1, -1));

	Mat element2(5,5, CV_8U, Scalar(1));
	dilate(gray_mask, gray_mask, element2, Point(-1, -1));
#endif

	imshow("gray_mask", gray_mask);

#if 0
	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;

	printf("lines: %d\n", lines);
	namedWindow(rawName, CV_WINDOW_AUTOSIZE);

	Mat detected_edges = image.clone();

	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

	namedWindow("canny", CV_WINDOW_AUTOSIZE);

	imshow(rawName, image);
	imshow("canny", detected_edges);

#endif

	waitKey(0);

	return 0;
}

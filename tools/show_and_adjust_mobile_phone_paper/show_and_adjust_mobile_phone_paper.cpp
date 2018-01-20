#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/calib3d/calib3d.hpp"

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

void featureDetection(Mat img_1, vector<Point2f>& points1) {
	vector<KeyPoint> keypoints_1;
	int fast_threshold = 150;
	bool nonmaxSuppression = true;
	FAST(img_1, keypoints_1, fast_threshold, nonmaxSuppression, FastFeatureDetector::TYPE_9_16);
	KeyPoint::convert(keypoints_1, points1, vector<int>());
}

void featureTracking(Mat& img_1, Mat& img_2, vector<Point2f>& points1,
		vector<Point2f>& points2, vector<uchar>& status) {

//this function automatically gets rid of points for which tracking fails

	vector<float> err;
	Size winSize = Size(21, 21);
	TermCriteria termcrit = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);

	calcOpticalFlowPyrLK(img_1, img_2, points1, points2, status, err, winSize,
			3, termcrit, 0, 0.001);
}

void compareImage(Mat & mat_templ, Mat & mat_motion, Mat & diff) {
	for (int j = 0; j < mat_templ.rows; ++j) {
		for (int i = 0; i < mat_templ.cols; ++i) {
			uchar tmpl_pixel = mat_templ.at<uchar>(j, i);
			uchar img_pixel = mat_motion.at<uchar>(j, i);

			if (tmpl_pixel != img_pixel) {
				if (img_pixel == 0xff) {
					diff.at<unsigned char>(j, i) = 0xff;
				} else {
					diff.at<unsigned char>(j, i) = 0;
				}
			} else {
				diff.at<unsigned char>(j, i) = 0;
			}

		}
	}
}

void filterImage(Mat & image) {

	uchar pixel;
	for (int j = 0; j < image.rows; ++j) {
		for (int i = 0; i < image.cols; ++i) {

			uchar img_pixel = image.at<uchar>(j, i);

			if(j < 2 || j >= image.rows-2 || i < 2 || i >= image.cols -2){
				image.at<uchar>(j, i) = 0;
			}

			int count = 0;
			if (img_pixel == 0xff) {

				//!< line -2
				pixel = image.at<uchar>(j - 2, i - 2);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 2, i - 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 2, i);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 2, i + 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 2, i + 2);
				if (pixel == 0xff)
					count++;

				//!< line -1
				pixel = image.at<uchar>(j - 1, i - 2);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 1, i - 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 1, i);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 1, i + 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j - 1, i + 2);
				if (pixel == 0xff)
					count++;

				//!< line
				pixel = image.at<uchar>(j, i - 2);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j, i - 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j, i + 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j, i + 2);
				if (pixel == 0xff)
					count++;

				//!< line +1
				pixel = image.at<uchar>(j + 1, i - 2);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 1, i - 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 1, i);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 1, i + 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 1, i + 2);
				if (pixel == 0xff)
					count++;

				//!< line +2
				pixel = image.at<uchar>(j + 2, i - 2);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 2, i - 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 2, i);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 2, i + 1);
				if (pixel == 0xff)
					count++;

				pixel = image.at<uchar>(j + 2, i + 2);
				if (pixel == 0xff)
					count++;

				if (count < 9) {
					image.at<uchar>(j, i) = 0;
				}

			}

		}
	}
}


void copyMask(Mat & templ, Mat & mask) {
	for (int j = 0; j < templ.rows; ++j) {
		for (int i = 0; i < templ.cols; ++i) {
			uchar tmpl_pixel = templ.at<uchar>(j, i);
			uchar mask_pixel = mask.at<uchar>(j, i);
			if(tmpl_pixel > 0x80)
			{
				mask.at<uchar>(j, i) = cv::GC_FGD;
			}
		}
	}
}

int main(int argc, char** argv) {
	unsigned char data_array[1024 * 2];

	if (argc != 3) {
		printf("useage: %s <imagefile>\n ", argv[0]);
		return -1;
	}
	char* rawName = argv[1];
	char* tempName = argv[2];

	Mat image = imread(rawName, IMREAD_UNCHANGED);
	Mat templ = imread(tempName, IMREAD_UNCHANGED);

	cv::Rect roi;
	roi.x = 180;
	roi.y = 560;

	roi.width = 400;
	roi.height = 640;

	Mat roi_image = image(roi);
	Mat roi_templ = templ(roi);


#if 1
	Mat image_gray, templ_gray;

	cvtColor(roi_image, image_gray, CV_BGR2GRAY);
	cvtColor(roi_templ, templ_gray, CV_BGR2GRAY);

	// feature detection, tracking
	vector<Point2f> points1, points2; //vectors to store the coordinates of the feature points
	featureDetection(templ_gray, points1);        //detect features in img_1
	featureDetection(image_gray, points2);        //detect features in img_1

	printf("points1: %d\n", points1.size());
	printf("points2: %d\n", points2.size());

	vector<uchar> status;
	featureTracking(templ_gray, image_gray, points1, points2, status); //track those features to img_2

	Mat h = findHomography(points2, points1);

	Mat roi_image_warp;
	Size size_image;
	size_image.height = roi_templ.rows;
	size_image.width = roi_templ.cols;
	warpPerspective(roi_image, roi_image_warp, h, size_image);

#if 1
	printf("status: %d\n", status.size());

	printf("points1: %d\n", points1.size());
	printf("points2: %d\n", points2.size());


	float total_x1 = 0.0f;
	float total_y1 = 0.0f;

	float total_x2 = 0.0f;
	float total_y2 = 0.0f;

	float l, t , b, r;

	l = r = points1[0].x;
	b = t = points1[0].y;


	Point pt;
	int avail_feature = 0;
	for (int i = 0; i < points1.size(); i++) {

		if (status[i] == 1 ) {

			pt.x = points1[i].x;
			pt.y = points1[i].y;

			total_x1 += points1[i].x;
			total_y1 += points1[i].y;

			if(points1[i].x > r) r = points1[i].x;
			if(points1[i].x < l) l = points1[i].x;

			if(points1[i].y < t) t = points1[i].y;
			if(points1[i].y > b) b = points1[i].y;

			//cv::circle(roi_templ, pt, 5, Scalar(0, 0, 255));

			pt.x = points2[i].x;
			pt.y = points2[i].y;

			total_x2 += points2[i].x;
			total_y2 += points2[i].y;
			//cv::circle(roi_image, pt, 5, Scalar(0, 255, 0));

			//printf("[%d]features: = (%f,%f) --> (%f,%f)\n", i, points1[i].x, points1[i].y, points2[i].x, points2[i].y);

			avail_feature ++;
		}
	}

	printf("Rect %f,%f --> %f,%f(%d x %d)\n", l, t, r, b, r-l, b-t);



	int offset = t / 2;
	Rect rt;
	rt.x = l - offset;
	rt.y = t - offset;
	rt.width = r - l + offset + offset;
	rt.height = b - t + offset + offset;


	printf("Rect(%d x %d)\n", rt.width, rt.height);

	roi_image = roi_image_warp(rt);
	roi_templ = roi_templ(rt);

	imshow(rawName, roi_image);
	imshow(tempName, roi_templ);

	//imwrite("/home/guoping/t/4.ppm", roi_image);
	//imwrite("/home/guoping/t/t.ppm", roi_templ);

#endif

	cv::Mat mask_image, mask_templ;

	cvtColor(roi_image, image_gray, CV_BGR2GRAY);
	cvtColor(roi_templ, templ_gray, CV_BGR2GRAY);

	//cv::Canny(image_gray, mask_image, 30, 100);
	//cv::Canny(templ_gray, mask_templ, 30, 100);

#if 1
	//Mat image_warp_gray;
	//cvtColor(roi_image_warp, image_warp_gray, CV_BGR2GRAY);

	cv::Size size(3, 3);
	cv::GaussianBlur(image_gray, image_gray, size, 0);
	cv::GaussianBlur(templ_gray, templ_gray, size, 0);

	adaptiveThreshold(image_gray, mask_image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 49, 10);
	//cv::bitwise_not(mask_image, mask_image);

	adaptiveThreshold(templ_gray, mask_templ, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 49, 10);
	//cv::bitwise_not(mask_templ, mask_templ);

	imshow("mask_image", mask_image);
	imshow("mask_templ", mask_templ);

	//cv::Mat diff1 = cv::Mat::zeros(roi_templ.rows, roi_templ.cols, CV_8UC1);
	cv::Mat diff2 = cv::Mat::zeros(roi_templ.rows, roi_templ.cols, CV_8UC1);

	//cv::absdiff(mask_templ, mask_image, diff1);

	compareImage(mask_templ, mask_image, diff2);

	//filterImage(diff1);
	filterImage(diff2);

	//imshow("differ1", diff1);
	imshow("differ2", diff2);

	cv::Mat grab_mask;
	grab_mask = cv::Mat(roi_image.rows, roi_image.cols, CV_8UC1, cv::Scalar(cv::GC_BGD));
	copyMask(diff2, grab_mask);

	cv::Rect rectangle(0, 0, roi_image.cols, roi_image.rows);

	//cv::Mat result;
	cv::Mat bgModel, fgModel;
	cv::grabCut(roi_image, grab_mask, rectangle, bgModel, fgModel, 4, cv::GC_INIT_WITH_MASK);

	cv::compare(grab_mask, cv::GC_FGD, grab_mask, cv::CMP_EQ);
	//result = result & 1 ;
	cv::Mat foreground(mask_image.size(), CV_8UC3, cv::Scalar(128, 128, 128));
	roi_image.copyTo(foreground, grab_mask);

	imshow("foreground", foreground);

#endif

#endif

	while (true) {
		int c = waitKey();
		if ((char) c == 27)
			break;
	}

	return 0;
}

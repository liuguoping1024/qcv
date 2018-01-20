#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

#if 0
int main(int argc, char * argv[]) {
	Mat3b src = imread(argv[1]);

	Mat1b gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);

	Mat1b thresh;
	threshold(gray, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// noise removal
	Mat1b kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat1b opening;
	morphologyEx(thresh, opening, MORPH_OPEN, kernel, Point(-1, -1), 2);

	Mat1b kernelb = getStructuringElement(MORPH_RECT, Size(21, 21));
	Mat1b background;
	morphologyEx(thresh, background, MORPH_DILATE, kernelb);
	background = ~background;

	// Perform the distance transform algorithm
	Mat1f dist_transform;
	distanceTransform(opening, dist_transform, CV_DIST_L2, 5);

	// Normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	normalize(dist_transform, dist_transform, 0, 1., NORM_MINMAX);

	// Threshold to obtain the peaks
	// This will be the markers for the foreground objects
	Mat1f dist_thresh;
	threshold(dist_transform, dist_thresh, 0.5, 1., CV_THRESH_BINARY);

	Mat1b dist_8u;
	dist_thresh.convertTo(dist_8u, CV_8U);

	// Find total markers
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Create the marker image for the watershed algorithm
	Mat1i markers(dist_thresh.rows, dist_thresh.cols, int(0));

	// Background as 1
	Mat1i one(markers.rows, markers.cols, int(1));
	bitwise_or(one, markers, markers, background);

	// Draw the foreground markers (from 2 up)
	for (int i = 0; i < int(contours.size()); i++)
	{
		drawContours(markers, contours, i, Scalar(i + 2), -1);
	}

	// Perform the watershed algorithm
	Mat3b dbg;
	cvtColor(opening, dbg, COLOR_GRAY2BGR);
	watershed(dbg, markers);

	Mat res;
	markers.convertTo(res, CV_8U);
	normalize(res, res, 0, 255, NORM_MINMAX);


	imshow("templ", res);
	imshow("templ_1", src);

	while (true) {
		int c = waitKey();
		if ((char) c == 27)
			break;
	}

	return 0;
}
#endif

#include "opencv2/opencv.hpp"
using namespace cv;

int main(int argc, char** argv)
{
    Mat3b img = imread(argv[1]);


	Mat1b gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat1b thresh;
	threshold(gray, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// noise removal
	Mat1b kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat1b opening;
	morphologyEx(thresh, opening, MORPH_OPEN, kernel, Point(-1, -1), 2);

	Mat1b kernelb = getStructuringElement(MORPH_RECT, Size(7, 7));

	Mat1b background;
	morphologyEx(thresh, background, MORPH_DILATE, kernelb);
	//background = ~background;


    imshow("img", img);
    imshow("background", background);
    waitKey();

    return 0;
}

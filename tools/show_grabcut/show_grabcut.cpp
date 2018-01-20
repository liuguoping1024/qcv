#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	printf("%s <image>\n", argv[0]);

	string filename = argv[1];
	Mat image = imread(filename, IMREAD_UNCHANGED);
	if (image.empty()) {
		cout << "\n Durn, couldn't read image filename " << filename << endl;
		return 1;
	}

	int rows = image.rows;
	int cols = image.cols;
	Rect rect(0, 0, cols, rows);

	cv::Mat mask;
	mask = cv::Mat(image.rows, image.cols, CV_8UC1, cv::Scalar(cv::GC_BGD));

	cv::Rect rectangle(50, 50, cols - 50, rows - 50);
	(mask(rectangle)).setTo(Scalar(GC_PR_FGD));

	//cv::Mat result;
	cv::Mat bgModel, fgModel;
	cv::grabCut(image, mask, rectangle, bgModel, fgModel, 4, cv::GC_INIT_WITH_MASK);

	//cv::grabCut(image, result, rectangle, bgModel, fgModel, 4,
	//		cv::GC_INIT_WITH_MASK);
#if 1
	cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
	//result = result & 1 ;
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(128, 128, 128));
	image.copyTo(foreground, mask);
#endif
	cvNamedWindow(filename.c_str(), CV_WINDOW_AUTOSIZE);
	imshow(filename.c_str(), image);

	cvNamedWindow("foreground", CV_WINDOW_AUTOSIZE);
	imshow("foreground", foreground);

	waitKey(0);
	return 0;
}

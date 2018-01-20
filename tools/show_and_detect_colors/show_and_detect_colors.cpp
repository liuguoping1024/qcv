
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

#if 0
static void help()
{
    cout <<
    "\nA program using pyramid scaling, Canny, contours, contour simpification and\n"
    "memory storage (it's got it all folks) to find\n"
    "squares in a list of images pic1-6.png\n"
    "Returns sequence of squares detected on the image.\n"
    "the sequence is stored in the specified memory storage\n"
    "Call:\n"
    "./squares\n"
    "Using OpenCV version %s\n" << CV_VERSION << "\n" << endl;
}


int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}


// the function draws all the squares in the image
static void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, LINE_AA);
    }

    imshow(wndname, image);
}
#endif

struct pixel{
	uchar b;
	uchar g;
	uchar r;
};

#define LEVEL (0x70)

static void findWhiteBlocks(Mat& image, Mat & fg) {

	printf("\n image: cn = %d, depth = %d \n" + image.channels(), image.depth());

	int width = image.rows;
	int height = image.cols;

	int sw = (width % 8) / 2;
	int sh = (height % 8) / 2;

	for (int y = 0; y < height; y += 1) {
		for (int x = 0; x < width; x += 1) {
			struct pixel p = image.at<struct pixel>(x, y);

			if ((p.r > LEVEL) && (p.g > LEVEL) && (p.b > LEVEL)) {

				int diff1 = p.r - p.b;
				int diff2 = p.g - p.b;
				int diff3 = p.r - p.g;

				diff1 = diff1 > 0 ? diff1 : -diff1;
				diff2 = diff2 > 0 ? diff2 : -diff2;
				diff3 = diff3 > 0 ? diff3 : -diff3;

				if (diff1 < 8 && diff2 < 8 && diff3 < 8) {
					fg.at<uchar>(x, y) = 0xff;
				}
			} else {
				fg.at<uchar>(x, y) = 0x10;
			}
		}
	}


#if 0
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
#endif

	//
}


int main(int argc, char** argv)
{
	if(argc != 3){
		//help();
		printf("Help: %s image1 temp1\n", argv[0]);
		return -1;
	}

    static const char* names[] = { "/home/guoping/t/1.ppm", 0 };
    //help();

    vector<vector<Point> > squares;

	Mat image = imread(argv[1], 1);
	if (image.empty()) {
		cout << "Couldn't load " << argv[1] << endl;
		return -1;
	}

	Mat templ = imread(argv[2], 1);
	if (templ.empty()) {
		cout << "Couldn't load " << argv[2] << endl;
		return -1;
	}

	cv::Mat image_gray;

	cvtColor(image, image_gray, CV_BGR2GRAY);
	imshow("image_gray", image_gray);

	cv::Mat image_1(image.size(), CV_8UC1, cv::Scalar(0, 0, 0));
	cv::Mat templ_1(image.size(), CV_8UC1, cv::Scalar(0, 0, 0));

	findWhiteBlocks(image, image_1);
	findWhiteBlocks(templ, templ_1);

#if 0
	Mat element(7,7,CV_8U, Scalar(1));

	erode(image_1, image_1, element, Point(-1, -1));
	erode(templ_1, templ_1, element, Point(-1, -1));

	dilate(image_1, image_1, Mat(), Point(-1, -1));
	dilate(templ_1, templ_1, Mat(), Point(-1, -1));
#endif

	imshow("image", image);
	imshow("image_1", image_1);

	imshow("templ", templ);
	imshow("templ_1", templ_1);

	while (true) {
		int c = waitKey();
		if ((char) c == 27)
			break;
	}

    return 0;
}

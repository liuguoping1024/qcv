#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <iomanip>

#include <sys/time.h>

#include "raw_image.h"

using namespace std;
using namespace cv;
using namespace zbar;

static long get_tick()
{
	struct timeval tv;
	gettimeofday(&tv, 0);

	return (long)(tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0);
}

static void printHelp(int argc, char ** argv)
{
	printf("Usage: %s [-w width(default:1024)] [-h height(default:768)] <raw image>\n", argv[0]);
}

int main(int argc, char **argv) {

	char* rawName = NULL;

	int width = 1024;
	int height = 768;

	//!< check argment for the command line.
	if (argc < 2) {
		printHelp(argc, argv);
		return -1;
	}

	int argc_index = 1;
	while (argc_index < argc) {
		if (argv[argc_index][0] == '-') {
			if (argc_index + 1 >= argc) {
				printf("Error parameters for command line [%s].\n",	argv[argc_index]);
				printHelp(argc, argv);
				return -1;
			}
			else
			{
				switch(argv[argc_index][1])
				{
				case 'w': argc_index ++; width = atoi(argv[argc_index]); break;
				case 'h': argc_index ++; height = atoi(argv[argc_index]); break;
				}
			}
		} else {
			if (rawName == NULL) {
				rawName = argv[argc_index];
			} else {
				printf("Error parameters for command line [%s].\n",	argv[argc_index]);
				printHelp(argc, argv);
				return -1;
			}
		}

		argc_index++;
	}

	if(rawName == NULL)
	{
		printf("No image file appointed.\n");
		printHelp(argc, argv);
		return -1;
	}

	//!< check the file and file format.
	FILE* fp = fopen(rawName, "rb");
	if (fp == NULL) {
		printf("Fail to open raw image file %s\n", rawName);
		return -1;
	}

	fseek(fp, 0l , SEEK_END);
	int total_size = (int)ftell(fp);

	int image_buffer_size = width * height * 2;
	if(total_size != image_buffer_size){
		printf("The image format cannot match the image file size.\n");
		fclose(fp);
		return -1;
	}

	//!< read the raw image file.
	fseek(fp, 0l , SEEK_SET);
	uint8_t * data_array = (uint8_t *)malloc(width * height * 2);

	size_t n_read = 0;
	int lines = 0;
	do {
		n_read = fread(&data_array[width * 2 * lines], width * 2, 1, fp);
		if (n_read > 0) {
			lines++;
		}
	} while (n_read > 0);

	fclose(fp);
	if (lines != height) {
		printf("Error in reading the raw image file, total %d bytes read.\n", (int)n_read);
		free(data_array);
		return -1;
	}

	//!----------------------------------------------------------------------------------------------

    // Create a zbar reader
    ImageScanner scanner;

    // Configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	namedWindow("Image", CV_WINDOW_AUTOSIZE);

	//!----------------------------------------------------------------------------------------------


	//!< origin dimension test
#if 0
	cv::Mat frame(height, width, CV_8UC1);

	convertRawToGrayGamma(data_array, width, height, 2, frame.data, 0.5);

	imshow("Image", frame);

	uchar *raw = (uchar *) (frame.data);

	// Wrap image data
	Image image(width, height, "Y800", raw, width * height);

#endif


	//!< half dimension test
#if 0
	int resized_width = width >> 1;
	int resized_height = height >> 1;
	uint8_t * resized_data_array = (uint8_t *)malloc(resized_width * resized_height * 2);
	scaleDownRawDataBy2((const uint16_t *)data_array, width, height, (uint16_t *)resized_data_array);

	cv::Mat frame(resized_height, resized_width, CV_8UC1);
	convertRawToGrayGamma(resized_data_array, resized_width, resized_height, 2, frame.data, 0.5);

	if(data_array != NULL){
		free(data_array);
		data_array = NULL;
	}

	imshow("Image", frame);

	uchar *raw = (uchar *) (frame.data);

	// Wrap image data
	Image image(resized_width, resized_height, "Y800", raw, resized_width * resized_height);

	if(resized_data_array != NULL){
		free(resized_data_array);
		resized_data_array = NULL;
	}

#endif

	//!< quarter dimension test
#if 1
	int resized_width = width >> 2;
	int resized_height = height >> 2;
	uint8_t * resized_data_array = (uint8_t *)malloc(resized_width * resized_height * 2);
	scaleDownRawDataBy4((const uint16_t *)data_array, width, height, (uint16_t *)resized_data_array);

	cv::Mat frame(resized_height, resized_width, CV_8UC1);
	convertRawToGrayGamma(resized_data_array, resized_width, resized_height, 2, frame.data, 0.5);

	if(data_array != NULL){
		free(data_array);
		data_array = NULL;
	}

	imshow("Image", frame);

	uchar *raw = (uchar *) (frame.data);

	// Wrap image data
	Image image(resized_width, resized_height, "Y800", raw, resized_width * resized_height);

	if(resized_data_array != NULL){
		free(resized_data_array);
		resized_data_array = NULL;
	}

#endif

	long tick1 = get_tick();
	// Scan the image for barcodes
	//int n = scanner.scan(image);
	if(scanner.scan(image) > 0)
	{
		printf("Scan success!\n");
	}
	else
	{
		printf("Scan Failed.\n");
	}
	long tick2 = get_tick();
	printf("QrCode scan time elapse %ld ms.\n", tick2 - tick1);

	// Extract results
	int counter = 0;
	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
		time_t now;
		tm *current;
		now = time(0);
		current = localtime(&now);

		// do something useful with results
		cout << "[" << current->tm_hour << ":" << current->tm_min << ":"
				<< setw(2) << setfill('0') << current->tm_sec << "] " << counter
				<< " " << "decoded " << symbol->get_type_name() << " symbol \""
				<< symbol->get_data() << '"' << endl;

		//cout << "Location: (" << symbol->get_location_x(0) << "," << symbol->get_location_y(0) << ")" << endl;
		//cout << "Size: " << symbol->get_location_size() << endl;

		// Draw location of the symbols found
		if (symbol->get_location_size() == 4) {
			//rectangle(frame, Rect(symbol->get_location_x(i), symbol->get_location_y(i), 10, 10), Scalar(0, 255, 0));
			line(frame,
					Point(symbol->get_location_x(0), symbol->get_location_y(0)),
					Point(symbol->get_location_x(1), symbol->get_location_y(1)),
					Scalar(0, 255, 0), 2, 8, 0);
			line(frame,
					Point(symbol->get_location_x(1), symbol->get_location_y(1)),
					Point(symbol->get_location_x(2), symbol->get_location_y(2)),
					Scalar(0, 255, 0), 2, 8, 0);
			line(frame,
					Point(symbol->get_location_x(2), symbol->get_location_y(2)),
					Point(symbol->get_location_x(3), symbol->get_location_y(3)),
					Scalar(0, 255, 0), 2, 8, 0);
			line(frame,
					Point(symbol->get_location_x(3), symbol->get_location_y(3)),
					Point(symbol->get_location_x(0), symbol->get_location_y(0)),
					Scalar(0, 255, 0), 2, 8, 0);
		}

		// Get points
		/*for (Symbol::PointIterator point = symbol.point_begin(); point != symbol.point_end(); ++point) {
		 cout << point << endl;
		 } */
		counter++;
	}

	// Show captured frame, now with overlays!
	imshow("captured", frame);

	// clean up
	image.set_data(NULL, 0);

	if(data_array != NULL){
		free(data_array);
	}

	while (true) {
		int c = waitKey();
		if ((char) c == 27)
			break;
	}

    return 0;
}

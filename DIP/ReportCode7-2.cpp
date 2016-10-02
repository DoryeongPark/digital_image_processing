#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>	

using namespace cv;
using namespace std;

int main() {
	//Load image
	Mat img = imread("stuff.jpg", CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {//If image wasn't loaded...
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}

	//Decreasesd Image size as 1/4
	resize(img, img, Size(img.cols / 2, img.rows / 2), 0, 0, CV_INTER_NN);

	//Convert RGB image into HSV image
	cvtColor(img, img, CV_RGB2HSV);
	
	//Shows HSV Image
	imshow("HSV_Image", img);

	vector<Mat> HSV_channels;
	//Splits Hue, Saturation, Value from HSV image
	split(img, HSV_channels);

	//Shows H, S, V Images
	imshow("H Image", HSV_channels[0]);
	imshow("S Image", HSV_channels[1]);
	imshow("V Image", HSV_channels[2]);

	Mat v_channel_equalized;

	//Equalize value channel
	equalizeHist(HSV_channels[2], v_channel_equalized);

	imshow("Equalized V Image", v_channel_equalized);

	//Change Value channel to Equalized value channel
	HSV_channels.pop_back();
	HSV_channels.emplace_back(v_channel_equalized);

	Mat hsv_result;

	//Gets HSV Image with merging Hue, Saturation, Value channels 
	merge(HSV_channels, hsv_result);
	
	imshow("HSV Image Result", hsv_result);//Shows result image

	Mat rgb_result;

	//Gets RGB Image converted from HSV Image
	cvtColor(hsv_result, rgb_result, CV_HSV2RGB);

	imshow("RGB Image Result", rgb_result);

	waitKey(0);

	//Stores Images
	imwrite("hsv_image.jpg", img);
	imwrite("value_equalized_hsv_image.jpg", hsv_result);
	imwrite("value_equalized_rgb_image.jpg", rgb_result);

	destroyAllWindows();
}
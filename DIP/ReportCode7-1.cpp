#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>	

using namespace cv;
using namespace std;

int main() {
	//Load Image
	Mat img = imread("canyon.jpg", CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {//If it wan't loaded..
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}
	//Decreasesd Image size as 1/9
	resize(img, img, Size(img.cols / 3, img.rows / 3), 0, 0, CV_INTER_NN);

	vector<Mat> RGB_channels;
	//Split R, G, B channels : Red = channel[2], Green = channel[1], Blue = channel[0]
	split(img, RGB_channels);

	//Shows each channels splited
	imshow("Red Channel", RGB_channels[2]);
	imshow("Green Channel", RGB_channels[1]);
	imshow("Blue Channel", RGB_channels[0]);

	//Image Variables for each Histogram - equalized channels
	Mat r_channel_equalized;
	Mat g_channel_equalized;
	Mat b_channel_equalized;

	//Equalize each channels
	equalizeHist(RGB_channels[2], r_channel_equalized);
	equalizeHist(RGB_channels[1], g_channel_equalized);
	equalizeHist(RGB_channels[0], b_channel_equalized);

	//Shows each channels equalized;
	imshow("Equalized Red Channel", r_channel_equalized);
	imshow("Equalized Green Channel", g_channel_equalized);
	imshow("Equalized Blue Channel", b_channel_equalized);

	Mat result;//Image variable for Result image

	vector<Mat> RGB_channels_equalized;

	//Insert each equalized channels
	RGB_channels_equalized.emplace_back(b_channel_equalized);
	RGB_channels_equalized.emplace_back(g_channel_equalized);
	RGB_channels_equalized.emplace_back(r_channel_equalized);
	
	//Merge it as 1 Image
	merge(RGB_channels_equalized, result);

	//Shows result Image
	imshow("Result", result);

	waitKey(0);
	
	//Stores Images
	imwrite("r_channel_equalized.jpg", r_channel_equalized);
	imwrite("g_channel_equalized.jpg", g_channel_equalized);
	imwrite("b_channel_equalized.jpg", b_channel_equalized);
	imwrite("composited_rgb_image.jpg", result);

	destroyAllWindows();
}
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>	

using namespace cv;
using namespace std;

int main() {

	Mat img_blurred;//Image blurred 
	Mat img_subtracted;//Result image of original image - blurred image.
	Mat img_readded;//Result image of original image + blurred image

	//Mask used for blurring
	Mat blurring_mask = (Mat_<float>(3, 3) << 0.11, 0.11, 0.11,
											  0.11, 0.11, 0.11,
											  0.11, 0.11, 0.11);

	//Original image
	Mat img = imread("chiltern_hill.jpg", CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {//If image wasn't loaded
		cout << "Can't load Image" << endl;
		exit(0);//exit program
	}
	
	//Resizes image 9 times smaller
	resize(img, img, Size(img.cols / 3, img.rows / 3), 0, 0, CV_INTER_NN);
	//Converts image into grayscale image.
	cvtColor(img, img, CV_BGR2GRAY);
	//Adjusts blurring mask made before to original image and gets blurred image. 
	filter2D(img, img_blurred, img.depth(), blurring_mask);
	//Gets result image of original image - blurred image.
	subtract(img, img_blurred, img_subtracted);
	//Gets result image of substracted image + original image.(It would be sharpen image.)
	add(img, img_subtracted, img_readded);
	
	//Shows Images
	imshow("Original Image", img);
	imshow("Blurred Image", img_blurred);
	imshow("Image Subtracted", img_subtracted);
	imshow("Image Readded", img_readded);
	
	waitKey(0);

	//Stores Images
	imwrite("chiltern_hill_gray.jpg", img);
	imwrite("Image_blurred.jpg", img_blurred);
	imwrite("Image_subtracted.jpg", img_subtracted);
	imwrite("Image_sharpened.jpg", img_readded);
	
	destroyAllWindows();
}
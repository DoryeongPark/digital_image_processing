#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>	

using namespace cv;
using namespace std;

int main() {
	
	Mat img_sharpened;//Image which is expected to be sharpened as theory.
	Mat img_blurred;//Image blurred
	Mat img_2times;//Original image + Original image
    
    //Mask used for blurring
	Mat blurring_mask = (Mat_<float>(3, 3) << 0.11, 0.11, 0.11,
											  0.11, 0.11, 0.11,
											  0.11, 0.11, 0.11);
	//Mask used for adding two original images.
	Mat original_2times_mask = (Mat_<float>(3, 3) << 0, 0, 0,
													 0, 2, 0,
													 0, 0, 0);
	//Load original image
	Mat img = imread("chiltern_hill.jpg", CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {//If image wasn't loaded.
		cout << "Can't load Image" << endl;
		exit(0);//exit program
	}

	//Resizes image 9 times smaller
	resize(img, img, Size(img.cols / 3, img.rows / 3), 0, 0, CV_INTER_NN);
	//Converts image into grayscale image.
	cvtColor(img, img, CV_BGR2GRAY);
	//Gets result image of original image + original image
	filter2D(img, img_2times, img.depth(), original_2times_mask);
	//Gets image blurred 
	filter2D(img, img_blurred, img.depth(), blurring_mask);
	//Gets result image of Original image + Original Image - blurred image
	subtract(img_2times, img_blurred, img_sharpened);
	
	//Shows Images
	imshow("Image Original", img);
	imshow("Image 2 times multiplied", img_2times);
	imshow("Image blurred", img_blurred);
	imshow("Image sharpened", img_sharpened);
	
	waitKey(0);

	//Stores Images
	imwrite("Image_2times_multiplied.jpg", img_2times);
	imwrite("Image_expected_to_be_sharpened.jpg", img_sharpened);

	destroyAllWindows();
}
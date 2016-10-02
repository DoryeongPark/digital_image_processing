/*
	ReportCode9-2.c
	Author : Doryeong Park
	SN	   : 2010112406
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

//Rotates Image by 30 degree
void rotate_by_30(Mat& img, const int n) noexcept{

	double angle = 30;

	Point2f img_center;
	Mat rotation_matrix;
	Rect bbox;

	for (int i = 0; i < n; ++i) {

		//Rotation point located on center of Image
		img_center = Point2f{ img.cols / 2.0f, img.rows / 2.0f };

		//Rotation matrix created with angle and point
		rotation_matrix = getRotationMatrix2D(img_center, angle, 1.0);

		//Rectangle to be background of Rotated Image
		bbox = RotatedRect(img_center, img.size(), angle).boundingRect();

		//Adjusts new option to rotation matrix
		rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - img_center.x;
		rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - img_center.y;

		//Rotates Image
		warpAffine(img, img, rotation_matrix, bbox.size());
	}
}

int main() {

	//Loads Image
	Mat img = imread("stuff_color.jpg", CV_LOAD_IMAGE_UNCHANGED);

	/*
		img_manipulated : Image used to rotate frequently
		img_rotated_1time : Image rotated by 30 degree with 1time
		img_360_1time : Image rotated by 30 degree with 12 times
		img_subtracted : Result Image of Original Image - Result Image
	*/
	Mat img_manipulated, img_rotated_1time, img_360_1time, img_subtracted;

	//Rotating angle
	double angle = 30;
	
	if (img.empty()) {//If Image couldn't be loaded
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}

	Point2f img_center;
	Mat rotation_matrix;
	Rect bbox;

	//Decreases Original Image size 
	resize(img, img, Size(img.cols / 2, img.rows / 2), 0, 0, CV_INTER_NN);

	//Copies original image with 
	img.copyTo(img_manipulated);

	//Rotates image by 30degree with 1 time
	rotate_by_30(img_manipulated, 1);

	//Copy Image
	img_manipulated.copyTo(img_rotated_1time);

	imshow("Rotated Image with 1time", img_rotated_1time);

	//Rotates image by 30 degree with 11 times (total : 12 times)
	rotate_by_30(img_manipulated, 11);

	//Rectangle area to crop 
	Rect crop{(img_manipulated.cols - img.cols)/2, (img_manipulated.rows - img.rows)/2 , img.cols, img.rows };

	//Crop area with Rectangle(Same size with Original Image)
	img_360_1time = img_manipulated(crop);
	
	//Gets Substracted Image
	img_subtracted = img - img_360_1time;

	//Shows all images
	imshow("Original_Image", img);
	imshow("Image rotated by 360 degree with 1 time", img_360_1time);
	imshow("Subtracted Image", img_subtracted);

	waitKey(0);

	//Save Result Images
	imwrite("Rotated_12times.jpg", img_360_1time);
	imwrite("Image_substracted_from_rotated.jpg", img_subtracted);
	imwrite("Rotated_1time.jpg", img_rotated_1time);

	destroyAllWindows();
}


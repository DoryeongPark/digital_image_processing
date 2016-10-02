/*
	ReportCode9-1.c
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

int main() {
	
	Mat img = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR);

	/* 
		Image A - 1.5 times scaled image after rotated by 45 degree.
	    Image B - Image rotated by 45 degree deafter 1.5 times scaled.
		Image substracted = result of Image B - Image A.
	*/
	Mat img_A, img_B, img_subtracted;

	if (img.empty()) {//If it wasn't loaded...
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}

	//Decreases original image size
	resize(img, img, Size(img.cols / 3, img.rows / 3), 0, 0, CV_INTER_NN);
	
	//Copies original image to Image A and Image B
	img.copyTo(img_A);
	img.copyTo(img_B);

	//Rotatation angle
	double angle = 45;

	//Gets rotatation point located on center of image A
	Point2f img_A_center{ img_A.cols / 2.0f, img_A.rows / 2.0f };

	//Gets rotation matrix with angle and point
	Mat rotation_matrix = getRotationMatrix2D(img_A_center, angle, 1.0);

	//Rectangle to be background of Rotated Image
	Rect bbox = RotatedRect(img_A_center, img_A.size(), angle).boundingRect();

	//Adjusts new option to rotation matrix
	rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - img_A_center.x;
	rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - img_A_center.y;

	//Rotates Image A by 45 degree with rotation matrix
	warpAffine(img_A, img_A, rotation_matrix, bbox.size());

	//Scaling 1.5 times
	resize(img_A, img_A, Size(img_A.cols * 1.5, img_A.rows *1.5), 0, 0, INTER_CUBIC);

	//Shows Image A
	imshow("Image A", img_A);

	//Scaling 1.5 times
	resize(img_B, img_B, Size(img_B.cols * 1.5, img_B.rows * 1.5), 0, 0, INTER_CUBIC);
	
	//Gets rotation point located on center of image B
	Point2f img_B_center{ img_B.cols / 2.0f, img_B.rows / 2.0f };

	//Gets rotation matrix with angle and point
	rotation_matrix = getRotationMatrix2D(img_B_center, angle, 1.0);
	bbox = RotatedRect(img_B_center, img_B.size(), angle).boundingRect();

	//Adjusts new option to rotation matrix
	rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - img_B_center.x;
	rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - img_B_center.y;

	//Rotates Scaled Image B with rotation matrix
	warpAffine(img_B, img_B, rotation_matrix, bbox.size());
	
	//shows Image B
	imshow("Image B", img_B);

	//Subtract Image B from Image A
	img_subtracted = img_B - img_A;
	
	//Shows Image Subtracted
	imshow("Image subtracted", img_subtracted);
	
	waitKey(0);

	//Save Images as files
	imwrite("Image_A.jpg", img_A);
	imwrite("Image_B.jpg", img_B);
	imwrite("Image_subtracted.jpg", img_subtracted);
	
	destroyAllWindows();
}
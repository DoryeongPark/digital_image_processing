/*
*	ReportCode5-1.cpp
*	설명 : 두 이미지를 각각 기본 Binariation, Otzu Method Binaration, Adaptive Binaration을 해보고 결과를 파일로 저장한다.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>	

using namespace std;
using namespace cv;

int main() {
	
	Mat img1 = imread("apple.jpg", CV_LOAD_IMAGE_COLOR);//Load Image A
	Mat img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR);//Load Image B

	Mat img1_gray;//Image A Converted into GrayScale Image
	Mat img2_gray;//Image B Converted into GrayScale Image
	
	Mat img1_binary;//Image A Converted into Binary Image
	Mat img2_binary;//Image B Converted into Binary Image

	Mat img1_otsu;//Image A Converted with Otsu Method Binarization
	Mat img2_otsu;//Image B Converted with Otsu Method Binarization

	Mat img1_adaptive;//Image A Converted with Adaptive Binarization
	Mat img2_adaptive;//Image B Converted with Adaptive Binarization

	if (img1.empty() || img2.empty()) {//If Images failed to be loaded
		cout << "Can't load Image" << endl;
		exit(0);//Exit Program
	}
	//Set image size 2 times smaller with img2's size 
	resize(img1, img1, Size(img2.cols / 2, img2.rows / 2), 0, 0, CV_INTER_NN);
	resize(img2, img2, Size(img2.cols / 2, img2.rows / 2), 0, 0, CV_INTER_NN);
	
	//Convert Images into Grayscale Images (Get gray channel with 0.2126 * R + 0.7152 * G + 0.0722 * B)
	cvtColor(img1, img1_gray, CV_BGR2GRAY);
	cvtColor(img2, img2_gray, CV_BGR2GRAY);

	//Shows Grayscale Images
	imshow("Image A", img1_gray);
	imshow("Image B", img2_gray);

	//Convert Images with threshold value 127 into Binary Image 
	double binary_value_A = threshold(img1_gray, img1_binary, 127, 255, CV_THRESH_BINARY);
	double binary_value_B = threshold(img2_gray, img2_binary, 127, 255, CV_THRESH_BINARY);

	//Print Threshold value 127
	cout << "binary_value_A : " << binary_value_A << endl;
	cout << "binary_value_B : " << binary_value_B << endl << endl;

	//Show Binary Image
	imshow("A_binary_Image", img1_binary);
	imshow("B_binary_Image", img2_binary);

	//Convert Images with Otsu Method Binarization
	double otsu_value_A = threshold(img1_gray, img1_otsu, 0, 255, CV_THRESH_OTSU);
	double otsu_value_B = threshold(img2_gray, img2_otsu, 0, 255, CV_THRESH_OTSU);

	//Print Threshold value determined by Otsu method
	cout << "otsu_value_A : " << otsu_value_A << endl;
	cout << "otsu_value_B : " << otsu_value_B << endl;

	//Show Otsu Binary Image
	imshow("A_otsu_Image", img1_otsu);
	imshow("B_otsu_Image", img2_otsu);

	//Convert Images with Adaptive Binarization
	adaptiveThreshold(img1_gray, img1_adaptive, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21, 10);
	adaptiveThreshold(img2_gray, img2_adaptive, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21, 10);

	//Show Adaptive Binary Image
	imshow("A_adaptive_Image", img1_adaptive);
	imshow("B_adaptive_Image", img2_adaptive);

	waitKey(0);//wait for key

	//Store Binary Images
	imwrite("A_binary_Image.jpg", img1_binary);
	imwrite("B_binary_Image.jpg", img2_binary);

	//Store Otsu Binary Images
	imwrite("A_otsu_Image.jpg", img1_otsu);
	imwrite("B_otsu_Image.jpg", img2_otsu);

	//Store Adaptive Binary Images
	imwrite("A_adaptive_Image.jpg", img1_adaptive);
	imwrite("B_adaptive_Image.jpg", img2_adaptive);

	destroyAllWindows();//Distroy all windows
}

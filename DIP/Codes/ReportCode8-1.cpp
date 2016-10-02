#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>	

using namespace cv;
using namespace std;

int main() {

	Mat img1, img2;//Image 1 - Lenna.png, Image 2 - stuff_color.jpg

	Mat sobelx, sobely;//Image filtered by Sobel x , Image filtered by Sobel y 

	Mat gaussian_blurred1, gaussian_blurred2;//Images blurred by Gaussian filter

	/*	4 cases about Image1 and Image2

	    E1 - Sobel Edge Detection 
		E2 - Laplacian Edge Detection
		E3 - Canny Edge Detection
		E4 - Laplacian of Gaussian Edge Detection
	*/
	Mat img1_E1, img1_E2, img1_E3, img1_E4;
	Mat img2_E1, img2_E2, img2_E3, img2_E4;

	//Loads 2 Images
	img1 = imread("Lenna.png", CV_LOAD_IMAGE_COLOR);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR);

	if (img1.empty() || img2.empty()) {//If images are failed to be loaded...
		cout << "Can't load image" << endl;
		exit(0);
	}
	
	//Converts 2 images into grayscale images
	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);

	//Decreases Image 2's size 1/4
	resize(img2, img2, Size(img2.cols / 2, img2.rows / 2), 0, 0, CV_INTER_NN);

	//-----------------------Image1 - E1-----------------------

	
	Sobel(img1, sobelx, CV_8U, 1, 0);//Gets Image filtered by sobel x
	Sobel(img1, sobely, CV_8U, 0, 1);//Gets Image filtered by sobel y
	
	add(abs(sobelx), abs(sobely), img1_E1);//Adds 2 images to get result

	imshow("Image1_E1", img1_E1);

	//-----------------------Image2 - E1-----------------------

	Sobel(img2, sobelx, CV_8U, 1, 0);//Gets Image filtered by sobel x
	Sobel(img2, sobely, CV_8U, 0, 1);//Gets Image filtered by sobel y

	add(abs(sobelx), abs(sobely), img2_E1);//Adds 2 images to get result

	imshow("Image2_E1", img2_E1);

	//-----------------------Image1 - E2-----------------------
	
	Laplacian(img1, img1_E2, CV_16S, 3, 1, 0, BORDER_DEFAULT);//Adjusts Laplacian edge filter to image1
	convertScaleAbs(img1_E2, img1_E2);//Converts image into CV_8U

	imshow("Image1_E2", img1_E2);

	//-----------------------Image2 - E2-----------------------

	Laplacian(img2, img2_E2, CV_16S, 3, 1, 0, BORDER_DEFAULT);//Adjusts Laplacian edge filter to image2
	convertScaleAbs(img2_E2, img2_E2);//Converts image into CV_8U
	
	imshow("Image2_E2", img2_E2);

	//-----------------------Image1 - E3-----------------------

	Canny(img1, img1_E3, 50, 200, 3);//Adjusts Canny edge filter to image1
	imshow("Image1_E3", img1_E3);

	//-----------------------Image2 - E3-----------------------

	Canny(img2, img2_E3, 50, 200, 3);//Adjusts Canny edge filter to image2
	imshow("Image2_E3", img2_E3);

	//-----------------------Image1 - E4-----------------------

	GaussianBlur(img1, gaussian_blurred1, Size(3, 3), 0, 0, BORDER_DEFAULT);//Adjusts Gaussian blur filter to image1
	Laplacian(gaussian_blurred1, img1_E4, CV_16S, 3, 1, 0, BORDER_DEFAULT);//Adjusts Laplacian edge filter
	convertScaleAbs(img1_E4, img1_E4);//Converts image into CV_8U
	imshow("Image1_E4", img1_E4);
	
	//-----------------------Image2 - E4-----------------------
	
	GaussianBlur(img2, gaussian_blurred2, Size(3, 3), 0, 0, BORDER_DEFAULT);//Adjusts Gaussian blur filter to image1
	Laplacian(gaussian_blurred2, img2_E4, CV_16S, 3, 1, 0, BORDER_DEFAULT);//Adjusts Laplacian edge filter
	convertScaleAbs(img2_E4, img2_E4);
	imshow("Image2_E4", img2_E4);

	waitKey(0);

	//Stores all result images

	imwrite("Image1_E1.jpg", img1_E1);
	imwrite("Image2_E1.jpg", img2_E1);
	imwrite("Image1_E2.jpg", img1_E2);
	imwrite("Image2_E2.jpg", img2_E2);
	imwrite("Image1_E3.jpg", img1_E3);
	imwrite("Image2_E3.jpg", img2_E3);
	imwrite("Image1_E4.jpg", img1_E4);
	imwrite("Image2_E4.jpg", img2_E4);

	destroyAllWindows();
}
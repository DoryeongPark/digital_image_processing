#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	Mat img1, img2;

	//이미지들을 그레이스케일 이미지로 가져온다.
	img1 = imread("tophat.PNG", CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty() || img2.empty())
		cout << "Images couldn't be loaded" << endl;

	//stuff_color.jpg는 사이즈가 너무 크므로 4배 줄인다.
	resize(img2, img2, Size{ img2.cols / 2,img2.rows / 2 }, 0, 0, CV_INTER_NN);

	//정사각형 모양의 151 * 151 크기 structuring element 생성
	auto se = getStructuringElement(MORPH_RECT, Size{ 151, 151 }, Point{ 75, 75 });

	//생성한 structuring element로 Image1, Image2에 Tophat(원본 이미지 - Opening(원본 이미지)효과 적용
	morphologyEx(img1, img1, CV_MOP_TOPHAT, se);
	morphologyEx(img2, img2, CV_MOP_TOPHAT, se);

	//Otsu threshold 적용
	threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
	threshold(img2, img2, 0, 255, CV_THRESH_OTSU);

	//Canny Edge 검출
	Canny(img1, img1, 125, 350);
	Canny(img2, img2, 125, 350);

	resize(img1, img1, Size{ img1.cols * 2,img1.rows * 2 }, 0, 0, CV_INTER_NN);
	resize(img2, img2, Size{ img2.cols * 2,img2.rows * 2 }, 0, 0, CV_INTER_NN);

	//3*3 사이즈의 십자가 모양 structuring element 생성
	auto se_new = getStructuringElement(MORPH_CROSS, Size{ 3, 3 }, Point{ 1, 1 });
	
	//해당 structuring element로 Opening 효과 적용
	morphologyEx(img1, img1, CV_MOP_OPEN, se_new);
	morphologyEx(img2, img2, CV_MOP_OPEN, se_new);
	
	//해당 structuring element로 Closing 효과 적용
	morphologyEx(img1, img1, CV_MOP_CLOSE, se_new);
	morphologyEx(img2, img2, CV_MOP_CLOSE, se_new);
	
	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-2(1).jpg", img1);
	imwrite("ReportCode10-2(2).jpg", img2);

	destroyAllWindows();
}
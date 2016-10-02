#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	Mat img1, img2;

	//�̹������� �׷��̽����� �̹����� �����´�.
	img1 = imread("tophat.PNG", CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty() || img2.empty())
		cout << "Images couldn't be loaded" << endl;

	//stuff_color.jpg�� ����� �ʹ� ũ�Ƿ� 4�� ���δ�.
	resize(img2, img2, Size{ img2.cols / 2,img2.rows / 2 }, 0, 0, CV_INTER_NN);

	//���簢�� ����� 151 * 151 ũ�� structuring element ����
	auto se = getStructuringElement(MORPH_RECT, Size{ 151, 151 }, Point{ 75, 75 });

	//������ structuring element�� Image1, Image2�� Tophat(���� �̹��� - Opening(���� �̹���)ȿ�� ����
	morphologyEx(img1, img1, CV_MOP_TOPHAT, se);
	morphologyEx(img2, img2, CV_MOP_TOPHAT, se);

	//Otsu threshold ����
	threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
	threshold(img2, img2, 0, 255, CV_THRESH_OTSU);

	//Canny Edge ����
	Canny(img1, img1, 125, 350);
	Canny(img2, img2, 125, 350);

	resize(img1, img1, Size{ img1.cols * 2,img1.rows * 2 }, 0, 0, CV_INTER_NN);
	resize(img2, img2, Size{ img2.cols * 2,img2.rows * 2 }, 0, 0, CV_INTER_NN);

	//3*3 �������� ���ڰ� ��� structuring element ����
	auto se_new = getStructuringElement(MORPH_CROSS, Size{ 3, 3 }, Point{ 1, 1 });
	
	//�ش� structuring element�� Opening ȿ�� ����
	morphologyEx(img1, img1, CV_MOP_OPEN, se_new);
	morphologyEx(img2, img2, CV_MOP_OPEN, se_new);
	
	//�ش� structuring element�� Closing ȿ�� ����
	morphologyEx(img1, img1, CV_MOP_CLOSE, se_new);
	morphologyEx(img2, img2, CV_MOP_CLOSE, se_new);
	
	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-2(1).jpg", img1);
	imwrite("ReportCode10-2(2).jpg", img2);

	destroyAllWindows();
}
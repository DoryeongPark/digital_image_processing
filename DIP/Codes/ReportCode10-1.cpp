#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	Mat img1, img2;
	Mat labels1, labels2;
	
	//�� �̹����� �׷��̽����� �������� �ε� �Ѵ�.
	img1 = imread("tophat.PNG", CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty() || img2.empty())
		cout << "Images couldn't be loaded" << endl;

	//stuff_color.jpg�� ũ�Ⱑ ũ�Ƿ� 4��� ���δ�.
	resize(img2, img2, Size{ img2.cols / 2,img2.rows / 2 }, 0, 0, CV_INTER_NN);

	//ũ�Ⱑ 151*151�� ���簢�� structuring element ����
	auto se = getStructuringElement(MORPH_RECT, Size{ 151, 151 }, Point{ 75, 75 });
	
	//������ structuring element�� ����Ͽ� Tophat ȿ�� ����
	morphologyEx(img1, img1, CV_MOP_TOPHAT, se);
	morphologyEx(img2, img2, CV_MOP_TOPHAT, se);
	
	//Otsu threshold ȿ�� ����
	threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
	threshold(img2, img2, 0, 255, CV_THRESH_OTSU);

	//�̹������� 0���� �������� �� �̹��� ������ŭ Labeling�� �����Ͽ� ���� labels�� �����Ѵ�.
	connectedComponents(img1, labels1, 8, CV_32S);
	connectedComponents(img2, labels2, 8, CV_32S);

	//�󺧸� �� �̹����� ������ �����ϱ� ���Ͽ� RGB�̹����� �ٲپ���. 
	cvtColor(img1, img1, CV_GRAY2BGR);
	cvtColor(img2, img2, CV_GRAY2RGB);

	//Image1�� "15�� �̹���"�� ���������� �ٲ۴�.
	for (int i = 0; i < labels1.cols; ++i) {
		for (int j = 0; j < labels1.rows; ++j) {
			if (labels1.at<int>(i, j) == 15) {
				img1.at<Vec3b>(i, j)[0] = 0;
				img1.at<Vec3b>(i, j)[1] = 0;
				img1.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}

	//Image2�� "1�� �̹���"�� �ʷϻ����� �ٲ۴�.
	for (int i = 0; i < labels2.rows; ++i) {
		for (int j = 0; j <	labels2.cols; ++j) {
			if (labels2.at<int>(i, j) == 1) {
				img2.at<Vec3b>(i, j)[0] = 0;
				img2.at<Vec3b>(i, j)[1] = 255;
				img2.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	
	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-1(1).jpg", img1);
	imwrite("ReportCode10-1(2).jpg", img2);

	destroyAllWindows();

}
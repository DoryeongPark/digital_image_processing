#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

constexpr double PI = 3.141592;

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

	//Line�� Rho�Ÿ��� ��Ÿ ������ ������ ���� ����
	vector<Vec2f> lines1, lines2;

	//Hough Transform�� ����Ͽ� ����� Line�� ���� ����(��ǥ �ִ� ���� : 180)
	HoughLines(img1, lines1, 1, PI / 180, 180);
	HoughLines(img2, lines2, 1, PI / 180, 180);

	//Image 1���� ����� Line���� �׸���.
	for (auto it = lines1.begin(); it != lines1.end(); ++it) {

		auto rho = (*it)[0]; //rho �Ÿ�
		auto theta = (*it)[1]; //��Ÿ ����

		if (theta < PI / 4. || theta > 3.*PI / 4.) {// ���� ��
			Point pt1(rho / cos(theta), 0);//Line�� �� ��1
			Point pt2((rho - img1.rows * sin(theta)) / cos(theta), img1.rows);//Line�� �� ��2

			line(img1, pt1, pt2, Scalar(255), 1);//�� ���� ���� ������ �Ͼ������ �׸���.
		}
		else {
			Point pt1(0, rho / sin(theta));//Line�� �� ��1
			Point pt2(img1.cols, (rho - img1.cols * cos(theta)) / sin(theta));//Line�� �� ��2

			line(img1, pt1, pt2, Scalar(255), 1);//�� ���� ���� ������ �Ͼ������ �׷���.
		}

	}

	//Image 2���� ����� Line���� �׸���.
	for (auto it = lines2.begin(); it != lines2.end(); ++it) {

		auto rho = (*it)[0]; //rho �Ÿ�
		auto theta = (*it)[1]; //��Ÿ ����

		if (theta < PI / 4. || theta > 3.*PI / 4.) {// ���� ��
			Point pt1(rho / cos(theta), 0);//Line�� �� ��1
			Point pt2((rho - img2.rows * sin(theta)) / cos(theta), img2.rows);//Line�� �� ��2

			line(img2, pt1, pt2, Scalar(255), 1);//�� ���� ���� ������ �Ͼ������ �׸���.
		}
		else {// ���� ��
			Point pt1(0, rho / sin(theta));//Line�� �� ��1
			Point pt2(img2.cols, (rho - img2.cols * cos(theta)) / sin(theta));//Line�� �� ��2

			line(img2, pt1, pt2, Scalar(255), 1);//�� ���� ���� ������ �Ͼ������ �׸���.
		}

	}

	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-3(1).jpg", img1);
	imwrite("ReportCode10-3(2).jpg", img2);

	destroyAllWindows();
}
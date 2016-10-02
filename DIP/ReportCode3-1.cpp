/*
*	ReportCode3-1.cpp
*	�ۼ���	:	�ڵ���
*	�й�		:	2010112406
*	����		:	LUT�� �ε��� �̹����� ��� �� ����� �����Ѵ�.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int bright;//�Է� ��Ⱚ(-255 ~ 255)
	int contrast;//�Է� ��ϰ�(-100 ~ 100)

	cout << "�̹����� Bright ����(�Է� : -255 ~ 255)" << endl;
	cout << "�Է� : ";

	cin >> bright;//��� �� �Է�

	cout << "�̹����� Contrast ����(�Է�(%) : -100 ~ 100)" << endl;
	cout << "�Է� : ";

	cin >> contrast;//��� �� �Է�

	auto ratio = (float)(1 + (float)contrast / 100);//��� ���� ���

	Mat image1 = imread("Lenna_gray.PNG");//�׷��̽����� �̹����� �ε��Ѵ�.
	Mat result;//��� �̹���
	image1.copyTo(result);//��� �̹����� �ε�� �̹����� �����Ѵ�.(�ʱ�ȭ��)

	Mat lkuptab{ 1, 256, CV_8U };//Lookup Table ����

	for (int i = 0; i < 256; i++) {//Lookup Table Set
		if ((float)(i + bright) * ratio > 255) //Clamping(255���� ū ���)
			lkuptab.at<uchar>(i) = 255;
		else if ((float)(i + bright) * ratio < 0) //Clamping(0���� ���� ���)
			lkuptab.at<uchar>(i) = 0;
		else //���� ��� i + bright * ratio = ���� �̹����� �Է��� ���� ��� ����
			lkuptab.at<uchar>(i) = (float)(i + bright) * ratio;
	}

	if (!image1.data) {//���� ����� �ε���� �ʾ�����
		cout << "Load Error" << endl;//���� ���� ���
		exit(1);//���α׷� ����
	}
	
	LUT(image1, lkuptab ,result); //��� �� ����� ������ ������̺�� ��� �̹����� �����.

	imshow("Adjusted Image", result);//�����츦 �̿��Ͽ� ��� �̹��� ���

	waitKey(0);// Waiting keyboard input

	destroyAllWindows();//��� ������ Destroy
}
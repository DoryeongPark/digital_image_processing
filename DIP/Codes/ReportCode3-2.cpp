/*
*	ReportCode3-2.cpp
*	�ۼ���	:	�ڵ���
*	�й�		:	2010112406
*	����		:	�ε��� �̹����� ����� �����Ѵ�.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int input; //�Է��� ��ϰ�

	cout << "�̹����� ��� ����(�Է�(%) : -100 ~ 100)" << endl;
	cout << "�Է� : ";

	cin >> input; //��ϰ��� �Է��Ѵ�.
	//�Է��� ��ϰ��� �´� ������ ����Ѵ�.
	auto ratio = (float)(1 + (float)input / 100);

	Mat image1 = imread("Lenna_gray.PNG");//�׷��̽����� �̹����� Matrix ���·� �ε��Ѵ�.
	Mat result;//��� �̹��� Matrix

	image1.copyTo(result);//�ε�� �̹����� �����Ͽ� ��� �̹����� �ִ´�.(�ʱ�ȭ��)

	if (!image1.data) {//�̹����� �ε��ϴ� ���� �����Ͽ��� ���
		cout << "Load Error" << endl;//���� ���� ���
		exit(1);//���α׷� ����
	}

	int row = image1.rows; //�̹����� ��
	int col = image1.cols; //�̹����� �� 

	for (int j = 0; j < row; j++) { //�̹����� �ุŭ �ݺ��� ����
		for (int i = 0; i < col; i++) { //�̹����� ����ŭ �ݺ��� ����(���� : ���� �Ʒ�)

			auto chBlue = (float)(image1.at<cv::Vec3b>(j, i)[0]); //�̹����� Blue Channel ����
			auto chGreen = (float)(image1.at<cv::Vec3b>(j, i)[1]); //�̹����� Green Channel ����
			auto chRed = (float)(image1.at<cv::Vec3b>(j, i)[2]); //�̹����� Red Channel ����

			auto ResultBlue = chBlue * ratio; //Blue Channel ���� �Է��� ������ŭ ���Ѵ�.
			auto ResultGreen = chGreen * ratio; //Green Channel ���� �Է��� ������ŭ ���Ѵ�.
			auto ResultRed = chRed * ratio; //Red Channel ���� �Է��� ������ŭ ���Ѵ�.

			//Clamping (��� 0����)
			if (ResultBlue < 0.0)
				ResultBlue = 0.0f;
			if (ResultGreen < 0.0)
				ResultGreen = 0.0f;
			if (ResultRed < 0.0)
				ResultRed = 0.0f;
			
			//Clamping (��� 255��)
			if (ResultBlue > 255.0)
				ResultBlue = 255.0f;
			if (ResultGreen > 255.0)
				ResultGreen = 255.0f;
			if (ResultRed > 255.0)
				ResultRed = 255.0f;

			result.at<cv::Vec3b>(j, i)[0] = ResultBlue;//����� Blue Channel�� ��� �̹����� ����
			result.at<cv::Vec3b>(j, i)[1] = ResultGreen;//����� Green Channel�� ��� �̹����� ����
			result.at<cv::Vec3b>(j, i)[2] = ResultRed;//����� Red Channel�� ��� �̹����� ����

		}
	}

	imshow("Image Adjusted", result); //��� �̹����� �����쿡 �����Ͽ� ���

	waitKey(0); // Waiting keyboard input

	destroyAllWindows(); //��� ������ destroy
}
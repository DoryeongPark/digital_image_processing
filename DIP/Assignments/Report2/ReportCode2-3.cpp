/*
*	ReportCode2-3.cpp
*	�ۼ���	: �ڵ���
*	�й�		: 2010112406
*	����		: �ܼ�â�� �Է��� ������ �� �̹����� ���´�.(Blending)
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int input;//�Է��� ����(�����)
	
	cout << "�� �̹����� Blending �� ������ �Է��Ͻʽÿ�.(�Է�(%) : 0 ~ 100)" << endl;
	cout << "�Է� : ";
	
	cin >> input;//����� �� �Է�

	auto ratio = (float)input / 100;//������ ��ȯ
	
	Mat image1 = imread("image.jpg");//�̹���1 Matrix Ŭ���� ���·� �ε�
	Mat image2 = imread("processed.jpg");//�̹���2 Matrix Ŭ���� ���·� �ε�
	Mat imageBlended;//�����

	image1.copyTo(imageBlended);//��� �̹��� �̹���1 ���� �ʱ�ȭ 
	
	if (!image1.data || !image2.data) {//�� �̹��� �� �ϳ� �̻��� �ε忡 �����Ͽ��� ���
		cout << "Load Error" << endl;//���� ���� ���
		exit(1);//���α׷� ����
	}
		
	int row = image1.rows;//�̹���1�� ���� ��
	int col = image1.cols;//�̹���1�� ���� ��

	for (int j = 0; j < row; j++) {//�̹����� �ุŭ �ݺ��� ����
		for (int i = 0; i < col; i++) {//�̹����� ����ŭ �ݺ��� ����(��� ���� : ���� �Ʒ�)
			
			auto chBlue1 = (float)(image1.at<Vec3b>(j, i)[0]);//Image1�� Blue Channel��
			auto chGreen1 = (float)(image1.at<Vec3b>(j, i)[1]);//Image1�� Green Channel��
			auto chRed1 = (float)(image1.at<Vec3b>(j, i)[2]);//Image1�� Red Channel��

			auto chBlue2 = (float)(image2.at<Vec3b>(j, i)[0]);//Image2�� Blue Channel��
			auto chGreen2 = (float)(image2.at<Vec3b>(j, i)[1]);//Image2�� Green Channel��
			auto chRed2 = (float)(image2.at<Vec3b>(j, i)[2]);//Image2�� Red Channel��

			auto BlendedBlue = (chBlue1 * ratio + chBlue2 * (1 - ratio));//image1�� image2�� Blue Channel���� �Է��� ������ŭ Blending
			auto BlendedGreen = (chGreen1 * ratio + chGreen2 * (1 - ratio));//image1�� image2�� Green Channel���� �Է��� ������ŭ Blending
			auto BlendedRed = (chRed1 * ratio + chRed2 * (1 - ratio));//image1�� image2�� Red Channel���� �Է��� ������ŭ Blending

			if (BlendedBlue < 0.0)//Blue Channel Clamping
				BlendedBlue = 0.0f;
			if (BlendedGreen < 0.0)//Green Channel Clamping
				BlendedGreen = 0.0f;
			if (BlendedRed < 0.0)//Red Channel Clamping
				BlendedRed = 0.0f;

			if (BlendedBlue > 255.0)//Blue Channel Clamping
				BlendedBlue = 255.0f;
			if (BlendedGreen > 255.0)//Green Channel Clamping
				BlendedGreen = 255.0f;
			if (BlendedRed > 255.0)//Red Channel Clamping
				BlendedRed = 255.0f;

			imageBlended.at<Vec3b>(j, i)[0] = BlendedBlue;//���� Blue Channel �̹��� ����
			imageBlended.at<Vec3b>(j, i)[1] = BlendedGreen;//���� Green Channel �̹��� ����
			imageBlended.at<Vec3b>(j, i)[2] = BlendedRed;//���� Red Channel �̹��� ����

		}
	}
	
	imshow("Image Blended", imageBlended);//��� �̹��� ���
	
	waitKey(0);// Waiting keyboard input

	destroyAllWindows();//��� ������ Destroy
}
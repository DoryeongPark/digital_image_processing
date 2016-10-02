#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace std;
using namespace cv;

int main() {

	//���� ���� ���
	string path = "video data for design hw.avi";

	//USB ī�޶� ��ü�� �����Ѵ�.
	VideoCapture camera{ path };

	Mat original_img; //�о���� �� ����
	Mat pattern_img = imread("blue_ball.jpg", CV_LOAD_IMAGE_COLOR); //���� �̹����� �ε��Ѵ�
	Mat corr_img; //���� �̹������� ��� ����� �ݿ��� �̹���(��ġ�ϴ� ���� ���� ����)

	Point min_point;//��� ����� ���� ���� ����
	Point match_point;//��� ����� ���� ���� ���� 

	double min, max; //�ִ밪�� �ּҰ��� ã�� ���� ����

	if (pattern_img.empty()) {
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}

	//���콺 �̹����� 1/9 ũ��� ���δ�
	resize(pattern_img, pattern_img, Size(pattern_img.cols / 3,
		pattern_img.rows / 3), 0, 0, CV_INTER_NN);

	//���� �̹����� â���� ����
	imshow("Pattern Image", pattern_img);

	if (!camera.isOpened()) {
		cout << "Camera couldn't be loaded." << endl;
		exit(0);
	}

	//ī�޶� ������ ���������� �о���δ�.
	while (camera.isOpened()) {

		//ī�޶�� �о� ���� ������ original_img�� �����Ѵ�
		camera.read(original_img);

		//���� �̹����� �� ������ ������ ������ ���Ѵ�
		matchTemplate(original_img, pattern_img, corr_img,
			CV_TM_CCOEFF_NORMED);

		//�������� �ִ��� ������ ã�´� : match_point
		minMaxLoc(corr_img, &min, &max, &min_point, &match_point);

		//match_point�� ��Ī�ϴ� ������ ���� �� �����̴�
		//���� ������ ���� �������� ���� ũ�⸸ŭ �׸���.
		rectangle(original_img, Rect{ match_point.x, match_point.y,
			pattern_img.cols, pattern_img.rows },
			Scalar{ 0, 0, 255 }, 3);

		//�ִ� ������ �ʷϻ����� ǥ���Ѵ�
		circle(original_img, match_point,
			3, Scalar(0, 255, 0), CV_FILLED);

		//������ ����Ѵ�.
		imshow("Original Image", original_img);
		imshow("Corr Image", corr_img);

		waitKey(1);
	}

}
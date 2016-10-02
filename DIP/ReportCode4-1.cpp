#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<iomanip>

using namespace std;
using namespace cv;

//�ش� �̹����� ������׷��� â���� ����ϰ� ������׷� �̹����� ��ȯ�Ѵ�.
Mat make_histogram_window(Mat& img, string s) {

	int histSize = 256;//������׷��� ���� ������
	
	double total;
	total = img.rows * img.cols;

	int hist_w = 512;//������׷��� ���� ����
	int hist_h = 500;//������׷��� ����
	int bin_w = cvRound((double)hist_w / histSize);//�Ǽ��� ������ �ݿø��Ѵ�.

	Mat histImage(hist_h, hist_w, CV_8U, Scalar(0, 0, 0));//������׷��� ǥ���� �̹���

	for (int i = 0; i < histSize; i++) {//���η� ����׷����� �׷�������. (���� ���� 100���� ������ ǥ��)
		line(histImage, Point(bin_w * (i + 1), hist_h - img.at<ushort>(i)/100),
			Point(bin_w * (i + 1), hist_h), CV_RGB(255, 255, 255));
	}

	imshow(s, histImage);//�ϼ��� ������׷� �̹����� ���δ�.

	return histImage;//������׷��� �̹����� �����Ѵ�.

}

int main() {

	Mat img1; //For �̹���1
	Mat img1_src = imread("mylove.jpg", CV_LOAD_IMAGE_COLOR);//�̹����� �ε��Ѵ�.
	Mat img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR); //�̹����� �ε��Ѵ�. For �̹���2

	if (img1_src.empty() || img2.empty()) {
		cout << "Failed to load image" << endl;//���� �� �� �ϳ��� �ε���� ���� ��� ���α׷��� �����Ѵ�.
		exit(0);
	}
	//mylove.jpg�� ����� �ʹ� ũ�Ƿ� ���ο� ���θ� 1/3�� �ٿ��� �������Ͽ� img1�� �ִ´�.
	resize(img1_src, img1, Size(img1_src.cols / 3, img1_src.rows / 3), 0, 0, CV_INTER_NN);

	//�׷��̽����� ��ȯ
	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);
	
	//������׷� ����(1 ~ 255�� �迭<usigned short>)
	Mat img1_histo{ 1, 256, CV_16U };
	Mat img2_histo{ 1, 256, CV_16U };
	
	//������׷� ��� 0���� �ʱ�ȭ
	for (int i = 0; i < 256; i++)
		img1_histo.at<ushort>(i) = 0;
	
	for (int i = 0; i < 256; i++)
		img2_histo.at<ushort>(i) = 0;

	auto img1_row = img1.rows;//�̹���1�� ���� �ȼ� ���� �ҷ��´�.
	auto img1_col = img1.cols;//�̹���1�� ���� �ȼ� ���� �ҷ��´�.

	//������׷��� ������ �� ����
	for (int i = 0; i < img1_row; ++i) {
		for (int j = 0; j < img1_col; ++j) {
			auto black = static_cast<int>(img1.at<uchar>(i, j));//�� �ȼ��� �� ����
			img1_histo.at<ushort>(black)++;//�ش� �ȼ��� �ε����� �� ����
		}
	}

	auto img2_row = img2.rows;//�̹���2�� ���� �ȼ� ���� �ҷ��´�.
	auto img2_col = img2.cols;//�̹���2�� ���� �ȼ� ���� �ҷ��´�.

	for (int i = 0; i < img2_row; ++i) {
		for (int j = 0; j < img2_col; ++j) {
			auto black = static_cast<int>(img2.at<uchar>(i, j));//�� �ȼ��� �� ����
			img2_histo.at<ushort>(black)++;//�ش� �ȼ��� �ε����� �� ����
		}
	}
	int accum = 0;
	//�̹���1�� ������׷��� ����Ѵ�.
	cout << "mylove.jpg - original" << endl << endl;
	auto row = 0; auto i = 0; auto flag = 0;
	while (flag < 256) {
		
		cout.setf(ios::left);
		cout << setw(4) << i << setw(3) << " : " << setw(7) <<static_cast<int>(img1_histo.at<ushort>(i));
		accum = accum + static_cast<int>(img1_histo.at<ushort>(i));
		i += 40; ++flag;

		if (i > 255) {
			cout << endl;
			i = ++row;
		}
	}
	cout << "�� �ȼ� ��: " << accum << endl;
	accum = 0;
	//�̹���2�� ������׷��� ����Ѵ�.
	cout << endl << endl;
	cout << "stuff_color.jpg - original" << endl << endl;

	row = 0; i = 0; flag = 0;
	while (flag < 256) {

		cout.setf(ios::left);
		cout << setw(4) << i << setw(3) << " : " << setw(7) << static_cast<int>(img2_histo.at<ushort>(i));
		accum = accum + static_cast<int>(img2_histo.at<ushort>(i));
		i += 40; ++flag;

		if (i > 255) {
			cout << endl;
			i = ++row;
		}
	}
	cout << "�� �ȼ� ��:" <<  accum << endl;

	cout << endl << endl;

	//�̹���1, �̹���2 â���� ���
	imshow("mylove.jpg - original", img1);
	imshow("stuff_color.jpg - original", img2);

	//�̹���1, �̹���2 ������׷� â ����
	Mat img1_og_hist_img = make_histogram_window(img1_histo, "mylove.jpg - Histogram original");
	Mat img2_og_hist_img = make_histogram_window(img2_histo, "stuff_color.jpg - Histogram original");

	Mat img1_hist_equalized; //Histogram Equalization �� �̹���1
	Mat img2_hist_equalized; //Histogram Equalization �� �̹���2

	//Histogram Equalization
	equalizeHist(img1, img1_hist_equalized);
	equalizeHist(img2, img2_hist_equalized);


	//������׷� �ʱ�ȭ
	for (int i = 0; i < 256; i++)
		img1_histo.at<ushort>(i) = 0;

	for (int i = 0; i < 256; i++)
		img2_histo.at<ushort>(i) = 0;

	
	//������׷��� ���� ����
	for (int i = 0; i < img1_row; ++i) {
		for (int j = 0; j < img1_col; ++j) {
			auto black = static_cast<int>(img1_hist_equalized.at<uchar>(i, j));//�� �ȼ��� �� ����
			img1_histo.at<ushort>(black)++;//�ش� �ȼ��� �ε����� �� ����
		}
	}

	for (int i = 0; i < img2_row; ++i) {
		for (int j = 0; j < img2_col; ++j) {
			auto black = static_cast<int>(img2_hist_equalized.at<uchar>(i, j));//�� �ȼ��� �� ����
			img2_histo.at<ushort>(black)++;//�ش� �ȼ��� �ε����� �� ����
		}
	}
	accum = 0;
	//�̹���1�� ������׷��� ����Ѵ�.
	cout << "mylove.jpg - equalized" << endl << endl;
	row = 0; i = 0; flag = 0;
	while (flag < 256) {

		cout.setf(ios::left);
		cout << setw(4) << i << setw(3) << " : " << setw(7) << static_cast<int>(img1_histo.at<ushort>(i));
		accum = accum + static_cast<int>(img1_histo.at<ushort>(i));
		i += 40; ++flag;

		if (i > 255) {
			cout << endl;
			i = ++row;
		}
	}
	cout << "�� �ȼ� �� : " << accum << endl;
	accum = 0;
	//�̹���2�� ������׷��� ����Ѵ�.
	cout << endl << endl;
	cout << "stuff_color.jpg - equalized" << endl << endl;
	row = 0; i = 0; flag = 0;
	while (flag < 256) {

		cout.setf(ios::left);
		cout << setw(4) << i << setw(3) << " : " << setw(7) << static_cast<int>(img2_histo.at<ushort>(i));
		accum = accum + static_cast<int>(img2_histo.at<ushort>(i));

		i += 40; ++flag;

		if (i > 255) {
			cout << endl;
			i = ++row;
		}
	}

	cout << "�� �ȼ� �� : " << accum << endl;

	cout << endl;
	
	//Equalized �� �̹���1, �̹���2 â���� ���
	imshow("mylove.jpg - equalized",img1_hist_equalized);
	imshow("stuff_color.jpg - equalized", img2_hist_equalized);

	//�̹���1, �̹���2 ������׷� â���� ��� �� ����
	Mat img1_eq_hist_img = make_histogram_window(img1_histo, "mylove.jpg - Histogram equalized");
	Mat img2_eq_hist_img = make_histogram_window(img2_histo, "stuff_color.jpg - Histogram equalized");
	
	waitKey(0);

	//Save original images
	imwrite("A_originalImg.jpg", img1);
	imwrite("B_originalImg.jpg", img2);

	//Save equalized images
	imwrite("A_equalizedImg.jpg", img1_hist_equalized);
	imwrite("B_equalizedImg.jpg", img2_hist_equalized);

	//Save original histogram
	imwrite("A_originalHist.jpg", img1_og_hist_img);
	imwrite("B_originalHist.jpg", img2_og_hist_img);

	//Save equalized histogram
	imwrite("A_equalizedHist.jpg", img1_eq_hist_img);
	imwrite("B_equalizedHist.jpg", img2_eq_hist_img);

	destroyAllWindows();
}


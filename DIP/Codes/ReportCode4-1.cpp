#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<iomanip>

using namespace std;
using namespace cv;

//해당 이미지의 히스토그램을 창으로 출력하고 히스토그램 이미지를 반환한다.
Mat make_histogram_window(Mat& img, string s) {

	int histSize = 256;//히스토그램의 가로 사이즈
	
	double total;
	total = img.rows * img.cols;

	int hist_w = 512;//히스토그램의 가로 길이
	int hist_h = 500;//히스토그램의 높이
	int bin_w = cvRound((double)hist_w / histSize);//실수형 변수를 반올림한다.

	Mat histImage(hist_h, hist_w, CV_8U, Scalar(0, 0, 0));//히스토그램을 표현할 이미지

	for (int i = 0; i < histSize; i++) {//세로로 막대그래프를 그려나간다. (색깔 수를 100으로 나누어 표현)
		line(histImage, Point(bin_w * (i + 1), hist_h - img.at<ushort>(i)/100),
			Point(bin_w * (i + 1), hist_h), CV_RGB(255, 255, 255));
	}

	imshow(s, histImage);//완성된 히스토그램 이미지를 보인다.

	return histImage;//히스토그램의 이미지를 리턴한다.

}

int main() {

	Mat img1; //For 이미지1
	Mat img1_src = imread("mylove.jpg", CV_LOAD_IMAGE_COLOR);//이미지를 로드한다.
	Mat img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR); //이미지를 로드한다. For 이미지2

	if (img1_src.empty() || img2.empty()) {
		cout << "Failed to load image" << endl;//만약 둘 중 하나라도 로드되지 않을 경우 프로그램을 종료한다.
		exit(0);
	}
	//mylove.jpg의 사이즈가 너무 크므로 가로와 세로를 1/3씩 줄여서 재조정하여 img1에 넣는다.
	resize(img1_src, img1, Size(img1_src.cols / 3, img1_src.rows / 3), 0, 0, CV_INTER_NN);

	//그레이스케일 변환
	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);
	
	//히스토그램 생성(1 ~ 255의 배열<usigned short>)
	Mat img1_histo{ 1, 256, CV_16U };
	Mat img2_histo{ 1, 256, CV_16U };
	
	//히스토그램 모두 0으로 초기화
	for (int i = 0; i < 256; i++)
		img1_histo.at<ushort>(i) = 0;
	
	for (int i = 0; i < 256; i++)
		img2_histo.at<ushort>(i) = 0;

	auto img1_row = img1.rows;//이미지1의 세로 픽셀 수를 불러온다.
	auto img1_col = img1.cols;//이미지1의 가로 픽셀 수를 불러온다.

	//히스토그램에 영상의 값 대입
	for (int i = 0; i < img1_row; ++i) {
		for (int j = 0; j < img1_col; ++j) {
			auto black = static_cast<int>(img1.at<uchar>(i, j));//한 픽셀의 값 추출
			img1_histo.at<ushort>(black)++;//해당 픽셀의 인덱스의 값 증가
		}
	}

	auto img2_row = img2.rows;//이미지2의 세로 픽셀 수를 불러온다.
	auto img2_col = img2.cols;//이미지2의 가로 픽셀 수를 불러온다.

	for (int i = 0; i < img2_row; ++i) {
		for (int j = 0; j < img2_col; ++j) {
			auto black = static_cast<int>(img2.at<uchar>(i, j));//한 픽셀의 값 추출
			img2_histo.at<ushort>(black)++;//해당 픽셀의 인덱스의 값 증가
		}
	}
	int accum = 0;
	//이미지1의 히스토그램을 출력한다.
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
	cout << "총 픽셀 수: " << accum << endl;
	accum = 0;
	//이미지2의 히스토그램을 출력한다.
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
	cout << "총 픽셀 수:" <<  accum << endl;

	cout << endl << endl;

	//이미지1, 이미지2 창으로 출력
	imshow("mylove.jpg - original", img1);
	imshow("stuff_color.jpg - original", img2);

	//이미지1, 이미지2 히스토그램 창 생성
	Mat img1_og_hist_img = make_histogram_window(img1_histo, "mylove.jpg - Histogram original");
	Mat img2_og_hist_img = make_histogram_window(img2_histo, "stuff_color.jpg - Histogram original");

	Mat img1_hist_equalized; //Histogram Equalization 된 이미지1
	Mat img2_hist_equalized; //Histogram Equalization 된 이미지2

	//Histogram Equalization
	equalizeHist(img1, img1_hist_equalized);
	equalizeHist(img2, img2_hist_equalized);


	//히스토그램 초기화
	for (int i = 0; i < 256; i++)
		img1_histo.at<ushort>(i) = 0;

	for (int i = 0; i < 256; i++)
		img2_histo.at<ushort>(i) = 0;

	
	//히스토그램에 영상 대입
	for (int i = 0; i < img1_row; ++i) {
		for (int j = 0; j < img1_col; ++j) {
			auto black = static_cast<int>(img1_hist_equalized.at<uchar>(i, j));//한 픽셀의 값 추출
			img1_histo.at<ushort>(black)++;//해당 픽셀의 인덱스의 값 증가
		}
	}

	for (int i = 0; i < img2_row; ++i) {
		for (int j = 0; j < img2_col; ++j) {
			auto black = static_cast<int>(img2_hist_equalized.at<uchar>(i, j));//한 픽셀의 값 추출
			img2_histo.at<ushort>(black)++;//해당 픽셀의 인덱스의 값 증가
		}
	}
	accum = 0;
	//이미지1의 히스토그램을 출력한다.
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
	cout << "총 픽셀 수 : " << accum << endl;
	accum = 0;
	//이미지2의 히스토그램을 출력한다.
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

	cout << "총 픽셀 수 : " << accum << endl;

	cout << endl;
	
	//Equalized 된 이미지1, 이미지2 창으로 출력
	imshow("mylove.jpg - equalized",img1_hist_equalized);
	imshow("stuff_color.jpg - equalized", img2_hist_equalized);

	//이미지1, 이미지2 히스토그램 창으로 출력 및 생성
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


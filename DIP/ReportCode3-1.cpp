/*
*	ReportCode3-1.cpp
*	작성자	:	박도령
*	학번		:	2010112406
*	설명		:	LUT를 로드한 이미지의 밝기 및 명암을 조정한다.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int bright;//입력 밝기값(-255 ~ 255)
	int contrast;//입력 명암값(-100 ~ 100)

	cout << "이미지의 Bright 조정(입력 : -255 ~ 255)" << endl;
	cout << "입력 : ";

	cin >> bright;//밝기 값 입력

	cout << "이미지의 Contrast 조정(입력(%) : -100 ~ 100)" << endl;
	cout << "입력 : ";

	cin >> contrast;//명암 값 입력

	auto ratio = (float)(1 + (float)contrast / 100);//명암 비율 계산

	Mat image1 = imread("Lenna_gray.PNG");//그레이스케일 이미지를 로드한다.
	Mat result;//결과 이미지
	image1.copyTo(result);//결과 이미지에 로드된 이미지를 복사한다.(초기화용)

	Mat lkuptab{ 1, 256, CV_8U };//Lookup Table 생성

	for (int i = 0; i < 256; i++) {//Lookup Table Set
		if ((float)(i + bright) * ratio > 255) //Clamping(255보다 큰 경우)
			lkuptab.at<uchar>(i) = 255;
		else if ((float)(i + bright) * ratio < 0) //Clamping(0보다 작은 경우)
			lkuptab.at<uchar>(i) = 0;
		else //정상 경우 i + bright * ratio = 현재 이미지에 입력한 밝기와 명암 적용
			lkuptab.at<uchar>(i) = (float)(i + bright) * ratio;
	}

	if (!image1.data) {//만약 제대로 로드되지 않았으면
		cout << "Load Error" << endl;//에러 문구 출력
		exit(1);//프로그램 종료
	}
	
	LUT(image1, lkuptab ,result); //밝기 및 명암을 적용한 룩업테이블로 결과 이미지를 만든다.

	imshow("Adjusted Image", result);//윈도우를 이용하여 결과 이미지 출력

	waitKey(0);// Waiting keyboard input

	destroyAllWindows();//모든 윈도우 Destroy
}
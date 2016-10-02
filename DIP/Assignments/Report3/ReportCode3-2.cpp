/*
*	ReportCode3-2.cpp
*	작성자	:	박도령
*	학번		:	2010112406
*	설명		:	로드한 이미지의 명암을 조정한다.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int input; //입력할 명암값

	cout << "이미지의 명암 조정(입력(%) : -100 ~ 100)" << endl;
	cout << "입력 : ";

	cin >> input; //명암값을 입력한다.
	//입력한 명암값에 맞는 비율을 계산한다.
	auto ratio = (float)(1 + (float)input / 100);

	Mat image1 = imread("Lenna_gray.PNG");//그레이스케일 이미지를 Matrix 형태로 로드한다.
	Mat result;//결과 이미지 Matrix

	image1.copyTo(result);//로드된 이미지를 복사하여 결과 이미지에 넣는다.(초기화용)

	if (!image1.data) {//이미지를 로드하는 데에 실패하였을 경우
		cout << "Load Error" << endl;//에러 문구 출력
		exit(1);//프로그램 종료
	}

	int row = image1.rows; //이미지의 행
	int col = image1.cols; //이미지의 열 

	for (int j = 0; j < row; j++) { //이미지의 행만큼 반복문 실행
		for (int i = 0; i < col; i++) { //이미지의 열만큼 반복문 실행(순서 : 가로 아래)

			auto chBlue = (float)(image1.at<cv::Vec3b>(j, i)[0]); //이미지의 Blue Channel 추출
			auto chGreen = (float)(image1.at<cv::Vec3b>(j, i)[1]); //이미지의 Green Channel 추출
			auto chRed = (float)(image1.at<cv::Vec3b>(j, i)[2]); //이미지의 Red Channel 추출

			auto ResultBlue = chBlue * ratio; //Blue Channel 값에 입력한 비율만큼 곱한다.
			auto ResultGreen = chGreen * ratio; //Green Channel 값에 입력한 비율만큼 곱한다.
			auto ResultRed = chRed * ratio; //Red Channel 값에 입력한 비율만큼 곱한다.

			//Clamping (모두 0으로)
			if (ResultBlue < 0.0)
				ResultBlue = 0.0f;
			if (ResultGreen < 0.0)
				ResultGreen = 0.0f;
			if (ResultRed < 0.0)
				ResultRed = 0.0f;
			
			//Clamping (모두 255로)
			if (ResultBlue > 255.0)
				ResultBlue = 255.0f;
			if (ResultGreen > 255.0)
				ResultGreen = 255.0f;
			if (ResultRed > 255.0)
				ResultRed = 255.0f;

			result.at<cv::Vec3b>(j, i)[0] = ResultBlue;//계산한 Blue Channel값 결과 이미지에 대입
			result.at<cv::Vec3b>(j, i)[1] = ResultGreen;//계산한 Green Channel값 결과 이미지에 대입
			result.at<cv::Vec3b>(j, i)[2] = ResultRed;//계산한 Red Channel값 결과 이미지에 대입

		}
	}

	imshow("Image Adjusted", result); //결과 이미지를 윈도우에 생성하여 출력

	waitKey(0); // Waiting keyboard input

	destroyAllWindows(); //모든 윈도우 destroy
}
/*
*	ReportCode2-3.cpp
*	작성자	: 박도령
*	학번		: 2010112406
*	설명		: 콘솔창에 입력한 비율로 두 이미지를 섞는다.(Blending)
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int input;//입력할 비율(백분위)
	
	cout << "두 이미지를 Blending 할 비율을 입력하십시오.(입력(%) : 0 ~ 100)" << endl;
	cout << "입력 : ";
	
	cin >> input;//백분위 값 입력

	auto ratio = (float)input / 100;//비율로 변환
	
	Mat image1 = imread("image.jpg");//이미지1 Matrix 클래스 형태로 로드
	Mat image2 = imread("processed.jpg");//이미지2 Matrix 클래스 형태로 로드
	Mat imageBlended;//결과값

	image1.copyTo(imageBlended);//결과 이미지 이미지1 으로 초기화 
	
	if (!image1.data || !image2.data) {//두 이미지 중 하나 이상이 로드에 실패하였을 경우
		cout << "Load Error" << endl;//에러 문구 출력
		exit(1);//프로그램 종료
	}
		
	int row = image1.rows;//이미지1의 행의 수
	int col = image1.cols;//이미지1의 열의 수

	for (int j = 0; j < row; j++) {//이미지의 행만큼 반복문 실행
		for (int i = 0; i < col; i++) {//이미지의 열만큼 반복문 실행(출력 순서 : 가로 아래)
			
			auto chBlue1 = (float)(image1.at<Vec3b>(j, i)[0]);//Image1의 Blue Channel값
			auto chGreen1 = (float)(image1.at<Vec3b>(j, i)[1]);//Image1의 Green Channel값
			auto chRed1 = (float)(image1.at<Vec3b>(j, i)[2]);//Image1의 Red Channel값

			auto chBlue2 = (float)(image2.at<Vec3b>(j, i)[0]);//Image2의 Blue Channel값
			auto chGreen2 = (float)(image2.at<Vec3b>(j, i)[1]);//Image2의 Green Channel값
			auto chRed2 = (float)(image2.at<Vec3b>(j, i)[2]);//Image2의 Red Channel값

			auto BlendedBlue = (chBlue1 * ratio + chBlue2 * (1 - ratio));//image1과 image2의 Blue Channel에서 입력한 비율만큼 Blending
			auto BlendedGreen = (chGreen1 * ratio + chGreen2 * (1 - ratio));//image1과 image2의 Green Channel에서 입력한 비율만큼 Blending
			auto BlendedRed = (chRed1 * ratio + chRed2 * (1 - ratio));//image1과 image2의 Red Channel에서 입력한 비율만큼 Blending

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

			imageBlended.at<Vec3b>(j, i)[0] = BlendedBlue;//섞은 Blue Channel 이미지 대입
			imageBlended.at<Vec3b>(j, i)[1] = BlendedGreen;//섞은 Green Channel 이미지 대입
			imageBlended.at<Vec3b>(j, i)[2] = BlendedRed;//섞은 Red Channel 이미지 대입

		}
	}
	
	imshow("Image Blended", imageBlended);//결과 이미지 출력
	
	waitKey(0);// Waiting keyboard input

	destroyAllWindows();//모든 윈도우 Destroy
}
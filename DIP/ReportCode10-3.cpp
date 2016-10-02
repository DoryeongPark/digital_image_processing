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
	
	//이미지들을 그레이스케일 이미지로 가져온다.
	img1 = imread("tophat.PNG", CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty() || img2.empty())
		cout << "Images couldn't be loaded" << endl;

	//stuff_color.jpg는 사이즈가 너무 크므로 4배 줄인다.
	resize(img2, img2, Size{ img2.cols / 2,img2.rows / 2 }, 0, 0, CV_INTER_NN);
	
	//정사각형 모양의 151 * 151 크기 structuring element 생성
	auto se = getStructuringElement(MORPH_RECT, Size{ 151, 151 }, Point{ 75, 75 });

	//생성한 structuring element로 Image1, Image2에 Tophat(원본 이미지 - Opening(원본 이미지)효과 적용
	morphologyEx(img1, img1, CV_MOP_TOPHAT, se);
	morphologyEx(img2, img2, CV_MOP_TOPHAT, se);

	//Otsu threshold 적용
	threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
	threshold(img2, img2, 0, 255, CV_THRESH_OTSU);

	//Line의 Rho거리와 델타 각도를 저장할 벡터 생성
	vector<Vec2f> lines1, lines2;

	//Hough Transform을 사용하여 검출된 Line의 정보 저장(투표 최대 개수 : 180)
	HoughLines(img1, lines1, 1, PI / 180, 180);
	HoughLines(img2, lines2, 1, PI / 180, 180);

	//Image 1에서 검출된 Line들을 그린다.
	for (auto it = lines1.begin(); it != lines1.end(); ++it) {

		auto rho = (*it)[0]; //rho 거리
		auto theta = (*it)[1]; //델타 각도

		if (theta < PI / 4. || theta > 3.*PI / 4.) {// 수직 행
			Point pt1(rho / cos(theta), 0);//Line의 끝 점1
			Point pt2((rho - img1.rows * sin(theta)) / cos(theta), img1.rows);//Line의 끝 점2

			line(img1, pt1, pt2, Scalar(255), 1);//두 점을 이은 선분을 하얀색으로 그린다.
		}
		else {
			Point pt1(0, rho / sin(theta));//Line의 끝 점1
			Point pt2(img1.cols, (rho - img1.cols * cos(theta)) / sin(theta));//Line의 끝 점2

			line(img1, pt1, pt2, Scalar(255), 1);//두 점을 이은 선분을 하얀색으로 그랜다.
		}

	}

	//Image 2에서 검출된 Line들을 그린다.
	for (auto it = lines2.begin(); it != lines2.end(); ++it) {

		auto rho = (*it)[0]; //rho 거리
		auto theta = (*it)[1]; //델타 각도

		if (theta < PI / 4. || theta > 3.*PI / 4.) {// 수직 행
			Point pt1(rho / cos(theta), 0);//Line의 끝 점1
			Point pt2((rho - img2.rows * sin(theta)) / cos(theta), img2.rows);//Line의 끝 점2

			line(img2, pt1, pt2, Scalar(255), 1);//두 점을 이은 선분을 하얀색으로 그린다.
		}
		else {// 수평 행
			Point pt1(0, rho / sin(theta));//Line의 끝 점1
			Point pt2(img2.cols, (rho - img2.cols * cos(theta)) / sin(theta));//Line의 끝 점2

			line(img2, pt1, pt2, Scalar(255), 1);//두 점을 이은 선분을 하얀색으로 그린다.
		}

	}

	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-3(1).jpg", img1);
	imwrite("ReportCode10-3(2).jpg", img2);

	destroyAllWindows();
}
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace std;
using namespace cv;

int main() {

	//비디오 파일 경로
	string path = "video data for design hw.avi";

	//USB 카메라 객체를 생성한다.
	VideoCapture camera{ path };

	Mat original_img; //읽어들인 원 영상
	Mat pattern_img = imread("blue_ball.jpg", CV_LOAD_IMAGE_COLOR); //패턴 이미지를 로드한다
	Mat corr_img; //패턴 이미지와의 상관 계수가 반영된 이미지(일치하는 곳이 가장 밝음)

	Point min_point;//상관 계수가 가장 낮은 지점
	Point match_point;//상관 계수가 가장 높은 지점 

	double min, max; //최대값과 최소값을 찾기 위한 변수

	if (pattern_img.empty()) {
		cout << "Image couldn't be loaded" << endl;
		exit(0);
	}

	//마우스 이미지를 1/9 크기로 줄인다
	resize(pattern_img, pattern_img, Size(pattern_img.cols / 3,
		pattern_img.rows / 3), 0, 0, CV_INTER_NN);

	//패턴 이미지를 창으로 띄운다
	imshow("Pattern Image", pattern_img);

	if (!camera.isOpened()) {
		cout << "Camera couldn't be loaded." << endl;
		exit(0);
	}

	//카메라 영상을 연속적으로 읽어들인다.
	while (camera.isOpened()) {

		//카메라로 읽어 들인 영상을 original_img에 저장한다
		camera.read(original_img);

		//패턴 이미지와 원 영상의 상관계수 영상을 구한다
		matchTemplate(original_img, pattern_img, corr_img,
			CV_TM_CCOEFF_NORMED);

		//상관계수가 최대인 지점을 찾는다 : match_point
		minMaxLoc(corr_img, &min, &max, &min_point, &match_point);

		//match_point는 매칭하는 영상의 왼쪽 위 지점이다
		//패턴 영상의 왼쪽 지점부터 영상 크기만큼 그린다.
		rectangle(original_img, Rect{ match_point.x, match_point.y,
			pattern_img.cols, pattern_img.rows },
			Scalar{ 0, 0, 255 }, 3);

		//최대 지점을 초록색으로 표현한다
		circle(original_img, match_point,
			3, Scalar(0, 255, 0), CV_FILLED);

		//영상을 출력한다.
		imshow("Original Image", original_img);
		imshow("Corr Image", corr_img);

		waitKey(1);
	}

}
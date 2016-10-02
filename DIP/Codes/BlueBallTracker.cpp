//======================================================================
//	
//	BlueBallTracker.cpp
//
//	작성자	:	박도령/2010112406	 &	김민정/2014
//	설명		:	카메라 영상 속의 파란색 공을 인식할 때마다 인식한 원을 추적한다.
//
//----------------------------------------------------------------------
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

	//============================ Variables ============================
	//
	//	camera			:	연결할 카메라 장치 객체
	//	original_image	:	카메라에서 읽어 들인 원 영상
	//	hsv_img			:	RGB 채널인 원 영상을 HSV영상으로 변환한 결과
	//	binary_image	:	HSV영상의 파란색 부분만 걸러서 이진영상으로 표현
	//	circles			:	Hough 원 검출 메소드로 검출한 원들의 정보
	//
	//-------------------------------------------------------------------

	VideoCapture camera{ 0 };
	Mat original_image;		
	Mat hsv_img;
	Mat binary_image;

	auto erode_se = getStructuringElement(MORPH_RECT, Size(17, 17));
	auto dilate_se = getStructuringElement(MORPH_ELLIPSE, Size(19, 19));

	vector<Vec3f> circles;

	if (!camera.isOpened()) {				
		cout << "Camera couldn't open" << endl;
		exit(0);														
	}

	while (camera.isOpened()) {		
		//원 영상을 카메라에서부터 불러온다.
		auto is_loaded = camera.read(original_image);		

		if (!is_loaded || original_image.empty()) {		
			cout << "Camera finished" << endl;	
			destroyAllWindows();
			exit(0);
		}
		
		//RGB형식인 원 영상을 HSV영상으로 변환 original_image -> hsv_img
		cvtColor(original_image, hsv_img, CV_BGR2HSV);
		
		//HSV영상에서 파란색 부분을 검출하여 해당 부분이 255인 이진 영상으로 변환 
		//경우에 따라 범위 수정 가능
		inRange(hsv_img, Scalar(105, 110, 75), Scalar(133, 245, 255),
														binary_image);

		//3*3 사이즈로 가우시안 블러 처리
		GaussianBlur(binary_image, binary_image, Size(3, 3), 0);

		//검출할 원의 최소 크기의 Noise들을 제거하기 위해서 15*15 사이즈의 
		//정사각형 SE를 사용하여 Erosion 연산을 적용한다.
		morphologyEx(binary_image, binary_image, CV_MOP_ERODE, erode_se);
		
		//부피가 줄어든 원의 크기를 원 모양으로 늘리기 위해서 21*21 사이즈의 
		//마름모꼴 SE를 사용하여 Dilation 연산을 적용한다.
		morphologyEx(binary_image, binary_image, CV_MOP_DILATE, dilate_se);
		

		//========================== Hough 원 검출 ==========================
		//
		//	binary_image		:	원을 검출할 이진 영상
		//	circles				:	검출할 원을 저장할 벡터
		//	CV_HOUGH_GRADIENT	:	Hough 원 검출 방법 사용(Two-Pass Algorithm)
		//	검출 원 최소 반지름	:	15
		//	검출 원 최대 반지름	:	120
		//	검출 포인트			:	40
		//
		//------------------------------------------------------------------

		HoughCircles(binary_image, circles,	CV_HOUGH_GRADIENT, 2,									
			 50, 100, 40, 21, 120);					
			
		//========================== 검출된 원의 명세 ==========================
		//
		//	circles[i][0]	:	검출된 원의 중심의 x 좌표
		//	circles[i][1]	:	검출된 원의 중심의 y 좌표
		//	circles[i][2]	:	검출된 원의 반지름
		//
		//--------------------------------------------------------------------
		//
		//	검출된 원을 바탕으로 Tracing Rectangle Image를 원 영상에 그린다.
		//

		for (int i = 0; i < circles.size(); i++) {	

			auto circle_x = (int)circles[i][0];
			auto circle_y = (int)circles[i][1];

			auto center_point = Point{ circle_x, circle_y };
			auto radius = (int)circles[i][2];

			//검출된 원보다 큰 범위에 Tracing Rectangle을 그리기 위한 길이 
			auto expanded_radius = (int)circles[i][2] + 10; 

			//길이를 사용하여 Rectangle을 그리기 시작할 좌표를 구한다.
			auto rect_x = circle_x - expanded_radius;
			auto rect_y = circle_y - expanded_radius;
															
			//빨간색의 4의 굵기를 가진 Rectangle을 검출된 원 주위에 그린다.
			rectangle(original_image, Rect{rect_x, rect_y, 
				expanded_radius * 2 , expanded_radius * 2}, 
				Scalar{ 0, 0, 255 }, 4);

			//검출된 원의 중심을 가로지르는 x축과 평행한 검은 선을 그린다.
			line(original_image, 
				Point{ circle_x - expanded_radius - 10, circle_y },
				Point{ circle_x + expanded_radius + 10, circle_y },
				Scalar{ 0, 0, 0 }, 2);
		
			//검출된 원의 중심을 가로지르는 y축과 평행한 검은 선을 그린다. 
			line(original_image, 
				Point{ circle_x, circle_y - expanded_radius - 10 },
				Point{ circle_x, circle_y + expanded_radius + 10 },
				Scalar{ 0, 0, 0 }, 2);
			
			//원의 중앙을 나타낼 초록색 작은 원을 그린다.
			circle(original_image, center_point,
				3, Scalar(0, 255, 0), CV_FILLED);
			/* //원의 중심 및 반지름 출력용 코드 
			cout << "Circle's center point : [" << circle_x << ", " << circle_y
				<< "], radius = " << radius << "\n";*/
		}	
		
		//영상 출력
		namedWindow("Blue Ball Tracker", CV_WINDOW_AUTOSIZE);	
		namedWindow("Image Recognition", CV_WINDOW_AUTOSIZE);	
															
		imshow("Blue Ball Tracker", original_image);			
		imshow("Image Recognition", binary_image);
		
		waitKey(1);
	}	

}
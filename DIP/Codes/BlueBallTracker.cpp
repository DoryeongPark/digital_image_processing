//======================================================================
//	
//	BlueBallTracker.cpp
//
//	�ۼ���	:	�ڵ���/2010112406	 &	�����/2014
//	����		:	ī�޶� ���� ���� �Ķ��� ���� �ν��� ������ �ν��� ���� �����Ѵ�.
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
	
	//���� ���� ���
	string path = "video data for design hw.avi";

	//============================ Variables ============================
	//
	//	camera			:	������ ī�޶� ��ġ ��ü
	//	original_image	:	ī�޶󿡼� �о� ���� �� ����
	//	hsv_img			:	RGB ä���� �� ������ HSV�������� ��ȯ�� ���
	//	binary_image	:	HSV������ �Ķ��� �κи� �ɷ��� ������������ ǥ��
	//	circles			:	Hough �� ���� �޼ҵ�� ������ ������ ����
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
		//�� ������ ī�޶󿡼����� �ҷ��´�.
		auto is_loaded = camera.read(original_image);		

		if (!is_loaded || original_image.empty()) {		
			cout << "Camera finished" << endl;	
			destroyAllWindows();
			exit(0);
		}
		
		//RGB������ �� ������ HSV�������� ��ȯ original_image -> hsv_img
		cvtColor(original_image, hsv_img, CV_BGR2HSV);
		
		//HSV���󿡼� �Ķ��� �κ��� �����Ͽ� �ش� �κ��� 255�� ���� �������� ��ȯ 
		//��쿡 ���� ���� ���� ����
		inRange(hsv_img, Scalar(105, 110, 75), Scalar(133, 245, 255),
														binary_image);

		//3*3 ������� ����þ� �� ó��
		GaussianBlur(binary_image, binary_image, Size(3, 3), 0);

		//������ ���� �ּ� ũ���� Noise���� �����ϱ� ���ؼ� 15*15 �������� 
		//���簢�� SE�� ����Ͽ� Erosion ������ �����Ѵ�.
		morphologyEx(binary_image, binary_image, CV_MOP_ERODE, erode_se);
		
		//���ǰ� �پ�� ���� ũ�⸦ �� ������� �ø��� ���ؼ� 21*21 �������� 
		//������� SE�� ����Ͽ� Dilation ������ �����Ѵ�.
		morphologyEx(binary_image, binary_image, CV_MOP_DILATE, dilate_se);
		

		//========================== Hough �� ���� ==========================
		//
		//	binary_image		:	���� ������ ���� ����
		//	circles				:	������ ���� ������ ����
		//	CV_HOUGH_GRADIENT	:	Hough �� ���� ��� ���(Two-Pass Algorithm)
		//	���� �� �ּ� ������	:	15
		//	���� �� �ִ� ������	:	120
		//	���� ����Ʈ			:	40
		//
		//------------------------------------------------------------------

		HoughCircles(binary_image, circles,	CV_HOUGH_GRADIENT, 2,									
			 50, 100, 40, 21, 120);					
			
		//========================== ����� ���� �� ==========================
		//
		//	circles[i][0]	:	����� ���� �߽��� x ��ǥ
		//	circles[i][1]	:	����� ���� �߽��� y ��ǥ
		//	circles[i][2]	:	����� ���� ������
		//
		//--------------------------------------------------------------------
		//
		//	����� ���� �������� Tracing Rectangle Image�� �� ���� �׸���.
		//

		for (int i = 0; i < circles.size(); i++) {	

			auto circle_x = (int)circles[i][0];
			auto circle_y = (int)circles[i][1];

			auto center_point = Point{ circle_x, circle_y };
			auto radius = (int)circles[i][2];

			//����� ������ ū ������ Tracing Rectangle�� �׸��� ���� ���� 
			auto expanded_radius = (int)circles[i][2] + 10; 

			//���̸� ����Ͽ� Rectangle�� �׸��� ������ ��ǥ�� ���Ѵ�.
			auto rect_x = circle_x - expanded_radius;
			auto rect_y = circle_y - expanded_radius;
															
			//�������� 4�� ���⸦ ���� Rectangle�� ����� �� ������ �׸���.
			rectangle(original_image, Rect{rect_x, rect_y, 
				expanded_radius * 2 , expanded_radius * 2}, 
				Scalar{ 0, 0, 255 }, 4);

			//����� ���� �߽��� ���������� x��� ������ ���� ���� �׸���.
			line(original_image, 
				Point{ circle_x - expanded_radius - 10, circle_y },
				Point{ circle_x + expanded_radius + 10, circle_y },
				Scalar{ 0, 0, 0 }, 2);
		
			//����� ���� �߽��� ���������� y��� ������ ���� ���� �׸���. 
			line(original_image, 
				Point{ circle_x, circle_y - expanded_radius - 10 },
				Point{ circle_x, circle_y + expanded_radius + 10 },
				Scalar{ 0, 0, 0 }, 2);
			
			//���� �߾��� ��Ÿ�� �ʷϻ� ���� ���� �׸���.
			circle(original_image, center_point,
				3, Scalar(0, 255, 0), CV_FILLED);
			/* //���� �߽� �� ������ ��¿� �ڵ� 
			cout << "Circle's center point : [" << circle_x << ", " << circle_y
				<< "], radius = " << radius << "\n";*/
		}	
		
		//���� ���
		namedWindow("Blue Ball Tracker", CV_WINDOW_AUTOSIZE);	
		namedWindow("Image Recognition", CV_WINDOW_AUTOSIZE);	
															
		imshow("Blue Ball Tracker", original_image);			
		imshow("Image Recognition", binary_image);
		
		waitKey(1);
	}	

}
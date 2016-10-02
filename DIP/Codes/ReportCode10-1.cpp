#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <string>	
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	Mat img1, img2;
	Mat labels1, labels2;
	
	//두 이미지를 그레이스케일 형식으로 로드 한다.
	img1 = imread("tophat.PNG", CV_LOAD_IMAGE_GRAYSCALE);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty() || img2.empty())
		cout << "Images couldn't be loaded" << endl;

	//stuff_color.jpg의 크기가 크므로 4배로 줄인다.
	resize(img2, img2, Size{ img2.cols / 2,img2.rows / 2 }, 0, 0, CV_INTER_NN);

	//크기가 151*151인 정사각형 structuring element 생성
	auto se = getStructuringElement(MORPH_RECT, Size{ 151, 151 }, Point{ 75, 75 });
	
	//생성한 structuring element를 사용하여 Tophat 효과 적용
	morphologyEx(img1, img1, CV_MOP_TOPHAT, se);
	morphologyEx(img2, img2, CV_MOP_TOPHAT, se);
	
	//Otsu threshold 효과 적용
	threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
	threshold(img2, img2, 0, 255, CV_THRESH_OTSU);

	//이미지마다 0부터 나누어진 총 이미지 개수만큼 Labeling을 적용하여 변수 labels에 저장한다.
	connectedComponents(img1, labels1, 8, CV_32S);
	connectedComponents(img2, labels2, 8, CV_32S);

	//라벨링 된 이미지에 색깔을 적용하기 위하여 RGB이미지로 바꾸었다. 
	cvtColor(img1, img1, CV_GRAY2BGR);
	cvtColor(img2, img2, CV_GRAY2RGB);

	//Image1의 "15번 이미지"를 빨간색으로 바꾼다.
	for (int i = 0; i < labels1.cols; ++i) {
		for (int j = 0; j < labels1.rows; ++j) {
			if (labels1.at<int>(i, j) == 15) {
				img1.at<Vec3b>(i, j)[0] = 0;
				img1.at<Vec3b>(i, j)[1] = 0;
				img1.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}

	//Image2의 "1번 이미지"를 초록색으로 바꾼다.
	for (int i = 0; i < labels2.rows; ++i) {
		for (int j = 0; j <	labels2.cols; ++j) {
			if (labels2.at<int>(i, j) == 1) {
				img2.at<Vec3b>(i, j)[0] = 0;
				img2.at<Vec3b>(i, j)[1] = 255;
				img2.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	
	imshow("Image 1", img1);
	imshow("Image 2", img2);
	
	waitKey(0);

	imwrite("ReportCode10-1(1).jpg", img1);
	imwrite("ReportCode10-1(2).jpg", img2);

	destroyAllWindows();

}
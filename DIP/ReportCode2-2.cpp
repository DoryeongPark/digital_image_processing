/*
*	ReportCode2-2.cpp
*	작성자 : 박도령
*	학번	  : 2010112406
*	설명	  : 두 이미지를 로드하여 뺄셈한 결과 이미지를 윈도우창 형태로 출력한다.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

int main(int argc, char* argv[])
{
	char* imgFileName1 = "image.jpg"; //이미지1의 파일 이름
	char *imgFileName2 = "processed.jpg"; //이미지2의 파일 이름
	char *winForBlended = "Blended Image";//이미지1에서 이미지2를 뺀 결과의 윈도우 이름

	IplImage *pImgIpl1 = nullptr; //이미지1
	IplImage *pImgIpl2 = nullptr; //이미지2
	IplImage *pImgBlended = nullptr; //결과 이미지

	pImgIpl1 = cvLoadImage(imgFileName1);//이미지1 로드
	pImgIpl2 = cvLoadImage(imgFileName2);//이미지2 로드

	if (!pImgIpl1 || !pImgIpl2)//만약 두 이미지중 하나라도 로드가 안되었을 경우
		printf("Can't load Image");//오류 출력
	else//성공적으로 로드 되었을 경우
	{
		//결과 이미지를 위한 윈도우 생성
		cvNamedWindow(winForBlended, CV_WINDOW_AUTOSIZE);
		//결과값에 대한 이미지를 똑같은 사이즈로 생성한다.
		pImgBlended = cvCreateImage(cvGetSize(pImgIpl2), 8, 3);
		//이미지1 - 이미지2 = 결과 이미지(이미지1과 이미지2를 뺄셈하여 뺼셈한 결과 pImgBlended에 저장)
		cvSub(pImgIpl1, pImgIpl2, pImgBlended, NULL);
		//생성했던 윈도우창을 이용하여 결과 이미지를 화면에 출력한다.
		cvShowImage(winForBlended, pImgBlended);

		// Waiting keyboard input
		cvWaitKey(0);

		// Release the image
		cvReleaseImage(&pImgIpl1);
		cvReleaseImage(&pImgIpl2);
		cvReleaseImage(&pImgBlended);

		// Destroy the window
		cvDestroyWindow(winForBlended);
	}
}
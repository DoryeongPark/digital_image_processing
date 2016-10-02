// Util.h: interface for the CUtil class.
//
//////////////////////////////////////////////////////////////////////


#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>


#if !defined(AFX_UTIL_H__3C7854E7_4EA2_4780_A97B_BBBEF2566DD6__INCLUDED_)
#define AFX_UTIL_H__3C7854E7_4EA2_4780_A97B_BBBEF2566DD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUtil
{
public:
	CUtil();
	virtual ~CUtil();

	// 영상 데이터 초기화
	IplImage *GS_createImage(CvSize size, int nChannels);
	IplImage *GS_createImage(CvSize size, int depth, int nChannels);

	// 지정한 크기 기준으로, 가로의 길이가 4의 배수에 맞게 축소/확대
	IplImage *GS_resizeForBitmap(int resize_height, int resize_width, IplImage *image);

	// IplImage를 HBITMAP으로 변환
	//HBITMAP GS_IplImage2Bitmap(IplImage *image);

	// IplImage를 OpenCV의 HighGUI와 연계하여 보기
	void GS_viewIplImage(char *title, IplImage *image);

	// 하나의 창으로 띄워준다. not using OpenCV
	void GS_showWindow(IplImage *image, char *caption_title);

	// 에러 메시지 ////////////////////////////////얘 구현이 안되있당!!!!!!!!!!!!!
	//void GS_errMsg(char *msg);

	// 클램핑
	int GS_clamping(int var);
	int GS_clamping(float var);
	int GS_clamping(double var);

	// 1차원 메모리 할당
	float *GS_floatAlloc1D(int length);

	// 할당한 1차원 메모리 해제
	void GS_free1D(float *data);

	// 2차원 메모리 할당
	//BYTE **GS_alloc2D(int height, int width, BYTE value = 0);
	float **GS_floatAlloc2D(int height, int width);
	double **GS_doubleAlloc2D(int height, int width);

	// 할당한 2차원 메모리 해제
	//void GS_free2D(BYTE **data, int length);
	void GS_free2D(float **data, int length);
	void GS_free2D(double **data, int length);

	// 2차원을 1차원으로 변환
	float *GS_toSingleArray(float **data, int height, int width);

	// Wirth sort : not recurisive sorting algorithm by N. Devilard from N. Wirth's book
	//BYTE GS_writh_sort(BYTE *data, int length);

	// Quick Select
	//BYTE GS_quick_select(BYTE *data, int length);

	// swap 
	//void GS_SWAP(BYTE *data, int x, int y);

	// quick sort
	//void GS_quick_sort(BYTE *data, int left, int right);

	// 해당 영상의 IplImage에서 명암도 공간을 2차원 배열로 가져온다.
	//void GS_getGrayFromIplImage(IplImage *src_image, BYTE ***gray, int *height, int *width);

	// BYTE, double 타입의 명암도 공간인 2차원 배열을 IplImage로 변환한다.
	//IplImage *GS_gray2IplImage(BYTE **gray, int height, int width);
	IplImage *GS_gray2IplImage(double **gray, int height, int width);

	// 해당 영상의 IplImage에서 RGB 컬러 공간을 2차원 배열로 가져온다.
	//void GS_getColorFromIplImage(IplImage *src_image, BYTE ***red, BYTE ***green, BYTE ***blue, int *height, int *width);

	// BYTE, double 타입의 컬러 공간인 2차원 배열을 IplImage로 변환한다.
	//IplImage *GS_color2IplImage(BYTE **image1, BYTE **image2, BYTE **image3, int height, int width);
	IplImage *GS_color2IplImage(double **image1, double **image2, double **image3, int height, int width);
};

#endif // !defined(AFX_UTIL_H__3C7854E7_4EA2_4780_A97B_BBBEF2566DD6__INCLUDED_)

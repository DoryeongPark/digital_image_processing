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

	// ���� ������ �ʱ�ȭ
	IplImage *GS_createImage(CvSize size, int nChannels);
	IplImage *GS_createImage(CvSize size, int depth, int nChannels);

	// ������ ũ�� ��������, ������ ���̰� 4�� ����� �°� ���/Ȯ��
	IplImage *GS_resizeForBitmap(int resize_height, int resize_width, IplImage *image);

	// IplImage�� HBITMAP���� ��ȯ
	//HBITMAP GS_IplImage2Bitmap(IplImage *image);

	// IplImage�� OpenCV�� HighGUI�� �����Ͽ� ����
	void GS_viewIplImage(char *title, IplImage *image);

	// �ϳ��� â���� ����ش�. not using OpenCV
	void GS_showWindow(IplImage *image, char *caption_title);

	// ���� �޽��� ////////////////////////////////�� ������ �ȵ��ִ�!!!!!!!!!!!!!
	//void GS_errMsg(char *msg);

	// Ŭ����
	int GS_clamping(int var);
	int GS_clamping(float var);
	int GS_clamping(double var);

	// 1���� �޸� �Ҵ�
	float *GS_floatAlloc1D(int length);

	// �Ҵ��� 1���� �޸� ����
	void GS_free1D(float *data);

	// 2���� �޸� �Ҵ�
	//BYTE **GS_alloc2D(int height, int width, BYTE value = 0);
	float **GS_floatAlloc2D(int height, int width);
	double **GS_doubleAlloc2D(int height, int width);

	// �Ҵ��� 2���� �޸� ����
	//void GS_free2D(BYTE **data, int length);
	void GS_free2D(float **data, int length);
	void GS_free2D(double **data, int length);

	// 2������ 1�������� ��ȯ
	float *GS_toSingleArray(float **data, int height, int width);

	// Wirth sort : not recurisive sorting algorithm by N. Devilard from N. Wirth's book
	//BYTE GS_writh_sort(BYTE *data, int length);

	// Quick Select
	//BYTE GS_quick_select(BYTE *data, int length);

	// swap 
	//void GS_SWAP(BYTE *data, int x, int y);

	// quick sort
	//void GS_quick_sort(BYTE *data, int left, int right);

	// �ش� ������ IplImage���� ��ϵ� ������ 2���� �迭�� �����´�.
	//void GS_getGrayFromIplImage(IplImage *src_image, BYTE ***gray, int *height, int *width);

	// BYTE, double Ÿ���� ��ϵ� ������ 2���� �迭�� IplImage�� ��ȯ�Ѵ�.
	//IplImage *GS_gray2IplImage(BYTE **gray, int height, int width);
	IplImage *GS_gray2IplImage(double **gray, int height, int width);

	// �ش� ������ IplImage���� RGB �÷� ������ 2���� �迭�� �����´�.
	//void GS_getColorFromIplImage(IplImage *src_image, BYTE ***red, BYTE ***green, BYTE ***blue, int *height, int *width);

	// BYTE, double Ÿ���� �÷� ������ 2���� �迭�� IplImage�� ��ȯ�Ѵ�.
	//IplImage *GS_color2IplImage(BYTE **image1, BYTE **image2, BYTE **image3, int height, int width);
	IplImage *GS_color2IplImage(double **image1, double **image2, double **image3, int height, int width);
};

#endif // !defined(AFX_UTIL_H__3C7854E7_4EA2_4780_A97B_BBBEF2566DD6__INCLUDED_)

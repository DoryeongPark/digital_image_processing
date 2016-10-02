/*
* Util.cpp : 유틸리티 관련 클래스
* Modified by Lee, Moon-Ho (conv2@korea.com)
* Last Modified : 2006/11/16
*/

#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUtil::CUtil()
{

}

CUtil::~CUtil()
{

}





// 2차원을 1차원으로 변환
float *CUtil::GS_toSingleArray(float **data, int height, int width)
{
	int cnt = -1;
	float *single_data = GS_floatAlloc1D(height*width);

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			single_data[++cnt] = data[i][j];
		}
	}

	return single_data;
}

// 2차원 메모리 할당
float **CUtil::GS_floatAlloc2D(int height, int width)
{
	float **data;

	data = (float **)calloc(height, sizeof(float *));
	for (int i = 0; i<height; i++)
	{
		data[i] = (float *)calloc(width, sizeof(float));
	}

	return data;
}

// 2차원 메모리 할당
double **CUtil::GS_doubleAlloc2D(int height, int width)
{
	double **data;

	data = (double **)calloc(height, sizeof(double *));
	for (int i = 0; i<height; i++)
	{
		data[i] = (double *)calloc(width, sizeof(double));
	}

	return data;
}

// 할당한 2차원 메모리 해제
void CUtil::GS_free2D(double **data, int length)
{
	for (int i = 0; i<length; i++)
		free(data[i]);

	free(data);
}

// 할당한 2차원 메모리 해제
void CUtil::GS_free2D(float **data, int length)
{
	for (int i = 0; i<length; i++)
		free(data[i]);

	free(data);
}

// 1차원 메모리 할당
float *CUtil::GS_floatAlloc1D(int length)
{
	return (float *)calloc(length, sizeof(float));
}

// 할당한 1차원 메모리 해제
void CUtil::GS_free1D(float *data)
{
	if (!data) free(data);
}


IplImage *CUtil::GS_createImage(CvSize size, int nChannels)
{
	IplImage *dst_image = cvCreateImage(size, IPL_DEPTH_8U, nChannels);
	cvSetZero(dst_image);

	return dst_image;
}

IplImage *CUtil::GS_createImage(CvSize size, int depth, int nChannels)
{
	IplImage *dst_image = cvCreateImage(size, depth, nChannels);
	cvSetZero(dst_image);

	return dst_image;
}


// 지정한 크기 기준으로, 가로의 길이가 4의 배수에 맞게 축소/확대
IplImage *CUtil::GS_resizeForBitmap(int resize_height, int resize_width, IplImage *image)
{
	// (주의!) BITMAP의 특성 : 가로의 길이가 4의 배수가 아니면 찌그러짐
	int height = image->height;
	int width = image->width - (image->width % 4);

	int sHeight;
	int sWidth;

	if (height > resize_height)
	{
		sHeight = resize_height;
		width = (int)(width*(resize_height / (double)height));
		sWidth = width - (width % 4);
	}
	else if (width > resize_width)
	{
		sHeight = height;
		sWidth = resize_width - (resize_width % 4);
	}
	else
	{
		sHeight = height;
		sWidth = width;
	}

	// sHeight x sWidth 크기의 영상 생성
	IplImage *resize_image = cvCreateImage(cvSize(sWidth, sHeight),
		IPL_DEPTH_8U,
		image->nChannels);

	// sHeight x sWidth 크기로 확대/축소 한다.
	cvResize(image, resize_image, CV_INTER_CUBIC);

	return resize_image;
}

void CUtil::GS_viewIplImage(char *title, IplImage *image)
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvShowImage(title, image);
	cvWaitKey(0);
	cvDestroyWindow(title);
}


int CUtil::GS_clamping(int var)
{
	int retVal = -1;

	// saturation 방법 적용
	if (var > 255) retVal = 255;
	else if (var < 0) retVal = 0;
	else retVal = var;

	return retVal;
}

int CUtil::GS_clamping(float var)
{
	return GS_clamping((int)var);
}

int CUtil::GS_clamping(double var)
{
	return GS_clamping((int)var);
}




// double 타입의 명암도 공간인 2차원 배열을 IplImage로 변환한다.
IplImage *CUtil::GS_gray2IplImage(double **gray, int height, int width)
{
	IplImage *dst_image = GS_createImage(cvSize(width, height), 1);

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			cvSetReal2D(dst_image, i, j, gray[i][j]);
		}
	}

	return dst_image;
}

// double 타입의 컬러 공간인 2차원 배열을 IplImage로 변환한다.
IplImage *CUtil::GS_color2IplImage(double **image1,
	double **image2,
	double **image3,
	int height,
	int width)
{
	IplImage *dst_image = GS_createImage(cvSize(width, height), 3);

	IplImage *ipl_image1 = GS_createImage(cvSize(width, height), 1);
	IplImage *ipl_image2 = GS_createImage(cvSize(width, height), 1);
	IplImage *ipl_image3 = GS_createImage(cvSize(width, height), 1);

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			cvSetReal2D(ipl_image1, i, j, image1[i][j]);
			cvSetReal2D(ipl_image2, i, j, image2[i][j]);
			cvSetReal2D(ipl_image3, i, j, image3[i][j]);
		}
	}

	// 조합
	cvCvtPlaneToPix(ipl_image1, ipl_image2, ipl_image3, NULL, dst_image);

	// 할당한 메모리 해제 
	cvReleaseImage(&ipl_image1);
	cvReleaseImage(&ipl_image2);
	cvReleaseImage(&ipl_image3);

	return dst_image;
}
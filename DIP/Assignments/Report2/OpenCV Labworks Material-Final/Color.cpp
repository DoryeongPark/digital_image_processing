// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "openmfc.h"
#include "Color.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColor::CColor()
{

}

CColor::~CColor()
{

}

// �÷� ������ ��ϵ� �������� ��ȯ  (conversion color space to gray scale image)
IplImage *CColor::GS_rgb2gray( IplImage *rgb_image )
{
	// STEP 1 : �޸� �Ҵ�  (memory allocation)
	IplImage *gray_image = cvCreateImage( cvGetSize(rgb_image), IPL_DEPTH_8U, 1 );

	// STEP 2 : cvCvtPixToPalne() �Լ� ȣ�� (call cvCvtPixToPlne() mothod)
	//          Gray level = 0.299*Red + 0.587*Green + 0.114*Blue
	cvCvtColor( rgb_image, gray_image, CV_BGR2GRAY );

	return gray_image;
}

// RGB �÷� ������ HSV �÷� �������� ��ȯ�Ѵ�. (convert RGB color space to HSV color space)
void CColor::GS_rgb2hsv( IplImage *red, IplImage *green, IplImage *blue,
						IplImage **hue, IplImage **saturation, IplImage **value )
{
	GS_cvtColor( red, green, blue, hue, saturation, value, CV_BGR2HSV );

	return;
}

// �־��� code�� ���� �÷� ���� ��ȯ�� ��, �� ä���� �и��Ͽ� ��ȯ�Ѵ�. 
//(split each channel and return after converting color space according to the given code) 
void CColor::GS_cvtColor( IplImage *src_ch1, IplImage *src_ch2, IplImage *src_ch3,
						  IplImage **dst_ch1, IplImage **dst_ch2, IplImage **dst_ch3, int code )
{
	// STEP 1 : �޸� �Ҵ� (memory allocation)
	IplImage *src_image = cvCreateImage( cvGetSize(src_ch1), IPL_DEPTH_8U, 3);
	IplImage *dst_image = cvCreateImage( cvGetSize(src_ch1), IPL_DEPTH_8U, 3 );

	*dst_ch1 = cvCreateImage( cvGetSize(src_ch1), IPL_DEPTH_8U, 1 );
	*dst_ch2 = cvCreateImage( cvGetSize(src_ch1), IPL_DEPTH_8U, 1 );
	*dst_ch3 = cvCreateImage( cvGetSize(src_ch1), IPL_DEPTH_8U, 1 );

	// STEP 2 : �� ä���� src_image�� �����Ѵ�.  (composite each channel to src_image)
	cvCvtPlaneToPix( src_ch1, src_ch2, src_ch3, NULL, src_image );

	// STEP 3 : �ش� �÷� ������ �־��� ���ڿ� �´� �÷� �������� ��ȯ. (convert color space to the new one which meets given argument)
	cvCvtColor( src_image, dst_image, code );

	// STEP 4 : ��ȯ�� �÷� ������ �и�(split the converted color space)
	cvCvtPixToPlane( dst_image, *dst_ch1, *dst_ch2, *dst_ch3, NULL );

	// STEP 5 : �޸� ���� (memory deallocation)
	cvReleaseImage( &src_image );
	cvReleaseImage( &dst_image );

	return;
}

// HSV �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to HSV color space)
void CColor::GS_hsv2rgb( IplImage *hue, IplImage *saturation, IplImage *value,
						IplImage **red, IplImage **green, IplImage **blue )
{
	GS_cvtColor( hue, saturation, value, red, green, blue, CV_HSV2BGR );

	return;
}

// RGB �÷� ������ YCbCr �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to YCbCr color space)
void CColor::GS_rgb2ycbcr( IplImage *red, IplImage *green, IplImage *blue,
						IplImage **Y, IplImage **Cb, IplImage **Cr )
{
	GS_cvtColor( red, green, blue, Y, Cr, Cb, CV_BGR2YCrCb );

	return;
}

// YCbCr �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to YCbCr color space)
void CColor::GS_ycbcr2rgb( IplImage *Y, IplImage *Cb, IplImage *Cr,
						IplImage **red, IplImage **green, IplImage **blue )
{
	GS_cvtColor( Y, Cr, Cb, red, green, blue, CV_YCrCb2BGR );

	return;
}

// RGB �÷� ���� �и�, ���࿡ ��ϵ� �����̸� �� ä�ο� �Ҵ��Ѵ�. (split RGB color space/ if the image is gray scale, assign it to each channel) 
void CColor::GS_splitRGB( IplImage *rgb_image, IplImage **red, IplImage **green, IplImage **blue )
{
	// STEP 1 : �޸� �Ҵ� (memory allocation)
	*red = cvCreateImage( cvGetSize(rgb_image), IPL_DEPTH_8U, 1 );
	*green = cvCreateImage( cvGetSize(rgb_image), IPL_DEPTH_8U, 1 );
	*blue = cvCreateImage( cvGetSize(rgb_image), IPL_DEPTH_8U, 1 );

	// STEP 2 : cvCvtPixToPlane() �Լ� ȣ�� (call cvCvtPixToPlne() mothod)
	// �÷� ���� (color image)
	if( rgb_image->nChannels == 3 )
	{
		// ���� : BGR ä�� ������ ������. (caution: B->G->R channel order)
		cvCvtPixToPlane( rgb_image, *blue, *green, *red, NULL );
	}
	// ��ϵ� ���� (gray scale image)
	else if( rgb_image->nChannels == 1 )
	{
		IplImage *tmp_image = cvCreateImage( cvGetSize(rgb_image), IPL_DEPTH_8U, 3 );
		cvCvtColor( rgb_image, tmp_image, CV_GRAY2BGR );
		cvCvtPixToPlane( tmp_image, *blue, *green, *red, NULL );
		cvReleaseImage( &tmp_image );
	}

	return;
}

// RGB ä���� �����Ͽ� RGB �÷� �������� ��ȯ�Ѵ�. (composite RGB channels and convert RGB color space)
IplImage *CColor::GS_compositeRGB( IplImage *red, IplImage *green, IplImage *blue )
{
	IplImage *rgb_image = cvCreateImage( cvGetSize( red ), IPL_DEPTH_8U, 3 );
	cvCvtPlaneToPix( blue, green, red, NULL, rgb_image );

	return rgb_image;
}

// RGB �÷� ���� �и� - 5�� ���� (Split RGB color space)
void CColor::SplitRGB( IplImage *color_image, IplImage **red, IplImage **green, IplImage **blue )
{
	// STEP 1 : �޸� �Ҵ� (memory allocation)
	*red = cvCreateImage( cvGetSize(color_image), IPL_DEPTH_8U, 1 );
	*green = cvCreateImage( cvGetSize(color_image), IPL_DEPTH_8U, 1 );
	*blue = cvCreateImage( cvGetSize(color_image), IPL_DEPTH_8U, 1 );

	// STEP 2 : cvCvtPixToPlne() �Լ� ȣ�� (call cvCvtPixToPlne() mothod)
	cvCvtPixToPlane( color_image, *red, *green, *blue, NULL );

	return;
}


// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_)
#define AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColor  
{
public:
	CColor();
	virtual ~CColor();

	// RGB 컬러 공간 분리, 만약에 명암도 영상이면 각 채널에 할당함.
	void GS_splitRGB( IplImage *rgb_image, IplImage **red, IplImage **green, IplImage **blue );

	// RGB 채널을 조합하여 하나의 RGB 컬러 공간으로 반환한다.
	IplImage *GS_compositeRGB( IplImage *red, IplImage *green, IplImage *blue );

	// RGB 컬러 공간을 HSV 컬러 공간으로 변환한다.
	void GS_rgb2hsv( IplImage *red, IplImage *green, IplImage *blue,
		 			 IplImage **hue, IplImage **saturation, IplImage **value );

	// HSV 컬러 공간을 RGB 컬러 공간으로 변환한다.
	void GS_hsv2rgb( IplImage *hue, IplImage *saturation, IplImage *value,
					 IplImage **red, IplImage **green, IplImage **blue );

	// RGB 컬러 공간을 YCbCr 컬러 공간으로 변환한다.
	void GS_rgb2ycbcr( IplImage *red, IplImage *green, IplImage *blue,
						IplImage **Y, IplImage **Cb, IplImage **Cr );

	// YCbCr 컬러 공간을 RGB 컬러 공간으로 변환한다.
	void GS_ycbcr2rgb( IplImage *Y, IplImage *Cb, IplImage *Cr,
						IplImage **red, IplImage **green, IplImage **blue );

	// 주어진 code에 따라 컬러 공간 변환한 후, 각 채널을 분리하여 반환한다.
	void GS_cvtColor( IplImage *src_ch1, IplImage *src_ch2, IplImage *src_ch3,
					  IplImage **dst_ch1, IplImage **dst_ch2, IplImage **dst_ch3, int code );
	
	// 컬러 공간을 명암도 영상으로 변환 
	IplImage *GS_rgb2gray( IplImage *rgb_image );

	// 5장 예제 
	void SplitRGB(IplImage *color_image, /* RGB 컬러 영상 */
				  IplImage **red,		 /* 빨강 채널 */
				  IplImage **green,		 /* 녹색 채널 */
				  IplImage **blue);		 /* 파랑 채널 */
};

#endif // !defined(AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_)

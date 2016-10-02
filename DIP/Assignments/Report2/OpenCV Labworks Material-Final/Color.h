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

	// RGB �÷� ���� �и�, ���࿡ ��ϵ� �����̸� �� ä�ο� �Ҵ���.
	void GS_splitRGB( IplImage *rgb_image, IplImage **red, IplImage **green, IplImage **blue );

	// RGB ä���� �����Ͽ� �ϳ��� RGB �÷� �������� ��ȯ�Ѵ�.
	IplImage *GS_compositeRGB( IplImage *red, IplImage *green, IplImage *blue );

	// RGB �÷� ������ HSV �÷� �������� ��ȯ�Ѵ�.
	void GS_rgb2hsv( IplImage *red, IplImage *green, IplImage *blue,
		 			 IplImage **hue, IplImage **saturation, IplImage **value );

	// HSV �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.
	void GS_hsv2rgb( IplImage *hue, IplImage *saturation, IplImage *value,
					 IplImage **red, IplImage **green, IplImage **blue );

	// RGB �÷� ������ YCbCr �÷� �������� ��ȯ�Ѵ�.
	void GS_rgb2ycbcr( IplImage *red, IplImage *green, IplImage *blue,
						IplImage **Y, IplImage **Cb, IplImage **Cr );

	// YCbCr �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.
	void GS_ycbcr2rgb( IplImage *Y, IplImage *Cb, IplImage *Cr,
						IplImage **red, IplImage **green, IplImage **blue );

	// �־��� code�� ���� �÷� ���� ��ȯ�� ��, �� ä���� �и��Ͽ� ��ȯ�Ѵ�.
	void GS_cvtColor( IplImage *src_ch1, IplImage *src_ch2, IplImage *src_ch3,
					  IplImage **dst_ch1, IplImage **dst_ch2, IplImage **dst_ch3, int code );
	
	// �÷� ������ ��ϵ� �������� ��ȯ 
	IplImage *GS_rgb2gray( IplImage *rgb_image );

	// 5�� ���� 
	void SplitRGB(IplImage *color_image, /* RGB �÷� ���� */
				  IplImage **red,		 /* ���� ä�� */
				  IplImage **green,		 /* ��� ä�� */
				  IplImage **blue);		 /* �Ķ� ä�� */
};

#endif // !defined(AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_)

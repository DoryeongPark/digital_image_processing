// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////


#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>


#if !defined(AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_)
#define AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilter
{
public:
	CFilter();
	virtual ~CFilter();

	// ����þ� ������ ���͸�
	IplImage *GS_gaussian_smoothing_filtering(IplImage *src_image, double sigma);

	// �ִ�/�ּҰ� ���͸�
	IplImage *GS_maxmin_filtering(IplImage *src_image, int method, int mask_height = 3, int mask_width = 3);

	// ��-trimmed ��հ� ���͸�
	IplImage *GS_alpha_trim_mean_filtering(IplImage *src_image, double alpha, int mask_height = 3, int mask_width = 3);

	// ��հ� ���͸� 
	IplImage *GS_mean_filtering(IplImage *src_image, int mask_height = 3, int mask_width = 3);

	// �̵�� ���͸�
	IplImage *GS_median_filtering(IplImage *src_image, int mask_height = 3, int mask_width = 3, int type = 0);

	// ������ ����
	IplImage *GS_soften_filter(IplImage *src_image, int method);

	// ���ڽ� ��Ŀ��/������ ����
	IplImage *GS_enhance_filter(IplImage *src_image, int method);

	// �� ����Ʈ/���� ���� 
	IplImage *GS_blur_lb_filter(IplImage *src_image, int method);

	// ������
	IplImage *GS_sharpening(IplImage *src_image, int method);

	// ������ ���� ��� ���͸�
	IplImage *GS_high_boost(IplImage *src_image, float alpha);
	IplImage *GS_high_boost(IplImage *src_image, float alpha, float bias);

	// �������
	IplImage *GS_unsharpening(IplImage *src_image, int method);
	IplImage *GS_unsharpening(IplImage *src_image, int method, float bias);

	// ����
	IplImage *GS_blurring(IplImage *src_image, int option, int method);

	// RGB �÷� ���������� �簢 ȿ��
	IplImage *GS_rgb_embossing(IplImage *src_image, int method);

	// HSV �÷� ���������� �簢 ȿ��
	IplImage *GS_hsv_embossing(IplImage *src_image, int method);

	// ��ϵ� ���󿡼��� �簢 ȿ��
	IplImage *GS_embossing(IplImage *src_image, int method);

	// ��ϵ� ���󿡼��� �簢 ȿ�� - ��ģ ȿ��
	IplImage *GS_wild_embossing(IplImage *src_image, int method);

	// ȸ�� ���� 
	IplImage *GS_conv2(IplImage *src_image, CvMat *mat_kernel);

	// 1���� Ŀ���� ���� ���͸�.
	IplImage *ApplyFilter2D(IplImage *src_image, /* �� ���� */
		float *kernel,		  /* 1���� Ŀ�� */
		int kHeight,		  /* Ŀ���� �ʺ� */
		int kWidth);		  /* Ŀ���� �ʺ� */

	// 2���� Ŀ���� ���� ���͸�.
	IplImage *ApplyFilter2D(IplImage *src_image, /* �� ���� */
		float **kernel,	  /* 2���� Ŀ�� */
		int kHeight,		  /* Ŀ���� �ʺ� */
		int kWidth);		  /* Ŀ���� �ʺ� */

	// CvMat�� kernel�� ���� ���͸�.
	IplImage *ApplyFilter2D(IplImage *src_image, /* �� ���� */
		CvMat kernel);		  /* Ŀ�� */

private:

	// ���� - cvFilter2D() �Լ� ���
	IplImage *GS_blurring_cvFilter2D(IplImage *src_image, int method);

	// ���� - cvSmooth() �Լ� ���
	IplImage *GS_blurring_cvSmooth(IplImage *src_image, int option, int method);
};

#endif // !defined(AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_)

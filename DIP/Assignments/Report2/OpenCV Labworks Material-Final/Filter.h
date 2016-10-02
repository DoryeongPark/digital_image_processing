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

	// 가우시안 스무딩 필터링
	IplImage *GS_gaussian_smoothing_filtering(IplImage *src_image, double sigma);

	// 최대/최소값 필터링
	IplImage *GS_maxmin_filtering(IplImage *src_image, int method, int mask_height = 3, int mask_width = 3);

	// α-trimmed 평균값 필터링
	IplImage *GS_alpha_trim_mean_filtering(IplImage *src_image, double alpha, int mask_height = 3, int mask_width = 3);

	// 평균값 필터링 
	IplImage *GS_mean_filtering(IplImage *src_image, int mask_height = 3, int mask_width = 3);

	// 미디언 필터링
	IplImage *GS_median_filtering(IplImage *src_image, int mask_height = 3, int mask_width = 3, int type = 0);

	// 소프턴 필터
	IplImage *GS_soften_filter(IplImage *src_image, int method);

	// 인핸스 포커스/디테일 필터
	IplImage *GS_enhance_filter(IplImage *src_image, int method);

	// 블러 라이트/블랜딩 필터 
	IplImage *GS_blur_lb_filter(IplImage *src_image, int method);

	// 샤프닝
	IplImage *GS_sharpening(IplImage *src_image, int method);

	// 고주파 지원 통과 필터링
	IplImage *GS_high_boost(IplImage *src_image, float alpha);
	IplImage *GS_high_boost(IplImage *src_image, float alpha, float bias);

	// 언샤프닝
	IplImage *GS_unsharpening(IplImage *src_image, int method);
	IplImage *GS_unsharpening(IplImage *src_image, int method, float bias);

	// 블러링
	IplImage *GS_blurring(IplImage *src_image, int option, int method);

	// RGB 컬러 공간에서의 양각 효과
	IplImage *GS_rgb_embossing(IplImage *src_image, int method);

	// HSV 컬러 공간에서의 양각 효과
	IplImage *GS_hsv_embossing(IplImage *src_image, int method);

	// 명암도 영상에서의 양각 효과
	IplImage *GS_embossing(IplImage *src_image, int method);

	// 명암도 영상에서의 양각 효과 - 거친 효과
	IplImage *GS_wild_embossing(IplImage *src_image, int method);

	// 회선 수행 
	IplImage *GS_conv2(IplImage *src_image, CvMat *mat_kernel);

	// 1차원 커널을 갖고 필터링.
	IplImage *ApplyFilter2D(IplImage *src_image, /* 원 영상 */
		float *kernel,		  /* 1차원 커널 */
		int kHeight,		  /* 커널의 너비 */
		int kWidth);		  /* 커널의 너비 */

	// 2차원 커널을 갖고 필터링.
	IplImage *ApplyFilter2D(IplImage *src_image, /* 원 영상 */
		float **kernel,	  /* 2차원 커널 */
		int kHeight,		  /* 커널의 너비 */
		int kWidth);		  /* 커널의 너비 */

	// CvMat인 kernel을 갖고 필터링.
	IplImage *ApplyFilter2D(IplImage *src_image, /* 원 영상 */
		CvMat kernel);		  /* 커널 */

private:

	// 블러링 - cvFilter2D() 함수 사용
	IplImage *GS_blurring_cvFilter2D(IplImage *src_image, int method);

	// 블러링 - cvSmooth() 함수 사용
	IplImage *GS_blurring_cvSmooth(IplImage *src_image, int option, int method);
};

#endif // !defined(AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_)

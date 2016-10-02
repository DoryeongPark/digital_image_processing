// Edge.h: interface for the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>

#if !defined(AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_)
#define AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEdge
{
public:
	CEdge();
	virtual ~CEdge();

	// 그래디언트 에지 추출
	IplImage *GS_gradient_edge(IplImage *src_image, int method = 2);

	// 영역 필터에 의한 에지 추출
	IplImage *GS_range_filter_edge(IplImage *src_image);

	// 소벨 에지 추출 : 마스크(3x3)
	IplImage *GS_sobel_edge(IplImage *src_image, int method = 2);

	// 소벨 에지 추출 : cvSobel() 함수 이용
	IplImage *GS_sobel_edge_cvSobel(IplImage *src_image, int method = 2);

	// 소벨 에지 추출 : 마스크(7x7)
	IplImage *GS_sobel_edge_7x7(IplImage *src_image, int method = 2);

	// 소벨 에지 추출 : 대각선
	IplImage *GS_sobel_diagonal_edge(IplImage *src_image, int method = 2);

	// 프리윗 에지 추출
	IplImage *GS_prewitt_edge(IplImage *src_image, int method = 2);

	// 프레이첸 에지 추출 
	IplImage *GS_freichen_edge(IplImage *src_image, int method = 2);

	// 서터캐스틱 에지 추출
	IplImage *GS_stochastic_edge(IplImage *src_image, int method = 2);

	// 로버츠 에지 추출
	IplImage *GS_roberts_edge(IplImage *src_image, int method = 2);

	// 라플라시안 에지 추출 : 마스크
	IplImage *GS_laplacian_edge(IplImage *src_image, int method = 1);

	// 라플라시안 에지 추출 : cvLaplace() 함수 이용
	IplImage *GS_laplacian_edge_cvLaplace(IplImage *src_image);

	// 라플라시안 샤프닝 
	IplImage *GS_laplacian_sharpening(IplImage *src_image, int method = 1);

	// LoG(Laplacian of Gaussian) 에지 추출 : 마스크
	IplImage *GS_LoG_edge(IplImage *src_image, double sigma, int method = 1);

	// LoG(Laplacian of Gaussian) 에지 추출 : 공식
	IplImage *GS_LoG2_edge(IplImage *src_image, double sigma);
	double **GS_get_LoG_mask(double sigma, int *mask_height, int *mask_width);
	double GS_get_LoG_formular(double x, double sigma);
	double norm(double x, double y);
	double GS_get_distance(double a, double b, double c, double d);
	double gauss(double x, double sigma);

	// 캐니 에지 추출 
	IplImage *GS_canny_edge(IplImage *src_image, double sigma, int threshold, int method);

	// 캐니 에지 추출 : cvCanny() 함수 이용 
	IplImage *GS_canny_edge_cvCanny(IplImage *src_image, double low_threshold = 0.0, double high_threshold = 255.0);

};
#endif // !defined(AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_)

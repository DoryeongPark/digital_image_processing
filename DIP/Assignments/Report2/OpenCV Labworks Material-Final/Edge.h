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

	// �׷����Ʈ ���� ����
	IplImage *GS_gradient_edge(IplImage *src_image, int method = 2);

	// ���� ���Ϳ� ���� ���� ����
	IplImage *GS_range_filter_edge(IplImage *src_image);

	// �Һ� ���� ���� : ����ũ(3x3)
	IplImage *GS_sobel_edge(IplImage *src_image, int method = 2);

	// �Һ� ���� ���� : cvSobel() �Լ� �̿�
	IplImage *GS_sobel_edge_cvSobel(IplImage *src_image, int method = 2);

	// �Һ� ���� ���� : ����ũ(7x7)
	IplImage *GS_sobel_edge_7x7(IplImage *src_image, int method = 2);

	// �Һ� ���� ���� : �밢��
	IplImage *GS_sobel_diagonal_edge(IplImage *src_image, int method = 2);

	// ������ ���� ����
	IplImage *GS_prewitt_edge(IplImage *src_image, int method = 2);

	// ������þ ���� ���� 
	IplImage *GS_freichen_edge(IplImage *src_image, int method = 2);

	// ����ĳ��ƽ ���� ����
	IplImage *GS_stochastic_edge(IplImage *src_image, int method = 2);

	// �ι��� ���� ����
	IplImage *GS_roberts_edge(IplImage *src_image, int method = 2);

	// ���ö�þ� ���� ���� : ����ũ
	IplImage *GS_laplacian_edge(IplImage *src_image, int method = 1);

	// ���ö�þ� ���� ���� : cvLaplace() �Լ� �̿�
	IplImage *GS_laplacian_edge_cvLaplace(IplImage *src_image);

	// ���ö�þ� ������ 
	IplImage *GS_laplacian_sharpening(IplImage *src_image, int method = 1);

	// LoG(Laplacian of Gaussian) ���� ���� : ����ũ
	IplImage *GS_LoG_edge(IplImage *src_image, double sigma, int method = 1);

	// LoG(Laplacian of Gaussian) ���� ���� : ����
	IplImage *GS_LoG2_edge(IplImage *src_image, double sigma);
	double **GS_get_LoG_mask(double sigma, int *mask_height, int *mask_width);
	double GS_get_LoG_formular(double x, double sigma);
	double norm(double x, double y);
	double GS_get_distance(double a, double b, double c, double d);
	double gauss(double x, double sigma);

	// ĳ�� ���� ���� 
	IplImage *GS_canny_edge(IplImage *src_image, double sigma, int threshold, int method);

	// ĳ�� ���� ���� : cvCanny() �Լ� �̿� 
	IplImage *GS_canny_edge_cvCanny(IplImage *src_image, double low_threshold = 0.0, double high_threshold = 255.0);

};
#endif // !defined(AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_)

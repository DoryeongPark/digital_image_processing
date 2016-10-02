// Geometry.h: interface for the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>

#if !defined(AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_)
#define AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeometry
{
public:
	CGeometry();
	virtual ~CGeometry();

	// �ܼ� Ȯ��/���
	IplImage *GS_enlarge(IplImage *src_image, int resize_height, int resize_width);

	// �ֱ��� �̿� ������
	IplImage *GS_neighbor_interp(IplImage *src_image, int resize_height, int reisize_width);

	// �缱�� ������
	IplImage *GS_bilinear_interp(IplImage *src_image, int resize_height, int reisize_width);

	// ���� : �߰���/��հ�
	IplImage *GS_reduction(IplImage *src_image, double h_scale, double w_scale, int method);

	// ȸ��
	IplImage *GS_rotate(IplImage *src_image, int angle);

	// ������ ��ȯ
	IplImage *GS_affine(IplImage *src_image, CvMat *map_matrix);

	// ������ ��ȯ - ȸ��
	IplImage *GS_affineRotate(IplImage *src_image, double angle, double scale);

	// ���� ��ȯ
	IplImage *GS_perspective(IplImage *src_image, CvPoint2D32f* src_point, CvPoint2D32f* dst_point);

	// �ݻ� 
	IplImage *GS_reflect(IplImage *src_image, int flip_mode);

	// �̵�
	IplImage *GS_translate(IplImage *src_image, int trans_height, int trans_width);

	// ���� ���� ����
	IplImage *GS_setROI(IplImage *src_image, int startH, int startW, int height, int width);

	// ���� ���� ����� ���� ������� ���� 
	void GS_resetROI(IplImage *roi_image);

	// �α�-���� ��ȯ
	IplImage *GS_logPolar(IplImage *src_image, int method);
};

#endif // !defined(AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_)

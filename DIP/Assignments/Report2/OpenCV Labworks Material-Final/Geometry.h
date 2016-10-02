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

	// 단순 확대/축소
	IplImage *GS_enlarge(IplImage *src_image, int resize_height, int resize_width);

	// 최근접 이웃 보간법
	IplImage *GS_neighbor_interp(IplImage *src_image, int resize_height, int reisize_width);

	// 양선형 보간법
	IplImage *GS_bilinear_interp(IplImage *src_image, int resize_height, int reisize_width);

	// 감축 : 중간값/평균값
	IplImage *GS_reduction(IplImage *src_image, double h_scale, double w_scale, int method);

	// 회전
	IplImage *GS_rotate(IplImage *src_image, int angle);

	// 어파인 변환
	IplImage *GS_affine(IplImage *src_image, CvMat *map_matrix);

	// 어파인 변환 - 회전
	IplImage *GS_affineRotate(IplImage *src_image, double angle, double scale);

	// 원근 변환
	IplImage *GS_perspective(IplImage *src_image, CvPoint2D32f* src_point, CvPoint2D32f* dst_point);

	// 반사 
	IplImage *GS_reflect(IplImage *src_image, int flip_mode);

	// 이동
	IplImage *GS_translate(IplImage *src_image, int trans_height, int trans_width);

	// 관심 영역 추출
	IplImage *GS_setROI(IplImage *src_image, int startH, int startW, int height, int width);

	// 관심 영역 추출된 것을 원래대로 복원 
	void GS_resetROI(IplImage *roi_image);

	// 로그-폴라 변환
	IplImage *GS_logPolar(IplImage *src_image, int method);
};

#endif // !defined(AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_)

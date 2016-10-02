// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "Geometry.h"
#include "Pixel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeometry::CGeometry()
{

}

CGeometry::~CGeometry()
{

}


// 관심 영역 추출 (extraction the region of interest)
IplImage *CGeometry::GS_setROI(IplImage *src_image,
	int startH, int startW,
	int height, int width)
{

	IplImage *dst_image = cvCloneImage(src_image);

	CvRect roi;
	roi.y = startH; // 높이에 대한 시작 위치 (Starting point for height)
	roi.x = startW; // 너비에 대한 시작 위치 (Starting point for width)
	roi.height = height; // 가져올 높이에 대한 범위 (
	roi.width = width; // 가져올 너비에 대한 범위

	cvSetImageROI(dst_image, roi);

	return dst_image;
}

// 관심 영역 추출된 것을 원래대로 복원  (restoration of the region of interest that was extracted)
void CGeometry::GS_resetROI(IplImage *roi_image)
{
	cvResetImageROI(roi_image);
}


// 반사 (Reflection)
IplImage *CGeometry::GS_reflect(IplImage *src_image, int flip_mode)
{
	// 초기화 (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	// flip_mode = 0 : 상하 뒤집기(flipping)
	// flip_mode > 0 : 좌우 뒤집기(mirroring)
	// flip_mode < 0 : 상하 뒤집기, 좌우 뒤집기 순 수행(flipping + mirroring)
	cvFlip(src_image, dst_image, flip_mode);

	return dst_image;
}

// 원근 변환 (Perspective transform)
IplImage *CGeometry::GS_perspective(IplImage *src_image,
	CvPoint2D32f* src_point,
	CvPoint2D32f* dst_point)
{
	// 초기화 (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	CvMat *map_matrix = cvCreateMat(3, 3, CV_32FC1);
	cvWarpPerspectiveQMatrix(src_point, dst_point, map_matrix);
	cvWarpPerspective(src_image, dst_image, map_matrix);

	// 할당한 메모리 해제 (memory deallocation)
	cvReleaseMat(&map_matrix);

	return dst_image;
}

// 어파인 변환 (Affine transform)
IplImage *CGeometry::GS_affine(IplImage *src_image, CvMat *map_matrix)
{
	// 초기화 (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	cvWarpAffine(src_image, dst_image, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);

	return dst_image;
}

// 어파인 변환을 이용한 회전 (Rotation using affine transform)
IplImage *CGeometry::GS_affineRotate(IplImage *src_image, double angle, double scale)
{
	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	// 회전 중심점 (center point of rotation)
	CvPoint2D32f center = cvPoint2D32f(width / 2.0, height / 2.0);

	CvMat *map_matrix = cvCreateMat(2, 3, CV_32FC1);

	// angle : 회전 각도
	// scale : 확대/축소 비율 
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	cvWarpAffine(src_image, dst_image, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);

	// 할당한 메모리 해제 (memory deallocation)
	cvReleaseMat(&map_matrix);

	return dst_image;
}


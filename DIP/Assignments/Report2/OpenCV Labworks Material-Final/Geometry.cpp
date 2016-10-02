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


// ���� ���� ���� (extraction the region of interest)
IplImage *CGeometry::GS_setROI(IplImage *src_image,
	int startH, int startW,
	int height, int width)
{

	IplImage *dst_image = cvCloneImage(src_image);

	CvRect roi;
	roi.y = startH; // ���̿� ���� ���� ��ġ (Starting point for height)
	roi.x = startW; // �ʺ� ���� ���� ��ġ (Starting point for width)
	roi.height = height; // ������ ���̿� ���� ���� (
	roi.width = width; // ������ �ʺ� ���� ����

	cvSetImageROI(dst_image, roi);

	return dst_image;
}

// ���� ���� ����� ���� ������� ����  (restoration of the region of interest that was extracted)
void CGeometry::GS_resetROI(IplImage *roi_image)
{
	cvResetImageROI(roi_image);
}


// �ݻ� (Reflection)
IplImage *CGeometry::GS_reflect(IplImage *src_image, int flip_mode)
{
	// �ʱ�ȭ (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	// flip_mode = 0 : ���� ������(flipping)
	// flip_mode > 0 : �¿� ������(mirroring)
	// flip_mode < 0 : ���� ������, �¿� ������ �� ����(flipping + mirroring)
	cvFlip(src_image, dst_image, flip_mode);

	return dst_image;
}

// ���� ��ȯ (Perspective transform)
IplImage *CGeometry::GS_perspective(IplImage *src_image,
	CvPoint2D32f* src_point,
	CvPoint2D32f* dst_point)
{
	// �ʱ�ȭ (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	CvMat *map_matrix = cvCreateMat(3, 3, CV_32FC1);
	cvWarpPerspectiveQMatrix(src_point, dst_point, map_matrix);
	cvWarpPerspective(src_image, dst_image, map_matrix);

	// �Ҵ��� �޸� ���� (memory deallocation)
	cvReleaseMat(&map_matrix);

	return dst_image;
}

// ������ ��ȯ (Affine transform)
IplImage *CGeometry::GS_affine(IplImage *src_image, CvMat *map_matrix)
{
	// �ʱ�ȭ (Initialization)
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	cvWarpAffine(src_image, dst_image, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);

	return dst_image;
}

// ������ ��ȯ�� �̿��� ȸ�� (Rotation using affine transform)
IplImage *CGeometry::GS_affineRotate(IplImage *src_image, double angle, double scale)
{
	// �ʱ�ȭ (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cvCloneImage(src_image);
	cvSetZero(dst_image);

	// ȸ�� �߽��� (center point of rotation)
	CvPoint2D32f center = cvPoint2D32f(width / 2.0, height / 2.0);

	CvMat *map_matrix = cvCreateMat(2, 3, CV_32FC1);

	// angle : ȸ�� ����
	// scale : Ȯ��/��� ���� 
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	cvWarpAffine(src_image, dst_image, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);

	// �Ҵ��� �޸� ���� (memory deallocation)
	cvReleaseMat(&map_matrix);

	return dst_image;
}


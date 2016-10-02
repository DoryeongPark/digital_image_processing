// Edge.cpp: implementation of the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "Edge.h"
#include "Util.h"
#include "Filter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge::CEdge()
{

}

CEdge::~CEdge()
{

}

// 캐니 에지 추출 : cvCanny() 함수 이용  (Extraction canny edge : using cvCanny())
IplImage *CEdge::GS_canny_edge_cvCanny(IplImage *src_image, double low_threshold, double high_threshold)
{
	CUtil cUtil;

	// 초기화 (Initialization)
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 
	IplImage *canny_edge = cvCloneImage(dst_image);

	// (1) cvCanny() 함수 수행시는 src_image와 dst_image의 depth는 같아야 함. (depth of src_image and dst_image shoud be the same in cvCanny())
	// (2) low_threshold, high_threshold는 Edge linking 처리하기 위한 값.(low_threshold, high_threshold are for Edge linking processing)
	cvCanny(src_image, canny_edge, low_threshold, high_threshold, 3);
	cvConvertScaleAbs(canny_edge, dst_image, 1, 0);

	return dst_image;
}

// 캐니 에지 추출 (Extraction canny edg)
IplImage *CEdge::GS_canny_edge(IplImage *src_image, double sigma, int threshold, int method)
{
	int i, j;
	double Gx_sum, Gy_sum, ret_var;
	CUtil cUtil;
	CFilter cFilter;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 가우시안 스무딩 (Gaussian smoothing)
	int mask_height, mask_width;
	GS_get_LoG_mask(sigma, &mask_height, &mask_width);
	IplImage *gs_image = cFilter.GS_gaussian_smoothing_filtering(src_image, sigma);

	// 처리 시간을 줄이기 위해 마스크를 쓰지 않고 직접 곱한다.
	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 소벨
			if (method == 0)
			{
				// 수직 마스크 (vertical mask)
				Gx_sum = -cvGetReal2D(gs_image, i, j)
					- 2 * cvGetReal2D(gs_image, i + 1, j)
					- cvGetReal2D(gs_image, i + 2, j)
					+ cvGetReal2D(gs_image, i, j + 2)
					+ 2 * cvGetReal2D(gs_image, i + 1, j + 2)
					+ cvGetReal2D(gs_image, i + 2, j + 2);
				// 수평 마스크 (horizontal mask)
				Gy_sum = -cvGetReal2D(gs_image, i, j)
					- 2 * cvGetReal2D(gs_image, i, j + 1)
					- cvGetReal2D(gs_image, i, j + 2)
					+ cvGetReal2D(gs_image, i + 2, j)
					+ 2 * cvGetReal2D(gs_image, i + 2, j + 1)
					+ cvGetReal2D(gs_image, i + 2, j + 2);
			}
			// 프리윗
			else if (method == 1)
			{
				// 수직 마스크 (vertical mask)
				Gx_sum = -cvGetReal2D(gs_image, i, j)
					- cvGetReal2D(gs_image, i + 1, j)
					- cvGetReal2D(gs_image, i + 2, j)
					+ cvGetReal2D(gs_image, i, j + 2)
					+ cvGetReal2D(gs_image, i + 1, j + 2)
					+ cvGetReal2D(gs_image, i + 2, j + 2);
				// 수평 마스크 (horizontal mask)
				Gy_sum = -cvGetReal2D(gs_image, i, j)
					- cvGetReal2D(gs_image, i, j + 1)
					- cvGetReal2D(gs_image, i, j + 2)
					+ cvGetReal2D(gs_image, i + 2, j)
					+ cvGetReal2D(gs_image, i + 2, j + 1)
					+ cvGetReal2D(gs_image, i + 2, j + 2);
			}
			// 로버츠
			else if (method == 2)
			{
				// 대각선 방향 (diagonal direction)
				Gx_sum = -cvGetReal2D(gs_image, i, j + 2) + cvGetReal2D(gs_image, i + 1, j + 1);

				// 역대각선 방향 (reverse diagonal direction)
				Gy_sum = -cvGetReal2D(gs_image, i, j) + cvGetReal2D(gs_image, i + 1, j + 1);
			}

			ret_var = fabs(Gx_sum) + fabs(Gy_sum);

			// hysteresis 수행
			// 즉, lower, upper 경계값 처리 하는 것.(lower, upper boundary value)
			// 여기서 upper를 255로 정하였다. (upper==255)
			if (ret_var > 255.0) ret_var = 0.0;
			else if (ret_var < (double)threshold) ret_var = 0.0;

			cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
		}
	}

	return dst_image;
}
// LoG(Laplacian of Gaussian) 에지 추출 : 공식 (Extraction LoG(Laplacian of Gaussian) edge: formula)


double **CEdge::GS_get_LoG_mask(double sigma, int *mask_height, int *mask_width)
{
	int i, j;
	double tmp;

	// 마스크의 크기를 결정한다. (set the size of mask)
	int size = (int)(3.35*sigma + 0.33);
	int mask_length = 2 * size + 1;

	CUtil cUtil;
	double **mask = cUtil.GS_doubleAlloc2D(mask_length, mask_length);

	for (i = 0; i<mask_length; i++)
	{
		for (j = 0; j<mask_length; j++)
		{
			tmp = GS_get_distance((double)i, (double)j, (double)size, (double)size);
			mask[i][j] = GS_get_LoG_formular(tmp, sigma);
		}
	}

	*mask_height = *mask_width = mask_length;
	return mask;
}

double CEdge::GS_get_LoG_formular(double x, double sigma)
{
	return (x*x - 2.0*sigma*sigma) / (sigma*sigma*sigma*sigma) * gauss(x, sigma);
}

double CEdge::norm(double x, double y)
{
	return sqrt(x*x + y*y);
}

double CEdge::GS_get_distance(double a, double b, double c, double d)
{
	return norm((a - c), (b - d));
}

double CEdge::gauss(double x, double sigma)
{
	return exp((-x*x) / (2.0*sigma*sigma));
}

// LoG(Laplacian of Gaussian) : 마스크 (mask)


// 라플라시안 샤프닝 (Laplacian Sharpening)
// method - 0 : 4 방향 마스크 1 (mask 1 for 4 directions)
//        - 1 : 4 방향 마스크 2 (mask 2 for 4 directions)
//        - 2 : 8 방향 마스크 1 (mask 1 for 8 directions)
//        - 3 : 8 방향 마스크 2 (mask 2 for 8 directions)
IplImage *CEdge::GS_laplacian_sharpening(IplImage *src_image, int method)
{
	CUtil cUtil;

	// 초기화 (Initialization)
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 라플라시안 에지 추출(Extraction Laplacian edge)
	IplImage *laplacian_edge = GS_laplacian_edge(src_image, method);

	// 덧셈 연산 (add)
	cvAdd(src_image, laplacian_edge, dst_image, NULL);

	// 할당한 메모리 해제(memory deallocation)
	cvReleaseImage(&laplacian_edge);

	return dst_image;
}

// 라플라시안 에지 추출 : cvLaplace() (Extraction Laplacian edge: cvLaplace())
IplImage *CEdge::GS_laplacian_edge_cvLaplace(IplImage *src_image)
{
	CUtil cUtil;

	// 초기화 
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 
	IplImage *laplacian_edge = cUtil.GS_createImage(cvGetSize(src_image),
		IPL_DEPTH_32F,
		src_image->nChannels);

	// 3번째 인자가 1이면 아래의 라플라시안 마스크를 갖고 처리한다. (if the third argument is 1, use Laplacian mask below)
	// |0  1  0|
	// |1 -4  1|
	// |0  1  0|
	cvLaplace(src_image, laplacian_edge, 1);
	cvConvertScale(laplacian_edge, dst_image, 1, 0);

	return dst_image;
}

// 라플라시안 에지 추출 : 마스크 (Extraction Laplacian edge: mask)
// method - 0 : 4 방향 마스크 1 (mask 1 for 4 directions)
//        - 1 : 4 방향 마스크 2 (mask 2 for 4 directions)
//        - 2 : 8 방향 마스크 1 (mask 1 for 8 directions)
//        - 3 : 8 방향 마스크 2 (mask 2 for 8 directions)
IplImage *CEdge::GS_laplacian_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double var, ret_var;
	CUtil cUtil;

	// 초기화  (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 라플라시안 마스크 정의  (define Laplacian mask)
	mask_height = mask_width = 3;
	int mask_four1[3][3] = { { 0, -1, 0 }, { -1, 4, -1 }, { 0, -1, 0 } };
	int mask_four2[3][3] = { { 0, 1, 0 }, { 1, -4, 1 }, { 0, 1, 0 } };
	int mask_eight1[3][3] = { { -1, -1, -1 }, { -1, 8, -1 }, { -1, -1, -1 } };
	int mask_eight2[3][3] = { { 1, -2, 1 }, { -2, 4, -2 }, { 1, -2, 1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			ret_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					var = cvGetReal2D(src_image, i + m, j + n);

					if (method == 0) ret_var += var*mask_four1[m][n];
					else if (method == 1) ret_var += var*mask_four2[m][n];
					else if (method == 2) ret_var += var*mask_eight1[m][n];
					else if (method == 3) ret_var += var*mask_eight2[m][n];
				}
			}

			ret_var = cUtil.GS_clamping(ret_var);
			cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
		}
	}

	return dst_image;
}

// 로버츠 에지 추출 (Extraction Roberts edge)
IplImage *CEdge::GS_roberts_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double diagonal_var1, diagonal_var2, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 대각선 마스크 정의 (define diagonal mask)
	mask_height = mask_width = 3;
	int diagonal_mask1[3][3] = { { -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, };
	int diagonal_mask2[3][3] = { { 0, 0, -1 }, { 0, 1, 0 }, { 0, 0, 0 }, };


	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			diagonal_var1 = diagonal_var2 = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 대각선 에지 1 (diagonal edge1)
					diagonal_var1 += cvGetReal2D(src_image, i + m, j + n)*diagonal_mask1[m][n];

					// 대각선 에지 2 (diagonal edge1)
					diagonal_var2 += cvGetReal2D(src_image, i + m, j + n)*diagonal_mask2[m][n];
				}
			}

			diagonal_var1 = fabs(diagonal_var1);
			diagonal_var2 = fabs(diagonal_var2);
			ret_var = diagonal_var1 + diagonal_var2;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, diagonal_var1);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, diagonal_var2);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}

// 서터캐스틱 에지 추출 (Extraction Stochastic edge)
IplImage *CEdge::GS_stochastic_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	double vertical_mask[3][3] = { { -0.97, 0, 0.97 }, { -1, 0, 1 }, { -0.97, 0, 0.97 } };
	double horizontal_mask[3][3] = { { 0.97, 1, 0.97 }, { 0, 0, 0 }, { -0.97, -1, -0.97 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 수직 에지 (vertical edge)
					vertical_var += cvGetReal2D(src_image, i + m, j + n)*vertical_mask[m][n];

					// 수평 에지 (horizontal edge)
					horizontal_var += cvGetReal2D(src_image, i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, vertical_var);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, horizontal_var);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}

// 프레이첸 에지 추출 (Extraction freichen edge)
IplImage *CEdge::GS_freichen_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	double vertical_mask[3][3] = { { -1, 0, 1 }, { -sqrt(2), 0, sqrt(2) }, { -1, 0, 1 } };
	double horizontal_mask[3][3] = { { 1, sqrt(2), 1 }, { 0, 0, 0 }, { -1, -sqrt(2), -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 수직 에지 (vertical edge)
					vertical_var += cvGetReal2D(src_image, i + m, j + n)*vertical_mask[m][n];

					// 수평 에지 (horizontal edge)
					horizontal_var += cvGetReal2D(src_image, i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, vertical_var);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, horizontal_var);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}


// 프리윗 에지 추출 (Extraction prewitt edge)
IplImage *CEdge::GS_prewitt_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int vertical_mask[3][3] = { { -1, 0, 1 }, { -1, 0, 1 }, { -1, 0, 1 } };
	int horizontal_mask[3][3] = { { 1, 1, 1 }, { 0, 0, 0 }, { -1, -1, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 수직 에지 (vertical edge)
					vertical_var += cvGetReal2D(src_image, i + m, j + n)*vertical_mask[m][n];

					// 수평 에지 (horizontal edge)
					horizontal_var += cvGetReal2D(src_image, i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, vertical_var);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, horizontal_var);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}

// 소벨 에지 추출 : 대각선 (Extraction sobel edge: diagonal)
IplImage *CEdge::GS_sobel_diagonal_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double diagonal_var1, diagonal_var2, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int diagonal_mask1[3][3] = { { -2, -1, 0 }, { -1, 0, 1 }, { 0, 1, 2 } };
	int diagonal_mask2[3][3] = { { 0, -1, -2 }, { 1, 0, -1 }, { 2, 1, 0 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			diagonal_var1 = diagonal_var2 = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 대각선 에지 1 (diagonal edge1)
					diagonal_var1 += cvGetReal2D(src_image, i + m, j + n)*diagonal_mask1[m][n];

					// 대각선 에지 2 (diagonal edge2)
					diagonal_var2 += cvGetReal2D(src_image, i + m, j + n)*diagonal_mask2[m][n];
				}
			}

			diagonal_var1 = fabs(diagonal_var1);
			diagonal_var2 = fabs(diagonal_var2);
			ret_var = diagonal_var1 + diagonal_var2;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, diagonal_var1);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, diagonal_var2);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}

// 소벨 에지 추출 : 마스크(7x7) (Extraction sobel edge : mask(7x7))
IplImage *CEdge::GS_sobel_edge_7x7(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 7;
	int vertical_mask[7][7] = { { -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -2, -2, -2, 0, 2, 2, 2 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 } };

	int horizontal_mask[7][7] = { { 1, 1, 1, 2, 1, 1, 1 },
	{ 1, 1, 1, 2, 1, 1, 1 },
	{ 1, 1, 1, 2, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -1, -1, -2, -1, -1, -1 },
	{ -1, -1, -1, -2, -1, -1, -1 },
	{ -1, -1, -1, -2, -1, -1, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 수직 에지 (vertical edge)
					vertical_var += cvGetReal2D(src_image, i + m, j + n)*vertical_mask[m][n];

					// 수평 에지 (horizontal edge)
					horizontal_var += cvGetReal2D(src_image, i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, vertical_var);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, horizontal_var);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}


// 소벨 에지 추출 : cvSobel() 함수 이용(Extraction sobel edge : using cvSobel())
IplImage *CEdge::GS_sobel_edge_cvSobel(IplImage *src_image, int method)
{
	CUtil cUtil;

	// 초기화  (Initialization)
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	//  
	IplImage *vertical_edge = cUtil.GS_createImage(cvGetSize(src_image),
		IPL_DEPTH_32F,
		src_image->nChannels);

	//
	IplImage *horizontal_edge = cUtil.GS_createImage(cvGetSize(src_image),
		IPL_DEPTH_32F,
		src_image->nChannels);

	//  cvSobel() 함수 세 번째 인자값이 3 이면 가우시안 스무딩 적용 
	// if the third argument is 3, apply Gaussian smoothing
	cvSobel(src_image, vertical_edge, 1, 0, 3); // 수직 (vertical)
	cvSobel(src_image, horizontal_edge, 0, 1, 3); // 수평 (horizontal)

	// cvConertScaleAbs() 함수는 IPL_DEPTH_32F를 IPL_DEPTH_8U로 변환.
	if (method == 0) cvConvertScaleAbs(vertical_edge, dst_image, 1, 0);
	else if (method == 1) cvConvertScaleAbs(horizontal_edge, dst_image, 1, 0);
	else if (method == 2)
	{
		IplImage *tmp_image1 = cvCloneImage(dst_image);
		IplImage *tmp_image2 = cvCloneImage(dst_image);

		cvConvertScaleAbs(vertical_edge, tmp_image1, 1, 0);
		cvConvertScaleAbs(horizontal_edge, tmp_image2, 1, 0);

		cvAdd(tmp_image1, tmp_image2, dst_image, NULL);

		cvReleaseImage(&tmp_image1);
		cvReleaseImage(&tmp_image2);
	}

	cvReleaseImage(&vertical_edge);
	cvReleaseImage(&horizontal_edge);

	return dst_image;
}

// 소벨 에지 추출 : 마스크(3x3) (Extraction sobel edge : mask(3x3))
IplImage *CEdge::GS_sobel_edge(IplImage *src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// 초기화 (Initialization)
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	// 수직/수평 마스크 정의 (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int vertical_mask[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	int horizontal_mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// 초기화 (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// 수직 에지 (vertical edge)
					vertical_var += cvGetReal2D(src_image, i + m, j + n)*vertical_mask[m][n];

					// 수평 에지 (horizontal edge)
					horizontal_var += cvGetReal2D(src_image, i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, vertical_var);
			}
			else if (method == 1)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, horizontal_var);
			}
			else if (method == 2)
			{
				cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, ret_var);
			}
		}
	}

	return dst_image;
}




// 그래디언트 에지 추출 (Extraction gradient edge)

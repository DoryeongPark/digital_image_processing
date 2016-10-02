/*
* Filter.cpp : ���� ���� ���͸� ���� Ŭ����
* Modified by Lee, Moon-Ho (conv2@korea.com)
* Last Modified : 2006/11/15
*/

// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "Filter.h"
#include "Util.h"
#include "Color.h"
#include "Pixel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter()
{

}

CFilter::~CFilter()
{

}

// ����þ� ������ ���͸�
IplImage *CFilter::GS_gaussian_smoothing_filtering(IplImage *src_image, double sigma)
{
	int i, j, mask_height, mask_width;
	double var;
	CUtil cUtil;

	// �ʱ�ȭ 
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (src_image->nChannels != 1) return dst_image;

	// sigma ������ �������� ���� ��
	if (sigma < 0.0)
	{
		//cUtil.GS_errMsg("sigma ���� 0.0 �̻��̾�� �մϴ�.");
		return dst_image;
	}

	// ����ũ�� ���Ѵ�.
	// Gaussian ����ũ�� ũ�⸦ �����Ѵ�.
	// length = 5 �̸� 2D�� ��� 5x5 ����ũ�̴�.
	int length = (int)(6 * sigma) + 1;
	int center = length / 2;

	mask_height = mask_width = length;
	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);
	double M_PI = 3.141592654;

	for (i = 0; i<mask_height; i++)
	{
		for (j = 0; j<mask_width; j++)
		{
			var = (1.0 / (2.0*M_PI*sigma*sigma))
				* exp(-((i - center)*(i - center) + (j - center)*(j - center))
				*(1.0 / (2.0*sigma*sigma)));

			mask[i][j] = (float)var;
		}
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, single_mask);

	// ȸ�� �� dst_image�� ����
	IplImage *tmp_image = GS_conv2(src_image, &mat_kernel);
	cvCopy(tmp_image, dst_image, NULL);

	free(single_mask);
	cvReleaseImage(&tmp_image);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// �ִ�/�ּҰ� ���͸�

// ��-trimmed ��հ� ���͸�

IplImage *CFilter::GS_mean_filtering(IplImage *src_image, int mask_height, int mask_width)
{
	int i, j, m, n;
	CUtil cUtil;
	double sum, var;

	// �ʱ�ȭ 
	int height = src_image->height;
	int width = src_image->width;
	IplImage *dst_image = cUtil.GS_createImage(cvGetSize(src_image), src_image->nChannels);
	if (dst_image->nChannels != 1) return dst_image;

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			sum = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			// mask_height x mask_width�� ����� ��� mask�� �Ҵ�.
			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					sum += cvGetReal2D(src_image, m + i, n + j);
				}
			}

			// ��հ��� ���Ѵ�.
			var = sum / (mask_height*mask_width);
			cvSetReal2D(dst_image, i + (mask_height - 1) / 2, j + (mask_width - 1) / 2, var);
		}
	}

	return dst_image;
}

// �̵�� ���͸�



// �� ����Ʈ/���� ���� 
IplImage *CFilter::GS_blur_lb_filter(IplImage *src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	// �� ����Ʈ ����
	if (method == 0)
	{
		mask[0][0] = 1.0f; mask[0][1] = 2.0f; mask[0][2] = 1.0f;
		mask[1][0] = 2.0f; mask[1][1] = 2.0f; mask[1][2] = 2.0f;
		mask[2][0] = 1.0f; mask[2][1] = 2.0f; mask[2][2] = 1.0f;
	}
	// �� ���� ���� 
	else if (method == 1)
	{
		mask[0][0] = 1.0f; mask[0][1] = 2.0f; mask[0][2] = 1.0f;
		mask[1][0] = 2.0f; mask[1][1] = 4.0f; mask[1][2] = 2.0f;
		mask[2][0] = 1.0f; mask[2][1] = 2.0f; mask[2][2] = 1.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, single_mask);

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = (float)cvmGet(&mat_kernel, i, j);

			if (method == 0) cvmSet(&mat_kernel, i, j, tmp / 14.0f);
			else if (method == 1) cvmSet(&mat_kernel, i, j, tmp / 16.0f);
		}
	}

	// ȸ�� 
	IplImage *dst_image = GS_conv2(src_image, &mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ���ڽ� ��Ŀ��/������ ����
IplImage *CFilter::GS_enhance_filter(IplImage *src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	// ���ڽ� ��Ŀ�� ����
	if (method == 0)
	{
		mask[0][0] = -1.0f; mask[0][1] = 0.0f; mask[0][2] = -1.0f;
		mask[1][0] = 0.0f; mask[1][1] = 7.0f; mask[1][2] = 0.0f;
		mask[2][0] = -1.0f; mask[2][1] = 0.0f; mask[2][2] = -1.0f;
	}
	// ���ڽ� ������ ���� 
	else if (method == 1)
	{
		mask[0][0] = 0.0f; mask[0][1] = -1.0f; mask[0][2] = 0.0f;
		mask[1][0] = -1.0f; mask[1][1] = 9.0f; mask[1][2] = -1.0f;
		mask[2][0] = 0.0f; mask[2][1] = -1.0f; mask[2][2] = 0.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, single_mask);

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = (float)cvmGet(&mat_kernel, i, j);

			if (method == 0) cvmSet(&mat_kernel, i, j, tmp / 3.0f);
			else if (method == 1) cvmSet(&mat_kernel, i, j, tmp / 5.0f);
		}
	}

	// ȸ�� 
	IplImage *dst_image = GS_conv2(src_image, &mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������ ����
IplImage *CFilter::GS_soften_filter(IplImage *src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	if (method == 0)
	{
		mask[0][0] = 11.0f; mask[0][1] = 11.0f; mask[0][2] = 11.0f;
		mask[1][0] = 11.0f; mask[1][1] = 11.0f; mask[1][2] = 11.0f;
		mask[2][0] = 11.0f; mask[2][1] = 11.0f; mask[2][2] = 11.0f;
	}
	else if (method == 1)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = 10.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}
	else if (method == 2)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = 4.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, single_mask);

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = (float)cvmGet(&mat_kernel, i, j);

			if (method == 0) cvmSet(&mat_kernel, i, j, tmp / 99.0f);
			else if (method == 1) cvmSet(&mat_kernel, i, j, tmp / 18.0f);
			else if (method == 2) cvmSet(&mat_kernel, i, j, tmp / 12.0f);
		}
	}

	// ȸ�� 
	IplImage *dst_image = GS_conv2(src_image, &mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������
IplImage *CFilter::GS_sharpening(IplImage *src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	if (method == 0)
	{
		mask[0][0] = 1.0f; mask[0][1] = -2.0f; mask[0][2] = 1.0f;
		mask[1][0] = -2.0f; mask[1][1] = 5.0f; mask[1][2] = -2.0f;
		mask[2][0] = 1.0f; mask[2][1] = -2.0f; mask[2][2] = 1.0f;
	}
	else if (method == 1)
	{
		mask[0][0] = 0.0f; mask[0][1] = -1.0f; mask[0][2] = 0.0f;
		mask[1][0] = -1.0f; mask[1][1] = 5.0f; mask[1][2] = -1.0f;
		mask[2][0] = 0.0f; mask[2][1] = -1.0f; mask[2][2] = 0.0f;
	}
	else if (method == 2)
	{
		mask[0][0] = -1.0f; mask[0][1] = -1.0f; mask[0][2] = -1.0f;
		mask[1][0] = -1.0f; mask[1][1] = 9.0f; mask[1][2] = -1.0f;
		mask[2][0] = -1.0f; mask[2][1] = -1.0f; mask[2][2] = -1.0f;
	}
	// ��ϴ�� ���� ����ũ�� �˷��� �ִ�.
	else if (method == 3)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = -7.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}
	else if (method == 4)
	{
		mask[0][0] = 0.0f;      mask[0][1] = -1.0f / 6.0f;  mask[0][2] = 0.0f;
		mask[1][0] = -1.0f / 6.0f; mask[1][1] = 10.0f / 6.0f;  mask[1][2] = -1.0f / 6.0f;
		mask[2][0] = 0.0f;      mask[2][1] = -1.0f / 6.0f;   mask[2][2] = 0.0f;
	}
	else if (method == 5)
	{
		mask[0][0] = 0.0f;      mask[0][1] = -1.0 / 4.0f;  mask[0][2] = 0.0f;
		mask[1][0] = -1.0f / 4.0f; mask[1][1] = 8.0f / 4.0f; mask[1][2] = -1.0f / 4.0f;
		mask[2][0] = 0.0f;      mask[2][1] = -1.0 / 4.0f;  mask[2][2] = 0.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, single_mask);

	// ȸ�� 
	IplImage *dst_image = GS_conv2(src_image, &mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������ ���� ��� ���͸�
IplImage *CFilter::GS_high_boost(IplImage *src_image, float alpha)
{
	return GS_high_boost(src_image, alpha, 0.0f);
}

IplImage *CFilter::GS_high_boost(IplImage *src_image, float alpha, float bias)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;
	int mask_mid_height = (mask_height + 1) / 2;
	int mask_mid_width = (mask_width + 1) / 2;

	// ����ũ ��� ����
	float mask_weights = (float)(mask_height*mask_width);

	// ����ũ �ʱ�ȭ
	CvMat *mat_kernel = cvCreateMat(mask_height, mask_width, CV_32FC1);
	cvSet(mat_kernel, cvScalarAll(-alpha / mask_weights));

	// �߽� ��� ����
	float mask_mid_var = (float)(mask_weights * alpha - 1.0) / mask_weights;
	cvmSet(mat_kernel, mask_mid_height, mask_mid_width, mask_mid_var);

	// ȸ�� 
	IplImage *tmp_image = GS_conv2(src_image, mat_kernel);

	// bias��ŭ �����ش�.
	IplImage *dst_image = cvCloneImage(tmp_image);
	cvAddS(tmp_image, cvScalarAll(bias), dst_image, NULL);

	// �Ҵ��� �޸� ���� 
	cvReleaseMat(&mat_kernel);
	cvReleaseImage(&tmp_image);

	return dst_image;
}

// �������
IplImage *CFilter::GS_unsharpening(IplImage *src_image, int method)
{
	return GS_unsharpening(src_image, method, 0.0f);
}

IplImage *CFilter::GS_unsharpening(IplImage *src_image, int method, float bias)
{
	CPixel cPixel;

	// ������ ��� ���͸� : ����
	IplImage *blurring_image = GS_blurring(src_image, 0, method);

	// �� ���� - ������ ��� ���͸� ����
	IplImage *tmp_image = cPixel.GS_subtract_image(src_image, blurring_image);

	// bias ��ŭ �����ش�.
	IplImage *dst_image = cvCloneImage(tmp_image);
	cvAddS(tmp_image, cvScalarAll(bias), dst_image, NULL);

	// �Ҵ��� �޸� ����
	cvReleaseImage(&tmp_image);

	return dst_image;
}

// ����
IplImage *CFilter::GS_blurring(IplImage *src_image, int option, int method)
{
	IplImage *dst_image = (IplImage *)NULL;

	// cvFilter2D() �Լ� ���
	if (option == 0)
	{
		dst_image = GS_blurring_cvFilter2D(src_image, method);
	}
	// cvSmooth() �Լ� ���
	else
	{
		dst_image = GS_blurring_cvSmooth(src_image, option, method);
	}

	return dst_image;
}

// ���� - cvFilter2D() �Լ� ���
IplImage *CFilter::GS_blurring_cvFilter2D(IplImage *src_image, int method)
{
	CUtil cUtil;
	int i, j;
	int mask_height, mask_width;

	// method : 0�̸� 3x3 ����ũ, 1�̸� 5x5 ����ũ, 2�̸� 7x7 ����ũ
	if (method == 0) mask_height = mask_width = 3;
	else if (method == 1) mask_height = mask_width = 5;
	else if (method == 2) mask_height = mask_width = 7;

	// ����ũ �ʱ�ȭ
	CvMat *mat_kernel = cvCreateMat(mask_width, mask_height, CV_32FC1);
	cvZero(mat_kernel);

	// ����ũ�� ��� ����
	int mask_length = mask_height*mask_width;
	for (i = 0; i<mask_height; i++)
		for (j = 0; j<mask_width; j++)
			cvmSet(mat_kernel, i, j, 1.0f / (float)mask_length);

	// ȸ�� 
	IplImage *dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ���� 
	cvReleaseMat(&mat_kernel);

	return dst_image;
}

// ���� - cvSmooth() �Լ� ���
IplImage *CFilter::GS_blurring_cvSmooth(IplImage *src_image, int option, int method)
{
	int mask_height, mask_width;

	// �ʱ�ȭ
	IplImage *dst_image = cvCreateImage(cvGetSize(src_image),
		IPL_DEPTH_8U,
		src_image->nChannels);

	// method : 0�̸� 3x3 ����ũ, 1�̸� 5x5 ����ũ, 2�̸� 7x7 ����ũ
	if (method == 0) mask_height = mask_width = 3;
	else if (method == 1) mask_height = mask_width = 5;
	else if (method == 2) mask_height = mask_width = 7;

	// option �� üũ
	// option : 1 - CV_BLUR,   
	// option : 2 - CV_GAUSSIAN
	// option : 3 - CV_MEDIAN
	if (option >= CV_BLUR && option <= CV_MEDIAN)
	{
		// ������ �� ���ڴ� ����ũ�� �ʺ�� ���̸� ����.
		cvSmooth(src_image, dst_image, option, mask_width, mask_height);
	}
	// option : 4 - CV_BILATERAL
	else if (option == CV_BILATERAL)
	{
		// ������ �� ���ڴ� "color sigma" �� "space sigma" �� ����.
		cvSmooth(src_image, dst_image, option, 100, 100);
	}

	return dst_image;
}

// ��ϵ� ���󿡼��� �簢 ȿ�� - ��ģ ȿ��
IplImage *CFilter::GS_wild_embossing(IplImage *src_image, int method)
{
	CUtil cUtil;
	int mask_length = 9;

	// North
	float mask_north[] = { -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f };
	float mask_west[] = { -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f };
	float mask_south[] = { 1.0, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -2.0f, -1.0f };
	float mask_east[] = { 1.0f, 0.0f, -1.0f, 2.0f, 0.0f, -2.0f, 1.0f, 0.0f, -1.0f };
	float mask_northeast[] = { 0.0f, -1.0f, -2.0f, 1.0f, 0.0f, -1.0f, 2.0f, 1.0f, 0.0f };


	// 1���� �޸� �Ҵ�
	float *mask = cUtil.GS_floatAlloc1D(mask_length);

	// ����ũ ����
	if (method == 0)
	{
		memcpy(mask, mask_north, sizeof(float)*mask_length);
	}
	else if (method == 1)
	{
		memcpy(mask, mask_west, sizeof(float)*mask_length);
	}
	else if (method == 2)
	{
		memcpy(mask, mask_south, sizeof(float)*mask_length);
	}
	else if (method == 3)
	{
		memcpy(mask, mask_east, sizeof(float)*mask_length);
	}
	else if (method == 4)
	{
		memcpy(mask, mask_northeast, sizeof(float)*mask_length);
	}

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(3, 3, CV_32FC1, mask);

	// ȸ�� 
	IplImage *tmp_image = GS_conv2(src_image, &mat_kernel);

	// �� ������ ��հ��� ���Ѵ�.
	CvScalar avg = cvAvg(src_image, NULL);

	// ��հ��� ���Ѵ�.
	IplImage *dst_image = cvCloneImage(tmp_image);
	cvAddS(tmp_image, avg, dst_image, NULL);

	// �Ҵ��� �޸� ����
	cvReleaseImage(&tmp_image);
	cUtil.GS_free1D(mask);

	return dst_image;
}


// HSV �÷� ���������� �簢 ȿ��
IplImage *CFilter::GS_hsv_embossing(IplImage *src_image, int method)
{
	// ä�� �и�
	IplImage *red, *green, *blue;
	IplImage *hue, *saturation, *value;

	CColor cColor;
	cColor.GS_splitRGB(src_image, &red, &green, &blue);
	cColor.GS_rgb2hsv(red, green, blue, &hue, &saturation, &value);

	// �簢 ȿ�� ����
	IplImage *value_embossing = GS_embossing(value, method);

	IplImage *new_red, *new_green, *new_blue;
	cColor.GS_hsv2rgb(hue, saturation, value_embossing, &new_red, &new_green, &new_blue);
	IplImage *dst_image = cColor.GS_compositeRGB(new_red, new_green, new_blue);

	// �Ҵ��� �޸� ����
	cvReleaseImage(&red); cvReleaseImage(&green); cvReleaseImage(&blue);
	cvReleaseImage(&new_red); cvReleaseImage(&new_green); cvReleaseImage(&new_blue);

	cvReleaseImage(&hue); cvReleaseImage(&saturation); cvReleaseImage(&value);
	cvReleaseImage(&value_embossing);

	cvReleaseImage(&value_embossing);

	return dst_image;
}

// RGB �÷� ���������� �簢 ȿ��
IplImage *CFilter::GS_rgb_embossing(IplImage *src_image, int method)
{
	/** ���� : cvFilter2D() �Լ��� 3�������� ó���� �� �����Ƿ�,
	�Ʒ� �ڵ� ��� �ٲ㵵 �Ȱ���.
	return GS_embossing( src_image, method );
	*/

	CColor cColor;

	// ä�� �и�
	IplImage *red, *green, *blue;
	cColor.GS_splitRGB(src_image, &red, &green, &blue);

	// �簢 ȿ�� ����
	IplImage *red_embossing = GS_embossing(red, method);
	IplImage *green_embossing = GS_embossing(green, method);
	IplImage *blue_embossing = GS_embossing(blue, method);

	IplImage *dst_image = cColor.GS_compositeRGB(red_embossing,
		green_embossing,
		blue_embossing);

	// �Ҵ��� �޸� ����
	cvReleaseImage(&red);
	cvReleaseImage(&green);
	cvReleaseImage(&blue);
	cvReleaseImage(&red_embossing);
	cvReleaseImage(&green_embossing);
	cvReleaseImage(&blue_embossing);

	return dst_image;
}

// ��ϵ� ���󿡼��� �簢 ȿ��
IplImage *CFilter::GS_embossing(IplImage *src_image, int method)
{
	CUtil cUtil;
	int mask_height = 3;
	int mask_width = 3;
	int mask_length = mask_height*mask_width;

	// 135��
	float mask_135[] = { -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f };
	// 90��
	float mask_90[] = { 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f };

	// 45�� 
	float mask_45[] = { 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f };

	// 1���� �޸� �Ҵ�
	float *mask = cUtil.GS_floatAlloc1D(mask_length);

	// ����ũ ����
	if (method == 0)
	{
		memcpy(mask, mask_135, sizeof(float)*mask_length);
	}
	else if (method == 1)
	{
		memcpy(mask, mask_90, sizeof(float)*mask_length);
	}
	else if (method == 2)
	{
		memcpy(mask, mask_45, sizeof(float)*mask_length);
	}

	// CvMat���� ��ȯ
	CvMat mat_kernel = cvMat(mask_width, mask_height, CV_32FC1, mask);

	// ȸ�� 
	IplImage *tmp_image = GS_conv2(src_image, &mat_kernel);

	// �� ������ ��հ��� ���Ѵ�.
	CvScalar avg = cvAvg(src_image, NULL);

	// ��հ��� ���Ѵ�.
	IplImage *dst_image = cvCloneImage(tmp_image);
	cvAddS(tmp_image, avg, dst_image, NULL);

	// �Ҵ��� �޸� ����
	cvReleaseImage(&tmp_image);
	cUtil.GS_free1D(mask);

	return dst_image;
}

// ȸ�� ���� 
IplImage *CFilter::GS_conv2(IplImage *src_image, CvMat *mat_kernel)
{
	// ȸ�� ����(=���͸�) ����� ���� ���� ���� �ʱ�ȭ
	IplImage *dst_image = cvCreateImage(cvGetSize(src_image),
		IPL_DEPTH_8U,
		src_image->nChannels);

	// ���͸� ���� : ����ũ�� ���� ȸ��(convolution)
	cvFilter2D(src_image, dst_image, mat_kernel, cvPoint(-1, -1));

	return dst_image;
}

// 1���� Ŀ���� ���� ���͸�. - ��
IplImage *CFilter::ApplyFilter2D(IplImage *src_image, float *kernel, int kHeight, int kWidth)
{
	// CvMat���� ��ȯ.
	CvMat mat_kernel = cvMat(kWidth, kHeight, CV_32FC1, kernel);

	// �����ε��� ��� �Լ� �� ȣ��
	IplImage *dst_image = ApplyFilter2D(src_image, mat_kernel);

	// �Ҵ��� �޸� ���� 
	cvReleaseData(&mat_kernel);

	return dst_image;
}

// 2���� Ŀ���� ���� ���͸�. - ��
IplImage *CFilter::ApplyFilter2D(IplImage *src_image, float **kernel, int kHeight, int kWidth)
{
	int cnt = -1;

	// 2������ 1�������� ��ȯ.
	float *single_kernel = (float *)calloc(kHeight*kWidth, sizeof(float));

	for (int i = 0; i<kHeight; i++)
	{
		for (int j = 0; j<kWidth; j++)
		{
			single_kernel[++cnt] = kernel[i][j];
		}
	}

	// �����ε��� ��� �Լ� �� ȣ��.
	IplImage *dst_image = ApplyFilter2D(src_image, single_kernel, kHeight, kWidth);

	// �Ҵ��� �޸� ����
	free(single_kernel);

	return dst_image;
}

// CvMat�� kernel�� ���� ���͸�. - ��
IplImage *CFilter::ApplyFilter2D(IplImage *src_image, CvMat kernel)
{
	// ���͸� ����� ���� ���� ���� �ʱ�ȭ
	IplImage *dst_image = cvCreateImage(cvGetSize(src_image),
		IPL_DEPTH_8U,
		src_image->nChannels);

	// ���͸� ���� : kernel�� ���� ȸ��(convolution)
	cvFilter2D(src_image, dst_image, &kernel, cvPoint(-1, -1));

	return dst_image;
}



/*
 * Pixel.cpp : Pixel Processing Class
 * Modified by Lee, Moon-Ho (conv2@korea.com), 2006/12/20
 * Additionally modified by Jung, Jin-Woo
 * Last Modified : 2015/03/27
 */

#include "Pixel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPixel::CPixel()
{

}

CPixel::~CPixel()
{

}

// Logical operation with two gray-scale images - bitwise operator
// Input: two 8bit gray-scale images + bitwise operator (AND 0, NAND 1, OR 2, NOR 3, XOR 4, Difference 5)
IplImage *CPixel::GS_gray_logic( IplImage *src_image1, IplImage *src_image2, int logic_method )
{

	// Initialization
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image1), 
										 IPL_DEPTH_8U, 
										 src_image1->nChannels );
	IplImage *tmp_image = cvCloneImage( dst_image );

	switch(logic_method)
	{
		// AND
		case 0 :
				// var = (var1 & var2); 
				cvAnd(src_image1, src_image2, dst_image, NULL); 
				break;
		// NAND
		case 1 :    
				//var = ~(var1 & var2); 
				cvAnd(src_image1, src_image2, tmp_image, NULL);
				cvNot(tmp_image, dst_image);
				break;
		// OR
		case 2 :
				//var = (var1 | var2);
				cvOr(src_image1, src_image2, dst_image, NULL); 
				break;
		// NOR
		case 3 :
				//var = ~(var1 | var2);
				cvOr(src_image1, src_image2, tmp_image, NULL);
				cvNot(tmp_image, dst_image);
				break;
		// XOR
		case 4 :
				//var = var1 ^ var2;
				cvXor(src_image1, src_image2, dst_image, NULL); 
				break;
		// Difference
		case 5 :
				//var = var & (~var2);
				cvNot(src_image2, tmp_image);
				cvAnd(src_image1, tmp_image, dst_image, NULL);
				break;  
		default :
				break;           
	} // end switch

	if (tmp_image) cvReleaseImage( &tmp_image );

	return dst_image;
}

// Logical operation with two binary images - logical operator
// Input: two binary images + logical operator (AND 0, NAND 1, OR 2, NOR 3, XOR 4, XNOR 5)
IplImage *CPixel::GS_binary_logic( IplImage *src_image1, IplImage *src_image2, int logic_method )
{
	int i, j;
	BYTE var, var1,var2;

	// 초기화
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image1), 
										 IPL_DEPTH_8U, 
										 src_image1->nChannels );

	int height = src_image1->height;
	int width = src_image1->width;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			var1 = ((BYTE)cvGetReal2D( src_image1, i, j ) == 0 ? 1 : 0);
			var2 = ((BYTE)cvGetReal2D( src_image2, i, j ) == 0 ? 1 : 0);

			switch(logic_method)
			{
				// AND
				case 0 :
						var = (var1 && var2); 
						break;
				// NAND
				case 1 :    
						var = !(var1 && var2); 
						break;
				// OR
				case 2 :
						var = (var1 || var2);
						break;
				// NOR
				case 3 :
						var = !(var1 || var2);
						break;
				// XOR
				case 4 :
						var = (var1 != var2 ? 1 : 0);
						break;
				// XNOR
				case 5 :
						var = (var1 == var2 ? 1 : 0);
						break;  
				default :
						break;           
			} // end switch
			 
			cvSetReal2D( dst_image, i, j, (var == 0.0) ? 1.0 : 0.0 );

		} // end for
	} // end for

	return dst_image;
}

// Convert binary image to gray-scale image
// Input: binary image
IplImage *CPixel::GS_binary2gray( IplImage *src_image )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );
	
	// Create an image with all 1 pixel values 
	IplImage *tmp_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );
	cvSet( tmp_image, cvScalarAll(1), NULL );

	// multiplication : dst(I)=contrast*src1(I)*src2(I)
	cvMul( src_image, tmp_image, dst_image, 255.0 );

	if (tmp_image) cvReleaseImage( &tmp_image );

	return dst_image;
}

// Binarization by adaptive thresholding
/* Input: 
src – Source 8-bit single-channel image.
dst – Destination image of the same size and the same type as src .
maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. 
adaptiveMethod – Adaptive thresholding algorithm to use, ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C.
thresholdType – Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV.
blockSize – Size of a pixel
*/
IplImage *CPixel::GS_adaptive_threshold( IplImage *src_image, double max_value, 
										 int adaptive_method, int threshold_type)
{
	return GS_adaptive_threshold( src_image, max_value, adaptive_method, threshold_type, 3, 5);
}

IplImage *CPixel::GS_adaptive_threshold( IplImage *src_image, double max_value, 
										 int adaptive_method, int threshold_type, 
										 int block_size, double param )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	cvAdaptiveThreshold( src_image, dst_image, max_value, 
						 adaptive_method, threshold_type, 
						 block_size, param);

	return dst_image;
}

// Binarization by thresholding
/* Input: 
src – Source 8-bit single-channel image.
dst – Destination image of the same size and the same type as src .
maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. 
threshold - Threshold value to be checked 
thresholdType – Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV.
*/
IplImage *CPixel::GS_threshold( IplImage *src_image, double threshold, int threshold_type )
{
	double max_value = 0.0;

	if( threshold_type == CV_THRESH_BINARY 
		|| threshold_type == CV_THRESH_BINARY_INV )
	{
		max_value = threshold;
	}

	return GS_threshold( src_image, threshold, max_value, threshold_type );
}

IplImage *CPixel::GS_threshold( IplImage *src_image, double threshold, double max_value, int threshold_type )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	cvThreshold( src_image, dst_image, threshold, max_value, threshold_type );

	return dst_image;
}

// Contrast & brightness adjustment by LUT
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_LUT_basic_contrast_brightness( IplImage *src_image, double contrast, 
						  					        int brightness )
{
	int i, var;

	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	// Create a LUT  
	CvMat *lut = cvCreateMat( 1, 256, CV_8UC1 );

	for(i=0; i<256; i++)
	{
		var = (int)( i*contrast + brightness );
		
		// Clamping for the out-range data 
		if (var > 255 ) var = 255;
		else if (var < 0 ) var = 0;

		lut->data.ptr[i] = (unsigned char)var;
	}

	lut->data.ptr[0] = 0;

	// Types for src_image, dst_image, and lut should be ALL SAME
	//		  CV_8UC1 : 8bit(integer value), CV_32FC1 : 32bit (real value)
	cvLUT( src_image, dst_image, lut );

	return dst_image; 
}

// Histogram Equalization
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_histeq( IplImage *src_image )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	cvEqualizeHist( src_image, dst_image );

	return dst_image;
}

// Histogram creation
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_imhist( IplImage *src_image )
{
	return GS_imhist( src_image, 0 );
}

IplImage *CPixel::GS_imhist( IplImage *src_image, int max_length )
{
	int i, j, bin_size = 256;
	int tmp = 0;

	float ranges[] = { 0, 255 }; 
	float *hist_range[] = {ranges}; 

	int hist_height = 128;
	int hist_width = 256;

	int hist_bar_height = 20;
	
	CvHistogram *hist = cvCreateHist(1, &bin_size, CV_HIST_ARRAY, hist_range, 1); 

	// Calculate Histogram
	cvCalcHist( &src_image, hist, 0, NULL );

	float max_bin_value = 0;
	float min_bin_value = 0;
	int max_level = 0;
	int min_level = 0;
    cvGetMinMaxHistValue( hist, &min_bin_value, &max_bin_value, &min_level, &max_level);

	IplImage* hist_image = cvCreateImage(cvSize(hist_width, hist_height + hist_bar_height),
										 IPL_DEPTH_8U, 1);

	int tmp_hist_height = cvRound(max_bin_value);
	if( max_length > 0 ) tmp_hist_height = max_length;

	// temporary images for making histogram with max_length and for resized histogram 
	IplImage* tmp_hist_image1 = cvCreateImage(cvSize(hist_width, tmp_hist_height),
										 IPL_DEPTH_8U, 1);

	IplImage* tmp_hist_image2 = cvCreateImage(cvSize(hist_width, hist_height),
										 IPL_DEPTH_8U, 1);

	// Making histogram image
	cvSetZero( tmp_hist_image1 );

	for(i=0; i<hist_width; i++)
	{
		tmp = tmp_hist_height - cvRound(cvQueryHistValue_1D(hist,i));
	
		if( tmp == 0 || tmp > tmp_hist_height ) continue;

		// fill the background color
		for(j=tmp-1; j>=0; j--)
		{
			cvSetReal2D( tmp_hist_image1, j, i, 192.0 );	
		}
	}

	// resize to the image with hist_height x hist_width
	cvResize( tmp_hist_image1, tmp_hist_image2, CV_INTER_CUBIC );

	// copy the histogram data
	for(i=0; i<hist_height; i++)
	{
		for(j=0; j<hist_width; j++)
		{
			cvSetReal2D( hist_image, i, j, cvGetReal2D(tmp_hist_image2, i, j) );
		}
	}

	// attach the histogram bar
 	for(i=hist_height; i<hist_height+hist_bar_height; i++)
		for( j=0; j<hist_width; j++ )
			cvSetReal2D( hist_image, i, j, j );	

	if (hist) cvReleaseHist(&hist);
	if (tmp_hist_image1) cvReleaseImage(&tmp_hist_image1);
	if (tmp_hist_image2) cvReleaseImage(&tmp_hist_image2);
	
	return hist_image;
}

// Contrast & brightness adjustment (multiplication & addition)
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_basic_contrast_brightness( IplImage *src_image, double contrast, 
											    int brightness )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	//-------------------------------------
	// STEP 1 : Multiplication for contrast
	//-------------------------------------

	IplImage *tmp_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	cvSet( tmp_image, cvScalarAll(1), NULL );

	// dst(I)=contrast*src1(I)*src2(I)
	cvMul( src_image, tmp_image, dst_image, contrast );

	//-------------------------------
	// STEP 2 : Addition for brightness
	//-------------------------------

	CvScalar value = cvScalarAll( brightness );
	cvCopy( dst_image, tmp_image, NULL ); // copy
	cvZero( dst_image );                  // initialization by 0

	// addition
	cvAddS( tmp_image, value, dst_image, NULL );

	if (tmp_image) cvReleaseImage( &tmp_image );

	return dst_image; 
}

// Contrast adjustment (multiplication)
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_multiple_constant( IplImage *src_image, double scale )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );

	IplImage *tmp_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );
	cvSet( tmp_image, cvScalarAll(1), NULL );

	// dst(I)=scale*src1(I)*src2(I)
	cvMul( src_image, tmp_image, dst_image, scale );

	if (tmp_image) cvReleaseImage( &tmp_image );

	return dst_image; 
}

// Contrast adjustment (division)
// Input: 8 bit gray-scale image
IplImage *CPixel::GS_divide_constant( IplImage *src_image, double scale )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );
	
	IplImage *tmp_image = cvCreateImage( cvGetSize(src_image), 
										 IPL_DEPTH_8U, 
										 src_image->nChannels );
	cvSet( tmp_image, cvScalarAll(1), NULL );

	// dst(I)=(1/scale)*src1(I)*src2(I)
	// (ref) Here, cvDiv() cannot be used because, in cvDiv(),
	//        dst(I)=scale?src1(I)/src2(I), if src1!=NULL
	//        dst(I)=scale/src2(I),         if src1=NULL

	cvMul( src_image, tmp_image, dst_image, 1.0/scale );

	if (tmp_image) cvReleaseImage( &tmp_image );

	return dst_image; 
}

// Blending 
// Input: any two images
IplImage *CPixel::GS_blending_effect( IplImage *src_image1, IplImage *src_image2, double alpha )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image1), 
										 IPL_DEPTH_8U, 
										 src_image1->nChannels );

	double beta = 1.0 - alpha;
	double gamma = 0.0;

	cvAddWeighted( src_image1, alpha, 
				   src_image2, beta,
				   gamma, dst_image );
	
	return dst_image; 
}

// addition with two images
// Input: any two images
IplImage *CPixel::GS_add_image( IplImage *src_image1, IplImage *src_image2 )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image1), 
										 IPL_DEPTH_8U, 
										 src_image1->nChannels );

	cvAdd( src_image1, src_image2, dst_image, NULL ); 

	return dst_image;
}

// subtraction with two images
// Input: any two images
IplImage *CPixel::GS_subtract_image( IplImage *src_image1, IplImage *src_image2 )
{
	IplImage *dst_image = cvCreateImage( cvGetSize(src_image1), 
										 IPL_DEPTH_8U, 
										 src_image1->nChannels );

	cvSub( src_image1, src_image2, dst_image, NULL ); 

	return dst_image;
}

// Brightness adjustment (constant addition)
// Input: any image
IplImage *CPixel::GS_add_constant( IplImage *src_image, int constant )
{
	IplImage *dst_image = cvCloneImage( src_image );
	cvSetZero( dst_image );

	CvScalar value = cvScalarAll( fabs((double)constant) );
	
	if( constant >= 0 )
		cvAddS( src_image, value, dst_image, NULL );
	else
		cvSubS( src_image, value, dst_image, NULL );

	return dst_image;
}

// Make band image 
IplImage *CPixel::GS_makeGrayBand()
{
	int i, j;
	int height = 64;
	int width = 256;
	IplImage *band_image = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			cvSetReal2D( band_image, i, j, j );
		}
	}

	return band_image;
}

// Make contrast image
IplImage *CPixel::GS_makeContrast()
{
	int i, j;
	int height = 100;
	int width = 256;
	IplImage *contrast_image = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );

	for(i=0; i<height; i++)
	{
		for(j=0; j<width/2; j++)
		{
			cvSetReal2D( contrast_image, i, j, 0 );
		}

		for(j=width/2; j<width; j++)
		{
			cvSetReal2D( contrast_image, i, j, 255 );
		}
	}

	return contrast_image;
}
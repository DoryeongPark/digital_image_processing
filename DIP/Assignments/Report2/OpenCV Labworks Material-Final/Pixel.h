// Pixel.h: interface for the CPixel class.
//
//////////////////////////////////////////////////////////////////////

#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>

typedef unsigned char BYTE;

class CPixel  
{
public:
	CPixel();
	virtual ~CPixel();

	// 레벨별 명암 보기
	IplImage *GS_makeGrayBand();

	// 명암 대비 보기
	IplImage *GS_makeContrast();

	// 밝기 조절 - 덧셈/뺄셈
	IplImage *GS_add_constant( IplImage *src_image, int constant );

	// 두 영상간 덧셈
	IplImage *GS_add_image( IplImage *src_image1, IplImage *src_image2 );

	// 두 영상간 뺄셈
	IplImage *GS_subtract_image( IplImage *src_image1, IplImage *src_image2 );

	// 명암 대비 조절 - 곱셈
	IplImage *GS_multiple_constant( IplImage *src_image, double scale );

	// 명암 대비 조절 - 나눗셈
	IplImage *GS_divide_constant( IplImage *src_image, double scale );

	// 밝기 조절 + 명암 대비 조절
	IplImage *GS_basic_contrast_brightness( IplImage *src_image, double contrast, int brightness );

	// 선형 결합
	IplImage *GS_blending_effect( IplImage *src_image1, IplImage *src_image2, double alpha );

	// 명암도 영상의 히스토그램 생성.
	IplImage *GS_imhist( IplImage *src_image );
	IplImage *GS_imhist( IplImage *src_image, int max_length );

	// 히스토그램 평활화
	IplImage *GS_histeq( IplImage *src_image );

	// 히스토그램 데이터를 로그 창에 출력
	void GS_view_hist_data( IplImage *src_image );

	// 룩업 테이블을 이용한 밝기 조절 + 명암 대비 조절
	IplImage *GS_LUT_basic_contrast_brightness( IplImage *src_image, double contrast, int brightness );

	// 주어진 조건에 의한 이진 영상 변환
	IplImage *GS_threshold( IplImage *src_image, double threshold, int threshold_type );
	IplImage *GS_threshold( IplImage *src_image, double threshold, double max_value, int threshold_type );

	// 적응적 경계값 설정에 의한 이진 영상 변환
	IplImage *GS_adaptive_threshold( IplImage *src_image, double max_value, 
									 int adaptive_method, int threshold_type);

	IplImage *GS_adaptive_threshold( IplImage *src_image, double max_value, 
									 int adaptive_method, int threshold_type, 
									 int block_size, double param );

	// 이진 영상에서의 논리 연산 - 논리 연산(logical operator) 사용
	IplImage *GS_binary_logic( IplImage *src_image1, IplImage *src_image2, int logic_method  );

	// 이진 영상을 명암도 영상으로 변환
	IplImage *GS_binary2gray( IplImage *src_image );

	// 명암도 영상에서의 논리 연산 - 비트 연산자(bitwise operator) 사용
	IplImage *GS_gray_logic( IplImage *src_image1, IplImage *src_image2, int logic_method );
};


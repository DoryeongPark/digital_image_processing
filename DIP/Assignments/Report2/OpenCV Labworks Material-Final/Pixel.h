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

	// ������ ��� ����
	IplImage *GS_makeGrayBand();

	// ��� ��� ����
	IplImage *GS_makeContrast();

	// ��� ���� - ����/����
	IplImage *GS_add_constant( IplImage *src_image, int constant );

	// �� ���� ����
	IplImage *GS_add_image( IplImage *src_image1, IplImage *src_image2 );

	// �� ���� ����
	IplImage *GS_subtract_image( IplImage *src_image1, IplImage *src_image2 );

	// ��� ��� ���� - ����
	IplImage *GS_multiple_constant( IplImage *src_image, double scale );

	// ��� ��� ���� - ������
	IplImage *GS_divide_constant( IplImage *src_image, double scale );

	// ��� ���� + ��� ��� ����
	IplImage *GS_basic_contrast_brightness( IplImage *src_image, double contrast, int brightness );

	// ���� ����
	IplImage *GS_blending_effect( IplImage *src_image1, IplImage *src_image2, double alpha );

	// ��ϵ� ������ ������׷� ����.
	IplImage *GS_imhist( IplImage *src_image );
	IplImage *GS_imhist( IplImage *src_image, int max_length );

	// ������׷� ��Ȱȭ
	IplImage *GS_histeq( IplImage *src_image );

	// ������׷� �����͸� �α� â�� ���
	void GS_view_hist_data( IplImage *src_image );

	// ��� ���̺��� �̿��� ��� ���� + ��� ��� ����
	IplImage *GS_LUT_basic_contrast_brightness( IplImage *src_image, double contrast, int brightness );

	// �־��� ���ǿ� ���� ���� ���� ��ȯ
	IplImage *GS_threshold( IplImage *src_image, double threshold, int threshold_type );
	IplImage *GS_threshold( IplImage *src_image, double threshold, double max_value, int threshold_type );

	// ������ ��谪 ������ ���� ���� ���� ��ȯ
	IplImage *GS_adaptive_threshold( IplImage *src_image, double max_value, 
									 int adaptive_method, int threshold_type);

	IplImage *GS_adaptive_threshold( IplImage *src_image, double max_value, 
									 int adaptive_method, int threshold_type, 
									 int block_size, double param );

	// ���� ���󿡼��� �� ���� - �� ����(logical operator) ���
	IplImage *GS_binary_logic( IplImage *src_image1, IplImage *src_image2, int logic_method  );

	// ���� ������ ��ϵ� �������� ��ȯ
	IplImage *GS_binary2gray( IplImage *src_image );

	// ��ϵ� ���󿡼��� �� ���� - ��Ʈ ������(bitwise operator) ���
	IplImage *GS_gray_logic( IplImage *src_image1, IplImage *src_image2, int logic_method );
};


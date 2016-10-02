/*
*	ReportCode2-2.cpp
*	�ۼ��� : �ڵ���
*	�й�	  : 2010112406
*	����	  : �� �̹����� �ε��Ͽ� ������ ��� �̹����� ������â ���·� ����Ѵ�.
*/
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

int main(int argc, char* argv[])
{
	char* imgFileName1 = "image.jpg"; //�̹���1�� ���� �̸�
	char *imgFileName2 = "processed.jpg"; //�̹���2�� ���� �̸�
	char *winForBlended = "Blended Image";//�̹���1���� �̹���2�� �� ����� ������ �̸�

	IplImage *pImgIpl1 = nullptr; //�̹���1
	IplImage *pImgIpl2 = nullptr; //�̹���2
	IplImage *pImgBlended = nullptr; //��� �̹���

	pImgIpl1 = cvLoadImage(imgFileName1);//�̹���1 �ε�
	pImgIpl2 = cvLoadImage(imgFileName2);//�̹���2 �ε�

	if (!pImgIpl1 || !pImgIpl2)//���� �� �̹����� �ϳ��� �ε尡 �ȵǾ��� ���
		printf("Can't load Image");//���� ���
	else//���������� �ε� �Ǿ��� ���
	{
		//��� �̹����� ���� ������ ����
		cvNamedWindow(winForBlended, CV_WINDOW_AUTOSIZE);
		//������� ���� �̹����� �Ȱ��� ������� �����Ѵ�.
		pImgBlended = cvCreateImage(cvGetSize(pImgIpl2), 8, 3);
		//�̹���1 - �̹���2 = ��� �̹���(�̹���1�� �̹���2�� �����Ͽ� �E���� ��� pImgBlended�� ����)
		cvSub(pImgIpl1, pImgIpl2, pImgBlended, NULL);
		//�����ߴ� ������â�� �̿��Ͽ� ��� �̹����� ȭ�鿡 ����Ѵ�.
		cvShowImage(winForBlended, pImgBlended);

		// Waiting keyboard input
		cvWaitKey(0);

		// Release the image
		cvReleaseImage(&pImgIpl1);
		cvReleaseImage(&pImgIpl2);
		cvReleaseImage(&pImgBlended);

		// Destroy the window
		cvDestroyWindow(winForBlended);
	}
}
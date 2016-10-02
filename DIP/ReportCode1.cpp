
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

int main(int argc, char* argv[])
{
	char *imgFileName = "image.jpg";
	char *winNameIn = "original image";
	char *winNameOut = "processed image";
	IplImage *pImgIpl = 0, *pImgIplOut;

	// Create an image
	pImgIpl = cvLoadImage(imgFileName);

	// Copy the image data
	pImgIplOut = cvCloneImage(pImgIpl);

	if (!pImgIpl)
		printf("can't load image: %s\n", imgFileName);
	else
	{
		// Create a window
		cvNamedWindow(winNameIn, CV_WINDOW_AUTOSIZE);
		cvNamedWindow(winNameOut, CV_WINDOW_AUTOSIZE);

		// Move Window
		cvMoveWindow(winNameIn, 50, 50);
		cvMoveWindow(winNameOut, 235, 50);

		// Flip the image
		cvFlip(pImgIpl, pImgIplOut, 1);

		// Show the image
		cvShowImage(winNameIn, pImgIpl);
		cvShowImage(winNameOut, pImgIplOut);

		// Waiting keyboard input
		cvWaitKey(0);

		// Store the image
		cvSaveImage("processed.jpg", pImgIplOut);

		// Release the image
		cvReleaseImage(&pImgIpl);
		cvReleaseImage(&pImgIplOut);

		// Destroy the window
		cvDestroyWindow(winNameIn);
		cvDestroyWindow(winNameOut);
	}
}
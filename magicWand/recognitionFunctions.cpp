#include "StdAfx.h"
#include "recognitionFunctions.h"




CvPoint CenterofMass(IplImage* image){
	int totalX = 0;
	int totalY = 0;
	int countTotal = 0;
	
	for( int y=0; y<image->height; y++ ) {
				
		uchar* ptrImage = (uchar*) (
			image->imageData + y * image->widthStep);

		for( int x=0; x < image->width; x++ ) {
			 if (ptrImage[x] > 0){
				countTotal++;
				totalX = totalX + x;
				totalY = totalY + y;
			 }
					 
		}
	}
	
	if( countTotal < 100)
		return cvPoint(0,0);
	else
		return cvPoint(totalX / countTotal, totalY / countTotal);
}


IplImage* GreenFilter (IplImage* image, int greenFlag){
	IplImage* blue  = cvCreateImage( cvGetSize(image), 8, 1 );
	IplImage* green  = cvCreateImage( cvGetSize(image), 8, 1 );
	IplImage* red  = cvCreateImage( cvGetSize(image), 8, 1 );
	cvCvtPixToPlane( image, blue, green, red, 0 );


			for( int y=0; y<green->height; y++ ) {
				uchar* ptrGreen = (uchar*) (
				green->imageData + y * green->widthStep);

				uchar* ptrBlue = (uchar*) (
				blue->imageData + y * blue->widthStep);

				uchar* ptrRed = (uchar*) (
				red->imageData + y * red->widthStep);

				for( int x=0; x < green->width; x++ ) {
					 if (ptrGreen[x] > ptrBlue[x] && ptrGreen[x] > ptrRed[x] && ptrBlue[x] < 220 && ptrRed[x] < 220){
						if(greenFlag == 1)
							ptrGreen[x] = 255;
						

					 }
					 else ptrGreen[x] = 0;
				}
			}

			cvReleaseImage(&blue);
			cvReleaseImage(&red);

			return green;
}

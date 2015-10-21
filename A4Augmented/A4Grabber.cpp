#include "stdafx.h"
#include "A4BasicGrabber.h"


A4BasicGrabber::A4BasicGrabber()
{
	dst = cvCreateImage(cvSize(1748/2, 1240/2), 8, 3);
	transformMat = cvCreateMat(3, 3, CV_32FC1);
	corners[0] = cvPoint2D32f(0, 0);
	corners[1] = cvPoint2D32f(dst->width-1, 0);
	corners[2] = cvPoint2D32f(0, dst->height-1);
	corners[3] = cvPoint2D32f(dst->width-1, dst->height-1);
}


A4BasicGrabber::~A4BasicGrabber()
{
	delete transformMat;
}


void A4BasicGrabber::grab(IplImage* src, A4PreciseDetectedRecord dstRecord) 
{
	CvPoint2D32f srcCorners[4];
	srcCorners[0] = cvPoint2D32f(dstRecord.UL.x, dstRecord.UL.y);
	srcCorners[1] = cvPoint2D32f(dstRecord.UR.x, dstRecord.UR.y);
	srcCorners[2] = cvPoint2D32f(dstRecord.DL.x, dstRecord.DL.y);
	srcCorners[3] = cvPoint2D32f(dstRecord.DR.x, dstRecord.DR.y);
	transformMat = cvGetPerspectiveTransform(srcCorners, corners, transformMat);
	cvWarpPerspective(src, dst, transformMat, CV_INTER_LINEAR);
};


void A4BasicGrabber::dump()
{
    cvShowImage("A4TextGrabber", dst);
	//char c = cvWaitKey(100000);
}



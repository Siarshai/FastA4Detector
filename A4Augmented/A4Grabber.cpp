#include "stdafx.h"
#include "A4Grabber.h"


A4TextGrabber::A4TextGrabber()
{
	dst = cvCreateImage(cvSize(1748/2, 1240/2), 8, 3);
	transformMat = cvCreateMat(3, 3, CV_32FC1);
	corners[0] = cvPoint2D32f(0, 0);
	corners[1] = cvPoint2D32f(dst->width-1, 0);
	corners[2] = cvPoint2D32f(0, dst->height-1);
	corners[3] = cvPoint2D32f(dst->width-1, dst->height-1);
}


A4TextGrabber::~A4TextGrabber()
{
	delete transformMat;
}


void A4TextGrabber::addProcessor(IImageProcessor *iip)
{
	imageProcessors.push_back(std::unique_ptr<IImageProcessor>(iip));
}


void A4TextGrabber::grab(IplImage* src, A4PreciseDetectedRecord dstRecord) 
{
	CvPoint2D32f srcCorners[4];
	srcCorners[0] = cvPoint2D32f(dstRecord.UL.x, dstRecord.UL.y);
	srcCorners[1] = cvPoint2D32f(dstRecord.UR.x, dstRecord.UR.y);
	srcCorners[2] = cvPoint2D32f(dstRecord.DL.x, dstRecord.DL.y);
	srcCorners[3] = cvPoint2D32f(dstRecord.DR.x, dstRecord.DR.y);
	transformMat = cvGetPerspectiveTransform(srcCorners, corners, transformMat);
	cvWarpPerspective(src, dst, transformMat, CV_INTER_LINEAR);
	process();
};


void A4TextGrabber::dump()
{
    cvShowImage("A4TextGrabber", dst);
	//char c = cvWaitKey(100000);
}


void A4TextGrabber::process()
{
	for(auto it = imageProcessors.begin(); it != imageProcessors.end(); ++it)
		(*it)->process(dst);
}



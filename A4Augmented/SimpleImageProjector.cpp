#include "stdafx.h"
#include "SimpleImageProjector.h"


SimpleImageProjector::SimpleImageProjector(char* pathToProjection)
{
	projection = cvLoadImage(pathToProjection);
	if (projection == nullptr) {
		throw std::exception("Can not load projection image");
	}
	transformMat = cvCreateMat(3, 3, CV_32FC1);
	corners[0] = cvPoint2D32f(0, 0);
	corners[1] = cvPoint2D32f(projection->width-1, 0);
	corners[2] = cvPoint2D32f(0, projection->height-1);
	corners[3] = cvPoint2D32f(projection->width-1, projection->height-1);
}


SimpleImageProjector::~SimpleImageProjector()
{
	delete transformMat;
	cvReleaseImage(&projection);
}


void SimpleImageProjector::project(IplImage* dst, A4PreciseDetectedRecord dstRecord)
{
	CvPoint2D32f dstCorners[4];
	dstCorners[0] = cvPoint2D32f(dstRecord.UL.x, dstRecord.UL.y);
	dstCorners[1] = cvPoint2D32f(dstRecord.UR.x, dstRecord.UR.y);
	dstCorners[2] = cvPoint2D32f(dstRecord.DL.x, dstRecord.DL.y);
	dstCorners[3] = cvPoint2D32f(dstRecord.DR.x, dstRecord.DR.y);
	cvWarpPerspective(projection, dst, transformMat, CV_INTER_LINEAR);
}

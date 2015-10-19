#ifndef A4PROJECTOR
#define A4PROJECTOR

#include "Utils.h"

class A4Projector {
    IplImage* projection;
	CvMat* transformMat;
	CvPoint2D32f corners[4]; //UL, UR, DL, DR;
public:
	A4Projector(char* pathToProjection)
	{
		projection = cvLoadImage(pathToProjection);
		if (projection == NULL) {
			throw std::exception("Can not load projection image");
		}
		transformMat = cvCreateMat(3, 3, CV_32FC1);
		corners[0] = cvPoint2D32f(0, 0);
		corners[1] = cvPoint2D32f(0, projection->height-1);
		corners[2] = cvPoint2D32f(projection->width-1, 0);
		corners[3] = cvPoint2D32f(projection->width-1, projection->height-1);
	}
	~A4Projector()
	{
		delete transformMat;
	}
	void findTransform(CvPoint2D32f* dst)
	{
	    transformMat = cvGetPerspectiveTransform(corners, dst, transformMat);
	}
	void project(IplImage* dst)
	{
	    cvWarpPerspective(projection, dst, transformMat);
	}
};

#endif
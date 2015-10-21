#ifndef A4BASICGRABBER
#define A4BASICGRABBER

#include "IGrabber.h"


class A4BasicGrabber : public IGrabber
{
	CvMat* transformMat;
	CvPoint2D32f corners[4]; //UL, UR, DL, DR;
public:
	IplImage* dst;
	A4BasicGrabber();
	~A4BasicGrabber();
	void grab(IplImage* src, A4PreciseDetectedRecord dstRecord);
	void dump();
};


#endif //A4BASICGRABBER
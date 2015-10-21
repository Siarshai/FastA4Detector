#ifndef SIMPLEIMAGEPROJECTOR
#define SIMPLEIMAGEPROJECTOR

#include "IProjector.h"


class SimpleImageProjector : public IProjector {
    IplImage* projection;
	CvMat* transformMat;
	CvPoint2D32f corners[4]; //UL, UR, DL, DR;
public:
	SimpleImageProjector(char* pathToProjection);
	~SimpleImageProjector();
	void project(IplImage* dst, A4PreciseDetectedRecord dstRecord);
};


#endif //SIMPLEIMAGEPROJECTOR
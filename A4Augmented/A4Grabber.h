#ifndef A4GRABBER
#define A4GRABBER

#include "Utils.h"
#include "A4InfoStructures.h"
#include <memory>

class IGrabber
{
public:
	virtual void grab(IplImage* src, A4PreciseDetectedRecord dstRecord) = 0;
};

class IImageProcessor
{
public:
	virtual void process(IplImage* src) = 0;
};

class A4TextGrabber : public IGrabber
{
	IplImage* dst;
	CvMat* transformMat;
	CvPoint2D32f corners[4]; //UL, UR, DL, DR;
	std::list<std::unique_ptr<IImageProcessor>> imageProcessors;
	void process();
public:
	A4TextGrabber();
	~A4TextGrabber();
	void addProcessor(IImageProcessor *iip);
	void grab(IplImage* src, A4PreciseDetectedRecord dstRecord);
	void dump();
};


#endif //A4GRABBER
#ifndef A4GRABBER
#define A4GRABBER

#include "Utils.h"
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
	void process()
	{
		for(auto it = imageProcessors.begin(); it != imageProcessors.end(); ++it)
			(*it)->process(dst);
	}
public:
	A4TextGrabber()
	{
		dst = cvCreateImage(cvSize(1748/2, 1240/2), 8, 3);
		transformMat = cvCreateMat(3, 3, CV_32FC1);
		corners[0] = cvPoint2D32f(0, 0);
		corners[1] = cvPoint2D32f(dst->width-1, 0);
		corners[2] = cvPoint2D32f(0, dst->height-1);
		corners[3] = cvPoint2D32f(dst->width-1, dst->height-1);
	}
	~A4TextGrabber()
	{
		delete transformMat;
	}
	void addProcessor(IImageProcessor *iip)
	{
		imageProcessors.push_back(std::unique_ptr<IImageProcessor>(iip));
	}
	void grab(IplImage* src, A4PreciseDetectedRecord dstRecord) 
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
	void dump()
	{
        cvShowImage("A4TextGrabber", dst);
		//char c = cvWaitKey(100000);
	}
};


#endif //A4GRABBER
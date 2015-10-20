#ifndef A4MEMORYBANK
#define A4MEMORYBANK

#include "Utils.h"

//Keeps all the subimages requiered to process raw image 
class A4MemoryBank
{
public:
	int resizeFactor;

	CvSize mainSize;
	CvSize sizeII;
	CvSize sizeFactored;
	CvSize sizeFactoredII;

    IplImage *redChannel, *greenChannel, *blueChannel; 
    IplImage *redChannelResized, *greenChannelResized, *blueChannelResized;

	IplImage *uBorders, *dBorders, *lBorders, *rBorders;
	IplImage *uBordersII, *dBordersII, *lBordersII, *rBordersII;
	
	IplImage *uBordersFactored, *dBordersFactored, *lBordersFactored, *rBordersFactored;
	IplImage *uBordersIIFactored, *dBordersIIFactored, *lBordersIIFactored, *rBordersIIFactored;

	IplImage *buffer;
	IplImage *bufferFactored;

	A4MemoryBank();
	void initMemory(CvSize size, int resizeFactor);
	void clearMemory();
	void consumeImage(IplImage *aimage, int resizeFactor = 4);
};

#endif //A4MEMORYBANK
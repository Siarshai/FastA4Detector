#include "stdafx.h"
#include "A4MemoryBank.h"


A4MemoryBank::A4MemoryBank()
{
}


void A4MemoryBank::initMemory(CvSize size, int aresizeFactor = 4)
{
	clearMemory();

	resizeFactor = aresizeFactor;
	mainSize = size;
	sizeFactored = cvSize(size.width/resizeFactor, size.height/resizeFactor);
	sizeII = cvSize(size.width + 1, size.height + 1);
	sizeFactoredII = cvSize(sizeFactored.width + 1, sizeFactored.height + 1);

    redChannel = cvCreateImage(mainSize, IPL_DEPTH_8U, 1); 
    greenChannel = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
    blueChannel = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);

    redChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    greenChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    blueChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);

	uBorders = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
	dBorders = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
	lBorders = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
	rBorders = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
	
	uBordersII = cvCreateImage(sizeII, IPL_DEPTH_32S, 1);
	dBordersII = cvCreateImage(sizeII, IPL_DEPTH_32S, 1);
	lBordersII = cvCreateImage(sizeII, IPL_DEPTH_32S, 1);
	rBordersII = cvCreateImage(sizeII, IPL_DEPTH_32S, 1);

	uBordersFactored = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
	dBordersFactored = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
	lBordersFactored = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
	rBordersFactored = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
	
	uBordersIIFactored = cvCreateImage(sizeFactoredII, IPL_DEPTH_32S, 1);
	dBordersIIFactored = cvCreateImage(sizeFactoredII, IPL_DEPTH_32S, 1);
	lBordersIIFactored = cvCreateImage(sizeFactoredII, IPL_DEPTH_32S, 1);
	rBordersIIFactored = cvCreateImage(sizeFactoredII, IPL_DEPTH_32S, 1);
	
    buffer = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
    bufferFactored = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
}


void A4MemoryBank::clearMemory()
{
	if(redChannel != nullptr)
		cvReleaseImage(&redChannel);
	if(greenChannel != nullptr)
		cvReleaseImage(&greenChannel);
	if(blueChannel != nullptr)
		cvReleaseImage(&blueChannel);

	if(redChannelResized != nullptr)
		cvReleaseImage(&redChannelResized);
	if(greenChannelResized != nullptr)
		cvReleaseImage(&greenChannelResized);
	if(blueChannelResized != nullptr)
		cvReleaseImage(&blueChannelResized);
	
	if(uBorders != nullptr)
		cvReleaseImage(&uBorders);
	if(dBorders != nullptr)
		cvReleaseImage(&dBorders);
	if(lBorders != nullptr)
		cvReleaseImage(&lBorders);
	if(rBorders != nullptr)
		cvReleaseImage(&rBorders);
	
	if(uBordersII != nullptr)
		cvReleaseImage(&uBorders);
	if(dBordersII != nullptr)
		cvReleaseImage(&dBorders);
	if(lBordersII != nullptr)
		cvReleaseImage(&lBorders);
	if(rBordersII != nullptr)
		cvReleaseImage(&rBorders);
	
	if(uBordersFactored != nullptr)
		cvReleaseImage(&uBordersFactored);
	if(dBordersFactored != nullptr)
		cvReleaseImage(&dBordersFactored);
	if(lBordersFactored != nullptr)
		cvReleaseImage(&lBordersFactored);
	if(rBordersFactored != nullptr)
		cvReleaseImage(&rBordersFactored);
	
	if(uBordersIIFactored != nullptr)
		cvReleaseImage(&uBordersFactored);
	if(dBordersIIFactored != nullptr)
		cvReleaseImage(&dBordersFactored);
	if(lBordersIIFactored != nullptr)
		cvReleaseImage(&lBordersFactored);
	if(rBordersIIFactored != nullptr)
		cvReleaseImage(&rBordersFactored);

	if(bufferFactored != nullptr)
		cvReleaseImage(&bufferFactored);
}


void A4MemoryBank::consumeImage(IplImage *aimage, int resizeFactor)
{
	CvSize currentSize = cvGetSize(aimage);
	currentSize.width -= currentSize.width%resizeFactor;
	currentSize.height -= currentSize.height%resizeFactor;

	if(currentSize.height != mainSize.height || currentSize.width != mainSize.width)
		initMemory(currentSize, resizeFactor); 

	//TODO: make this part more efficient
	if(aimage->width%resizeFactor != 0 || aimage->height%resizeFactor != 0) 
	{
		printf("DEBUG: Image formated to be divisable by %d\n", resizeFactor);
		IplImage *tmpRed = cvCreateImage(cvSize(aimage->width, aimage->height), IPL_DEPTH_8U, 1);
		IplImage *tmpGreen = cvCreateImage(cvSize(aimage->width, aimage->height), IPL_DEPTH_8U, 1);
		IplImage *tmpBlue = cvCreateImage(cvSize(aimage->width, aimage->height), IPL_DEPTH_8U, 1);
		cvSplit(aimage, tmpRed, tmpGreen, tmpBlue, NULL);
		cvResize(tmpRed, redChannel);
		cvResize(tmpGreen, greenChannel);
		cvResize(tmpBlue, blueChannel);
		cvReleaseImage(&tmpRed);
		cvReleaseImage(&tmpGreen);
		cvReleaseImage(&tmpBlue);
	} 
	else
	{
		cvSplit(aimage, redChannel, greenChannel, blueChannel, NULL); //FIXME: aimage -> image (rgb)
	}
	cvResize(redChannel, redChannelResized);
	cvResize(greenChannel, greenChannelResized);
	cvResize(blueChannel, blueChannelResized);

	cvSmooth(redChannelResized, redChannelResized, CV_MEDIAN, 3);
	cvSmooth(greenChannelResized, greenChannelResized, CV_MEDIAN, 3);
	cvSmooth(blueChannelResized, blueChannelResized, CV_MEDIAN, 3);
	
}


#ifndef A4MATCHER_H
#define A4MATCHER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <array>
#include <unordered_map>

#include "LineClusterifier.h"
#include "PointClusterifier.h"
#include "BorderAnalyzer.h"
#include "LocalHoughTransformer.h"
#include "A4InfoStructures.h"


class A4Matcher
{
	int resizeFactor;

	CvSize mainSize;
	CvSize sizeII;
	CvSize sizeFactored;
	CvSize sizeFactoredII;

	IplImage *image, *imageResized;

	//Temporary memory banks needed for image processing
    IplImage *redChannel, *greenChannel, *blueChannel; //TODO: Theese may be optimized
    IplImage *redChannelResized, *greenChannelResized, *blueChannelResized;

	IplImage *uBorders, *dBorders, *lBorders, *rBorders;
	IplImage *uBordersII, *dBordersII, *lBordersII, *rBordersII;
	
	IplImage *uBordersFactored, *dBordersFactored, *lBordersFactored, *rBordersFactored;
	IplImage *uBordersIIFactored, *dBordersIIFactored, *lBordersIIFactored, *rBordersIIFactored;

	IplImage *buffer;
	IplImage *bufferFactored;
	
	LocalHoughTransformer localHoughTransformer;
	
	void formatImage(IplImage *aimage);
	void channelSplit(IplImage *aimage);
	void prepareDerivativesSearchTemplates();
	void prepareDerivativesSearchTemplatesBase(IplImage *rc, IplImage *gc, IplImage *bc, 
													  IplImage *ubord, IplImage *dbord, IplImage *lbord, IplImage *rbord, 
													  IplImage *ubordII, IplImage *dbordII, IplImage *lbordII, IplImage *rbordII, 
													  IplImage *buff, int numberOfAnalyzers);
	void applyA4SearchMask();
	void normalizePoints();
	void findPreciseBorderAlignedLines();
	CvPoint findPreciseBorderAlignedLinesFindLineSubroutine(unsigned char *dataOrigin, int step, int xOffset, int yOffset);
	void clearMemory();

public:

	std::list<A4PreDetectedRecord> A4PreDetected;
	std::list<A4PreciseDetectedRecord> A4PreciseDetected;

	void analyseImage();
	void initMemory(CvSize size);
	void setAndAnalyseImage(IplImage *aimage);
	void clearResults();
	
	A4Matcher();
	~A4Matcher(void);
};


#endif
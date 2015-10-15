#include "stdafx.h"
#include <cmath>
#include "A4Matcher.h"



A4Matcher::A4Matcher() : mainSize(cvSize(-1, -1)), resizeFactor(4) //TODO: replace with constants //lineClusterifier(0.3f, 20.0f), pointClusterifier(10.0f), 
{

}

void A4Matcher::initMemory(CvSize size)
{
	clearMemory();

	mainSize = size;
	sizeFactored = cvSize(size.width/resizeFactor, size.height/resizeFactor);
	sizeII = cvSize(size.width + 1, size.height + 1);
	sizeFactoredII = cvSize(sizeFactored.width + 1, sizeFactored.height + 1);

	//Resetting memory banks
	//Main image
    image = cvCreateImage(mainSize, IPL_DEPTH_8U, 1);
    imageResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1); //make lesser?
	
    redChannel = cvCreateImage(mainSize, IPL_DEPTH_8U, 1); //TODO: get rid of them
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

void A4Matcher::clearMemory()
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


void A4Matcher::formatImage(IplImage *aimage)
{
	CvSize currentSize = cvGetSize(aimage);
	currentSize.width -= currentSize.width%resizeFactor;
	currentSize.height -= currentSize.height%resizeFactor;

	if( (currentSize.width != mainSize.width)||(currentSize.height != mainSize.height) )
		initMemory(currentSize);

	if(aimage->width%resizeFactor != 0 || aimage->height%resizeFactor != 0) 
	{
		printf("DEBUG: Image formated to be divisable by %d\n", resizeFactor);
		IplImage *tmp = cvCreateImage(cvSize(aimage->width, aimage->height), IPL_DEPTH_8U, 1);
		cvCvtColor(aimage, tmp, CV_RGB2GRAY);
		cvResize(tmp, image);
		cvReleaseImage(&tmp);
	} 
	else
	{
		cvCvtColor(aimage, image, CV_RGB2GRAY);
	}
}


void A4Matcher::channelSplit(IplImage *aimage)
{
	cvSplit(aimage, redChannel, greenChannel, blueChannel, NULL);
	
	cvResize(redChannel, redChannelResized);
	cvResize(greenChannel, greenChannelResized);
	cvResize(blueChannel, blueChannelResized);

	cvSmooth(redChannelResized, redChannelResized, CV_MEDIAN, 3);
	cvSmooth(greenChannelResized, greenChannelResized, CV_MEDIAN, 3);
	cvSmooth(blueChannelResized, blueChannelResized, CV_MEDIAN, 3);
}

void A4Matcher::prepareDerivativesSearchTemplatesBase(IplImage *rc, IplImage *gc, IplImage *bc, 
													  IplImage *ubord, IplImage *dbord, IplImage *lbord, IplImage *rbord, 
													  IplImage *ubordII, IplImage *dbordII, IplImage *lbordII, IplImage *rbordII, 
													  IplImage *buff, int numberOfAnalyzers)
{
	const int secondDerivativeThreshold = 180;
	const int firstDerivativeDiagThreshold = 40;
	const int firstDerivativeXYThreshold = 180;

    uchar *dataRed = (uchar *)rc->imageData;
    uchar *dataGreen = (uchar *)gc->imageData;
    uchar *dataBlue = (uchar *)bc->imageData;

    uchar *dataUBorders = (uchar *)ubord->imageData;
    uchar *dataDBorders = (uchar *)dbord->imageData;
    uchar *dataLBorders = (uchar *)lbord->imageData;
    uchar *dataRBorders = (uchar *)rbord->imageData;
    uchar *databufferFactored = (uchar *)buff->imageData;
	
	int width = rc->width;
	int height = rc->height;
    int stepU8 = rc->widthStep;
	int tmpU8;

	//Calculating borders with state-keeptin border analyzer
	//vector<BorderAnalyzer> ba(numberOfAnalyzers, BorderAnalyzer(numberOfAnalyzers/4));
	//BorderAnalyzer ba[numberOfAnalyzers];

	//Zerofying. May be optimized.
	for(int j = 0; j < width; ++j) 
	{
		for(int i = 0; i < height; ++i) 
		{
			tmpU8 = i*stepU8 + j;
			dataUBorders[tmpU8] = 0;
			dataDBorders[tmpU8] = 0;
			dataRBorders[tmpU8] = 0;
			dataLBorders[tmpU8] = 0;
			databufferFactored[tmpU8] = 0;
		}
	}

	ChoirOfBorderAnalyzers cba(numberOfAnalyzers, numberOfAnalyzers/4, numberOfAnalyzers*3/4);
	
    for (int j = numberOfAnalyzers/2; j < width-numberOfAnalyzers/2; ++j) {
		for (int i = height - 1 - numberOfAnalyzers/2; i >= numberOfAnalyzers/2; --i) 
			cba.response(i*stepU8 + j, 1, dataRed, dataGreen, dataBlue, dataUBorders);
		cba.invalidate();
		for (int i = numberOfAnalyzers/2; i < height - numberOfAnalyzers/2; ++i) 
			cba.response(i*stepU8 + j, 1, dataRed, dataGreen, dataBlue, dataDBorders);
		cba.invalidate();
	}

	for (int i = numberOfAnalyzers/2; i < height - numberOfAnalyzers/2; ++i) 
	{
		for (int j = numberOfAnalyzers/2; j < width - numberOfAnalyzers/2; ++j) 
			cba.response(i*stepU8 + j, stepU8, dataRed, dataGreen, dataBlue, dataRBorders);
		cba.invalidate();
		for (int j = width - 1 - numberOfAnalyzers/2; j >= numberOfAnalyzers/2; --j) 
			cba.response(i*stepU8 + j, stepU8, dataRed, dataGreen, dataBlue, dataLBorders);
		cba.invalidate();
	}
	
	cvIntegral(ubord, ubordII);
	cvIntegral(dbord, dbordII);
	cvIntegral(lbord, lbordII);
	cvIntegral(rbord, rbordII);
}


void A4Matcher::prepareDerivativesSearchTemplates()
{	
	const int numberOfAnalyzers = 44; 
	const int numberOfAnalyzersFactored = numberOfAnalyzers/resizeFactor; 

	prepareDerivativesSearchTemplatesBase(redChannelResized, greenChannelResized, blueChannelResized, 
													  uBordersFactored, dBordersFactored, lBordersFactored, rBordersFactored, 
													  uBordersIIFactored, dBordersIIFactored, lBordersIIFactored, rBordersIIFactored, 
													  bufferFactored, numberOfAnalyzersFactored);

	prepareDerivativesSearchTemplatesBase(redChannel, greenChannel, blueChannel, 
													  uBorders, dBorders, lBorders, rBorders, 
													  uBordersII, dBordersII, lBordersII, rBordersII, 
													  buffer, numberOfAnalyzers);
	
	cvShowImage("ubord", uBorders);
	cvShowImage("dbord", dBorders);
	cvShowImage("lbord", lBorders);
	cvShowImage("rbord", rBorders);
	cvShowImage("uBordersFactored", uBordersFactored);
	cvShowImage("dBordersFactored", dBordersFactored);
	cvShowImage("lBordersFactored", lBordersFactored);
	cvShowImage("rBordersFactored", rBordersFactored);
	//char c = cvWaitKey(100000);
}

void A4Matcher::setAndAnalyseImage(IplImage *aimage)
{
	clearResults();
	formatImage(aimage);
	cvResize(image, imageResized);
	channelSplit(aimage);
	prepareDerivativesSearchTemplates();
	applyA4SearchMask();
	normalizePoints();
	findPreciseBorderAlignedLines();
}


void A4Matcher::applyA4SearchMask() 
{
    unsigned char *dataUBorders = (unsigned char *)uBordersFactored->imageData;
    unsigned char *dataDBorders = (unsigned char *)dBordersFactored->imageData;
    unsigned char *dataLBorders = (unsigned char *)lBordersFactored->imageData;
    unsigned char *dataRBorders = (unsigned char *)rBordersFactored->imageData;
    int *datauBordersIIFactored = (int *)uBordersIIFactored->imageData;
    int *datadBordersIIFactored = (int *)dBordersIIFactored->imageData;
    int *datalBordersIIFactored = (int *)lBordersIIFactored->imageData;
    int *datarBordersIIFactored = (int *)rBordersIIFactored->imageData;
    unsigned char *databufferFactored = (unsigned char *)bufferFactored->imageData;
	int step = uBordersIIFactored->widthStep/sizeof(int);
	int stepbufferFactored = bufferFactored->widthStep/sizeof(char);

	int searchWidth;
	int searchHeight;
	const float maxSizeDivisor = 1.0f/2.0f; //2.0f/3.0f; // <<<======================
	const int kernelResolution = 1;
	if(sizeFactored.width > sizeFactored.height*99/70) {
		searchWidth = static_cast<int>( sizeFactored.width*maxSizeDivisor ); //TODO: make it constant
		searchHeight = searchWidth*70/99;
	} else {
		searchHeight = static_cast<int>( sizeFactored.height*maxSizeDivisor ); //TODO: make it constant
		searchWidth = searchHeight*99/70;
	}

	const int safetyMarginDivisor = 6;
	int safetyWidthMargin = max(searchHeight/safetyMarginDivisor, kernelResolution); //TODO: note they switched places
	int safetyHeightMargin = max(searchWidth/safetyMarginDivisor, kernelResolution);
	int widthBorderBlock = (searchWidth + 2*safetyWidthMargin)/4; //4 border blocks
	int heightBorderBlock = (searchHeight + 2*safetyHeightMargin)/4; //4 border blocks
	
	int debug = 2;
	const int minHeight = 20;

	while(searchHeight > minHeight) 
	{
		for(int i = 0; i < sizeFactored.height - searchHeight - 2*safetyHeightMargin; ++i) 
		{
			for(int j = 0; j < sizeFactored.width - searchWidth - 2*safetyWidthMargin; ++j)
			{
				if(databufferFactored[i*stepbufferFactored + j] == 0)
				{
					int whiteBodyBorders = pieceOfII(datauBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
					whiteBodyBorders += pieceOfII(datadBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
					whiteBodyBorders += pieceOfII(datarBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
					whiteBodyBorders += pieceOfII(datalBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
					if(whiteBodyBorders < 50*255) // <<<
					{ 
						if(horizontalBlockPassII(datauBordersIIFactored, j, i, widthBorderBlock, safetyHeightMargin, step))
						{
							if(verticalBlockPassII(datalBordersIIFactored, j, i, safetyWidthMargin, heightBorderBlock, step))
							{
								if( horizontalBlockPassII(datadBordersIIFactored, j, i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step) )
								{
									if( verticalBlockPassII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i, safetyWidthMargin, heightBorderBlock, step) )
									{
										int overlap = 0;
										for(int k = 0; k < searchHeight + 2*safetyHeightMargin; ++k)
										{
											for(int l = 0; l < searchWidth + 2*safetyWidthMargin; ++l) 
											{
												overlap += databufferFactored[(i + k)*stepbufferFactored + j + l];
											}
										}
										if(overlap < (searchWidth + 2*safetyWidthMargin)*(searchHeight + 2*safetyHeightMargin)*3/4) 
										{
											A4PreDetected.push_back(
												A4PreDetectedRecord(cvPoint(j + safetyWidthMargin, i + safetyHeightMargin), 
																						cvPoint(j + safetyWidthMargin + searchWidth, i + safetyHeightMargin + searchHeight),
																						cvPoint(j, i),
																						cvPoint(j + searchWidth + 2*safetyWidthMargin, i + searchHeight + 2*safetyHeightMargin),
																						widthBorderBlock, heightBorderBlock, safetyWidthMargin, safetyHeightMargin)
												);
											for(int k = 0; k < searchHeight + 2*safetyHeightMargin; ++k)
											{
												for(int l = 0; l < searchWidth + 2*safetyWidthMargin; ++l) 
												{
													databufferFactored[(i + k)*stepbufferFactored + j + l] = 1;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		searchHeight -= kernelResolution; //safetyHeightMargin;
		searchWidth = searchHeight*99/70;
		
		safetyWidthMargin = max(searchHeight/safetyMarginDivisor, kernelResolution); //TODO: note they switched places
		safetyHeightMargin = max(searchWidth/safetyMarginDivisor, kernelResolution);
		widthBorderBlock = (searchWidth + 2*safetyWidthMargin)/4; //4 border blocks
		heightBorderBlock = (searchHeight + 2*safetyHeightMargin)/4; //4 border blocks
	}
}


CvPoint A4Matcher::findPreciseBorderAlignedLinesFindLineSubroutine(unsigned char *dataOrigin, int step, int xOffset, int yOffset)
{
	localHoughTransformer.setPictureSpace(dataOrigin + yOffset*step + xOffset);
	CvPoint line;
	try {
		line = localHoughTransformer.analyze();
	} catch(...) { throw; }
	line.y += -xOffset*sin(line.x*M_PI/180.0) + yOffset*cos(line.x*M_PI/180.0);
	return line;
}

void A4Matcher::findPreciseBorderAlignedLines()
{
	for(A4PreDetectedRecord a4pd : A4PreDetected)
	{
		int x = a4pd.ulptBorder.x;
		int y = a4pd.ulptBorder.y;

		int widthBorderBlock = a4pd.widthBorderBlock*resizeFactor;
		int heightBorderBlock = a4pd.heightBorderBlock*resizeFactor;
		int safetyWidthMargin = a4pd.safetyWidthMargin*resizeFactor;
		int safetyHeightMargin = a4pd.safetyHeightMargin*resizeFactor;
		
		unsigned char *dataUBorders = (unsigned char *)uBorders->imageData;
		unsigned char *dataDBorders = (unsigned char *)dBorders->imageData;
		unsigned char *dataLBorders = (unsigned char *)lBorders->imageData;
		unsigned char *dataRBorders = (unsigned char *)rBorders->imageData;
		int step = uBorders->widthStep;
		
		CvPoint lineHorUL, lineHorUR, lineHorDL, lineHorDR;
		CvPoint lineVerUL, lineVerUR, lineVerDL, lineVerDR;
		localHoughTransformer.fullReset(-20, 20, 2*widthBorderBlock, 2*safetyHeightMargin, step, NULL);
		
		try {
			lineHorUL = findPreciseBorderAlignedLinesFindLineSubroutine(dataUBorders, step, x, y);
		} catch(...) {
			lineHorUL = cvPoint(0, -a4pd.ulpt.y);
		}

		try {
			lineHorUR = findPreciseBorderAlignedLinesFindLineSubroutine(dataUBorders, step, x + 2*widthBorderBlock, y);
		} catch(...) { 
			lineHorUR = cvPoint(0, -a4pd.ulpt.y); 
		}

		try {
			lineHorDL = findPreciseBorderAlignedLinesFindLineSubroutine(dataDBorders, step, x, y + 4*heightBorderBlock - 2*safetyHeightMargin);
		} catch(...) {
			lineHorDL = cvPoint(0, -y - 4*heightBorderBlock + safetyHeightMargin); 
		}

		try {
			lineHorDR = findPreciseBorderAlignedLinesFindLineSubroutine(dataDBorders, step, x + 2*widthBorderBlock, y + 4*heightBorderBlock - 2*safetyHeightMargin);
		} catch(...) {
			lineHorDR = cvPoint(0, -y - 4*heightBorderBlock + safetyHeightMargin); 
		}

		localHoughTransformer.fullReset(70, 110, 2*safetyWidthMargin, 2*heightBorderBlock, step, NULL);
		
		try {
			lineVerUL = findPreciseBorderAlignedLinesFindLineSubroutine(dataLBorders, step, x, y);
		} catch(...) {
			lineVerUL = cvPoint(90, -a4pd.ulpt.x);
		}
		try {
			lineVerDL = findPreciseBorderAlignedLinesFindLineSubroutine(dataLBorders, step, x, y + 2*heightBorderBlock);
		} catch(...) {
			lineVerDL = cvPoint(90, -a4pd.ulpt.x);
		}
		try {
			lineVerUR = findPreciseBorderAlignedLinesFindLineSubroutine(dataRBorders, step, x + 4*widthBorderBlock - 2*safetyWidthMargin, y);
		} catch(...) {
			lineVerUR = cvPoint(90, -x - 4*widthBorderBlock + safetyWidthMargin);
		}
		try {
			lineVerDR = findPreciseBorderAlignedLinesFindLineSubroutine(dataRBorders, step, x + 4*widthBorderBlock - 2*safetyWidthMargin, y + 2*heightBorderBlock);
		} catch(...) {
			lineVerDR = cvPoint(90, -x - 4*widthBorderBlock + safetyWidthMargin);
		}
		
		CvPoint cornerUL = lineIntersection(lineHorUL.x*M_PI/180, -lineHorUL.y, lineVerUL.x*M_PI/180, -lineVerUL.y);
		CvPoint cornerUR = lineIntersection(lineHorUR.x*M_PI/180, -lineHorUR.y, lineVerUR.x*M_PI/180, -lineVerUR.y);
		CvPoint cornerDL = lineIntersection(lineHorDL.x*M_PI/180, -lineHorDL.y, lineVerDL.x*M_PI/180, -lineVerDL.y);
		CvPoint cornerDR = lineIntersection(lineHorDR.x*M_PI/180, -lineHorDR.y, lineVerDR.x*M_PI/180, -lineVerDR.y);

		A4PreciseDetected.push_back( A4PreciseDetectedRecord(cornerUL, cornerUR, cornerDL, cornerDR
			#ifdef DEBUG_LINES_INFORMATION 
			,
									lineHorUL, lineHorUR, lineHorDL, lineHorDR, 
									lineVerUL, lineVerDL, lineVerUR, lineVerDR
			#endif DEBUG_LINES_INFORMATION 
									) );
	}
}

void A4Matcher::normalizePoints()
{
	for(std::list<A4PreDetectedRecord>::iterator it = A4PreDetected.begin(); it != A4PreDetected.end(); ++it) {
		(*it).ulpt.x = (*it).ulpt.x*resizeFactor + resizeFactor/2;
		(*it).ulpt.y = (*it).ulpt.y*resizeFactor + resizeFactor/2;
		(*it).drpt.x = (*it).drpt.x*resizeFactor + resizeFactor/2;
		(*it).drpt.y = (*it).drpt.y*resizeFactor + resizeFactor/2;
		(*it).ulptBorder.x = (*it).ulptBorder.x*resizeFactor + resizeFactor/2;
		(*it).ulptBorder.y = (*it).ulptBorder.y*resizeFactor + resizeFactor/2;
		(*it).drptBorder.x = (*it).drptBorder.x*resizeFactor + resizeFactor/2;
		(*it).drptBorder.y = (*it).drptBorder.y*resizeFactor + resizeFactor/2;
	}
}

void A4Matcher::clearResults()
{
	A4PreDetected.clear();
	A4PreciseDetected.clear();
}

A4Matcher::~A4Matcher(void)
{
	clearMemory();
}

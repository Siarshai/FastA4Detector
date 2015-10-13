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
    image = cvCreateImage(size, IPL_DEPTH_8U, 1);
    imageResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1); //make lesser?
	
    redChannel = cvCreateImage(size, IPL_DEPTH_8U, 1); //TODO: get rid of them
    greenChannel = cvCreateImage(size, IPL_DEPTH_8U, 1);
    blueChannel = cvCreateImage(size, IPL_DEPTH_8U, 1);

    redChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    greenChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    blueChannelResized = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);

    redChannelResizedTmp = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    greenChannelResizedTmp = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);
    blueChannelResizedTmp = cvCreateImage(sizeFactored, IPL_DEPTH_8U, 1);

	uBorders = cvCreateImage(size, IPL_DEPTH_8U, 1);
	dBorders = cvCreateImage(size, IPL_DEPTH_8U, 1);
	lBorders = cvCreateImage(size, IPL_DEPTH_8U, 1);
	rBorders = cvCreateImage(size, IPL_DEPTH_8U, 1);
	
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
	
    buffer = cvCreateImage(size, IPL_DEPTH_8U, 1);
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

	if(redChannelResizedTmp != nullptr)
		cvReleaseImage(&redChannelResizedTmp);
	if(greenChannelResizedTmp != nullptr)
		cvReleaseImage(&greenChannelResizedTmp);
	if(blueChannelResizedTmp != nullptr)
		cvReleaseImage(&blueChannelResizedTmp);
	
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
	
	cvResize(redChannel, redChannelResized); //cvPyrDown
	cvResize(greenChannel, greenChannelResized);
	cvResize(blueChannel, blueChannelResized);
	
	cvSmooth(redChannelResized, redChannelResizedTmp, CV_MEDIAN, 1);
	cvSmooth(greenChannelResized, greenChannelResizedTmp, CV_MEDIAN, 1);
	cvSmooth(blueChannelResized, blueChannelResizedTmp, CV_MEDIAN, 1);
	
	cvSmooth(redChannelResizedTmp, redChannelResized, CV_MEDIAN, 3);
	cvSmooth(greenChannelResizedTmp, greenChannelResized, CV_MEDIAN, 3);
	cvSmooth(blueChannelResizedTmp, blueChannelResized, CV_MEDIAN, 3);
	/*
	cvEqualizeHist( redChannelResizedTmp, redChannelResized );
	cvEqualizeHist( greenChannelResizedTmp, greenChannelResized );
	cvEqualizeHist( blueChannelResizedTmp, blueChannelResized );
	*/
}


void A4Matcher::applyColorInvalidator()
{
	//Manually filtering source image because OpenCV's native functions are too slow and require too much memory
	//Gray regions become white, colorful regions become black. Dark regions become even darker.

	uchar *grayData = (uchar *)imageResized->imageData;
    int grayStep = imageResized->widthStep;

    uchar *dataR = (uchar *)redChannelResized->imageData;
    uchar *dataG = (uchar *)greenChannelResized->imageData;
    uchar *dataB = (uchar *)blueChannelResized->imageData;
    int step = redChannelResized->widthStep;
	uchar newVal;
	int idx;

    for (int i = 0; i < redChannelResized->height; i++) 
	{
        for (int j = 0; j < redChannelResized->width; j++) 
		{
			idx = i*step+j;
			unsigned char B = dataR[idx];
			unsigned char G = dataG[idx];
			unsigned char R = dataB[idx];
			newVal = std::max(255 - static_cast<int>( darkness(R, G, B)*colorfulness(R, G, B) ), 0);
			newVal = static_cast<unsigned char>(darknessThresholdPenalty(R, G, B)*newVal);
			grayData[i*grayStep + j] = std::min( grayData[i*grayStep + j], newVal );
		}
	}
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
	int numberOfOk;
	vector<BorderAnalyzer> ba(numberOfAnalyzers, BorderAnalyzer(numberOfAnalyzers/4));
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

    for (int j = numberOfAnalyzers/2; j < width-numberOfAnalyzers/2; ++j) 
	{
		for(auto b = ba.begin(); b != ba.end(); ++b)
			(*b).invalidate();
		//for(int k = 0; k < numberOfAnalyzers; ++k)
		//	ba[k].invalidate();

		for (int i = height - 1 - numberOfAnalyzers/2; i >= numberOfAnalyzers/2; --i) 
		{
			numberOfOk = 0;
			tmpU8 = i*stepU8 + j;
			for(int k = 0; k < numberOfAnalyzers; ++k)
				numberOfOk += ba[k].analyze(dataRed[tmpU8 - 2 + k], dataGreen[tmpU8 - 2 + k], dataBlue[tmpU8 - 2 + k]);
			if(numberOfOk > numberOfAnalyzers*3/4) {
				dataUBorders[tmpU8] = 255;
				for(auto b = ba.begin(); b != ba.end(); ++b)
					(*b).invalidate();
			} else
				dataUBorders[tmpU8] = 0;
		}

		for(int k = 0; k < numberOfAnalyzers; ++k)
			ba[k].invalidate();

		for (int i = numberOfAnalyzers/2; i < height - numberOfAnalyzers/2; ++i) 
		{
			numberOfOk = 0;
			tmpU8 = i*stepU8 + j;
			for(int k = 0; k < numberOfAnalyzers; ++k)
				numberOfOk += ba[k].analyze(dataRed[tmpU8 - 2 + k], dataGreen[tmpU8 - 2 + k], dataBlue[tmpU8 - 2 + k]);
			if(numberOfOk > numberOfAnalyzers*3/4) {
				dataDBorders[tmpU8] = 255;
				for(auto b = ba.begin(); b != ba.end(); ++b)
					(*b).invalidate();
			} else
				dataDBorders[tmpU8] = 0;
		}
	}
	for (int i = numberOfAnalyzers/2; i < height - numberOfAnalyzers/2; ++i) 
	{	
		for(int k = 0; k < numberOfAnalyzers; ++k)
			ba[k].invalidate();

		for (int j = numberOfAnalyzers/2; j < width - numberOfAnalyzers/2; ++j) 
		{
			numberOfOk = 0;
			tmpU8 = i*stepU8 + j;
			for(int k = 0; k < numberOfAnalyzers; ++k)
				numberOfOk += ba[k].analyze(dataRed[tmpU8 + (-2 + k)*stepU8], dataGreen[tmpU8 + (-2 + k)*stepU8], dataBlue[tmpU8 + (-2 + k)*stepU8]);
			if(numberOfOk > numberOfAnalyzers*3/4) {
				dataRBorders[tmpU8] = 255;
				for(auto b = ba.begin(); b != ba.end(); ++b)
					(*b).invalidate();
			} else
				dataRBorders[tmpU8] = 0;
		}

		for(int k = 0; k < numberOfAnalyzers; ++k)
			ba[k].invalidate();

		for (int j = width - 1 - numberOfAnalyzers/2; j >= numberOfAnalyzers/2; --j) 
		{
			numberOfOk = 0;
			tmpU8 = i*stepU8 + j;
			for(int k = 0; k < numberOfAnalyzers; ++k)
				numberOfOk += ba[k].analyze(dataRed[tmpU8 + (-2 + k)*stepU8], dataGreen[tmpU8 + (-2 + k)*stepU8], dataBlue[tmpU8 + (-2 + k)*stepU8]);
			if(numberOfOk > numberOfAnalyzers*3/4) {
				dataLBorders[tmpU8] = 255;
				for(auto b = ba.begin(); b != ba.end(); ++b)
					(*b).invalidate();
			} else
				dataLBorders[tmpU8] = 0;
		}
	}
	
	cvIntegral(ubord, ubordII);
	cvIntegral(dbord, dbordII);
	cvIntegral(lbord, lbordII);
	cvIntegral(rbord, rbordII);

    //char act = cvWaitKey(100000);
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
	
	/*
	cvShowImage("ubord", uBorders);
	cvShowImage("dbord", dBorders);
	cvShowImage("lbord", lBorders);
	cvShowImage("rbord", rBorders);
	cvShowImage("uBordersFactored", uBordersFactored);
	cvShowImage("dBordersFactored", dBordersFactored);
	cvShowImage("lBordersFactored", lBordersFactored);
	cvShowImage("rBordersFactored", rBordersFactored);
	char c = cvWaitKey(100000);
	*/
	
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
					if(whiteBodyBorders < 400*255) //!
					{ 
						//TODO: make it constant
						//printf("wb %d %d\n", j, i);

						int borderPixels1b = pieceOfII(datauBordersIIFactored, j,						i, widthBorderBlock, safetyHeightMargin, step); 
						int borderPixels2b = pieceOfII(datauBordersIIFactored, j +   widthBorderBlock,  i, widthBorderBlock, safetyHeightMargin, step);
						int borderPixels3b = pieceOfII(datauBordersIIFactored, j + 2*widthBorderBlock,  i, widthBorderBlock, safetyHeightMargin, step);
						int borderPixels4b = pieceOfII(datauBordersIIFactored, j + 3*widthBorderBlock,  i, widthBorderBlock, safetyHeightMargin, step);
						if( blockPass(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, widthBorderBlock*255/safetyHeightMargin*4/5) ) //borderPixels1b > widthBorderBlock && borderPixels2b > widthBorderBlock && borderPixels3b > widthBorderBlock && borderPixels4b > widthBorderBlock)
						{
							//printf("ubord %d %d\n", j, i);
					
							borderPixels1b = pieceOfII(datalBordersIIFactored, j, i,					   safetyWidthMargin, heightBorderBlock, step); 
							borderPixels2b = pieceOfII(datalBordersIIFactored, j, i +   heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
							borderPixels3b = pieceOfII(datalBordersIIFactored, j, i + 2*heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
							borderPixels4b = pieceOfII(datalBordersIIFactored, j, i + 3*heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
					
							if( blockPass(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, heightBorderBlock*255/safetyWidthMargin*4/5) ) //borderPixels1b > heightBorderBlock && borderPixels2b > heightBorderBlock && borderPixels3b > heightBorderBlock && borderPixels4b > heightBorderBlock)
							{
								borderPixels1b = pieceOfII(datadBordersIIFactored, j,						i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step); 
								borderPixels2b = pieceOfII(datadBordersIIFactored, j +   widthBorderBlock,  i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step);
								borderPixels3b = pieceOfII(datadBordersIIFactored, j + 2*widthBorderBlock,  i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step);
								borderPixels4b = pieceOfII(datadBordersIIFactored, j + 3*widthBorderBlock,  i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step);	

								if( blockPass(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, widthBorderBlock*255/safetyHeightMargin*4/5) ) //borderPixels1b > widthBorderBlock && borderPixels2b > widthBorderBlock && borderPixels3b > widthBorderBlock && borderPixels4b > widthBorderBlock)
								{
									borderPixels1b = pieceOfII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i,					     safetyWidthMargin, heightBorderBlock, step); 
									borderPixels2b = pieceOfII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i +   heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
									borderPixels3b = pieceOfII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i + 2*heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
									borderPixels4b = pieceOfII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i + 3*heightBorderBlock, safetyWidthMargin, heightBorderBlock, step);
								
									if(blockPass(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, heightBorderBlock*255/safetyWidthMargin*4/5)) //( borderPixels1b > heightBorderBlock && borderPixels2b > heightBorderBlock && borderPixels3b > heightBorderBlock && borderPixels4b > heightBorderBlock)
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
	lhtHor.setPictureSpace(dataOrigin + yOffset*step + xOffset);
	CvPoint line = lhtHor.analyze();
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
		lhtHor.fullReset(-20, 20, 2*widthBorderBlock, 2*safetyHeightMargin, step, NULL);

		lineHorUL = findPreciseBorderAlignedLinesFindLineSubroutine(dataUBorders, step, x, y);
		testLines.push_back( lineHorUL );
		
		lineHorUR = findPreciseBorderAlignedLinesFindLineSubroutine(dataUBorders, step, x + 2*widthBorderBlock, y);
		testLines.push_back( lineHorUR );
		
		lineHorDL = findPreciseBorderAlignedLinesFindLineSubroutine(dataDBorders, step, x, y + 4*heightBorderBlock - 2*safetyHeightMargin);
		testLines.push_back( lineHorDL );
		
		lineHorDR = findPreciseBorderAlignedLinesFindLineSubroutine(dataDBorders, step, x + 2*widthBorderBlock, y + 4*heightBorderBlock - 2*safetyHeightMargin);
		testLines.push_back( lineHorDR );
		
		lhtHor.fullReset(70, 110, 2*safetyWidthMargin, 2*heightBorderBlock, step, NULL);

		lineVerUL = findPreciseBorderAlignedLinesFindLineSubroutine(dataLBorders, step, x, y);
		testLines.push_back( lineVerUL );
		
		lineVerDL = findPreciseBorderAlignedLinesFindLineSubroutine(dataLBorders, step, x, y + 2*heightBorderBlock);
		testLines.push_back( lineVerDL );
		
		lineVerUR = findPreciseBorderAlignedLinesFindLineSubroutine(dataRBorders, step, x + 4*widthBorderBlock - 2*safetyWidthMargin, y);
		testLines.push_back( lineVerUR );
		
		lineVerDR = findPreciseBorderAlignedLinesFindLineSubroutine(dataRBorders, step, x + 4*widthBorderBlock - 2*safetyWidthMargin, y + 2*heightBorderBlock);
		testLines.push_back( lineVerDR );
		
		CvPoint cornerUL = lineIntersection(lineHorUL.x*M_PI/180, -lineHorUL.y, lineVerUL.x*M_PI/180, -lineVerUL.y);
		CvPoint cornerUR = lineIntersection(lineHorUR.x*M_PI/180, -lineHorUR.y, lineVerUR.x*M_PI/180, -lineVerUR.y);
		CvPoint cornerDL = lineIntersection(lineHorDL.x*M_PI/180, -lineHorDL.y, lineVerDL.x*M_PI/180, -lineVerDL.y);
		CvPoint cornerDR = lineIntersection(lineHorDR.x*M_PI/180, -lineHorDR.y, lineVerDR.x*M_PI/180, -lineVerDR.y);
		testCorners.push_back( cornerUL );
		testCorners.push_back( cornerUR );
		testCorners.push_back( cornerDL );
		testCorners.push_back( cornerDR );

		foundA4.push_back( A4Record(cornerUL, cornerUR, cornerDL, cornerDR) );

		/*
		cvDrawRect(image,   cvPoint(x + 4*widthBorderBlock - 2*safetyWidthMargin, y), 
							cvPoint(x + 4*widthBorderBlock, y + 2*heightBorderBlock), CV_RGB(255, 255, 255), 1, 8, 0);
		cvShowImage("image", image);
		*/
	}
}

/*
bool A4Matcher::applyWhiteBodyDetector(int directionX, int directionY, int x0, int y0)
{
	int failsLeft = kernelSize;
	
    uchar *dataUBorder = (uchar *)uBordersFactored->imageData;
    uchar *dataDBorder = (uchar *)dBordersFactored->imageData;
    uchar *dataRBorder = (uchar *)rBordersFactored->imageData;
    uchar *dataLBorder = (uchar *)lBordersFactored->imageData;
	
    uchar *dataimageResized = (uchar *)imageResized->imageData;
    int stepimageResized = imageResized->widthStep;
	
	double ForegroundDispersion = 0;
	double meanForeground = 0;
	double likelihood = 0;
	int idx; 

	printf(">>> enter: %d %d\n", x0, y0);
	int test = 0;

	//Gathering stats
	for(unsigned int k = 0; k < kernelSize; ++k) 
	{
		for(unsigned int l = 0; l < kernelSize; ++l) 
		{
			//The further we come from corner the bigger safe margin we should leave
			if((k >= l/skewFactor)&&(l >= k/skewFactor))
			{
				++test;
				idx = (y0+directionY*k)*stepimageResized+(x0+directionX*l);
				int val = dataimageResized[idx];
				meanForeground += val;
				ForegroundDispersion += val*val;
				if(dataUBorder[idx] || dataDBorder[idx] || dataLBorder[idx] || dataRBorder[idx])
					--failsLeft;
			}
		}
		if(!failsLeft)
			return false;
	}
			
	meanForeground /= whiteBodySize;

	//What is supposed to be a Foreground is too dim
	if(meanForeground < 90) 
		return false;

	printf(">>> meanForeground passed: %d %d\n", x0-1, y0+1);

	ForegroundDispersion = (ForegroundDispersion - meanForeground*meanForeground*whiteBodySize)/(whiteBodySize - 1); 

	//debug
	assert(meanForeground >= 0 && ForegroundDispersion >= 0 && "Something deeply horrible happened: theese numbers should be positive");

	if(ForegroundDispersion > thresholdCornerForegroundDispersion)
		return false;
	
	printf(">>> ForegroundDispersion passed: %d %d\n", x0-1, y0+1);
			
	
	unsigned char ucMeanForeground = static_cast<unsigned char>(meanForeground);
	//Now applying more accurate pixel-by-pixel matcher
	for(unsigned int k = 0; k < kernelSize; ++k) {
		for(unsigned int l = 0; l < kernelSize; ++l) {
			//The further we come from corner the bigger safe margin we should leave
			if((k >= l/skewFactor)&&(l >= k/skewFactor))
			{
				uchar val = dataimageResized[(y0+directionY*k)*stepimageResized+(x0+directionX*l)];
				likelihood += matchForegroundTrimBlack(val, ucMeanForeground, sqrt(ForegroundDispersion));
			}
		}
	}
	

	if (likelihood > thresholdCornerLikelihood*whiteBodySize) 
	{ 
		printf(">>> likelihood passed: %d %d\n", x0-1, y0+1);
		return true;
	}
	return false;
}
*/

bool A4Matcher::applyBorderDetector(uchar *dataBorder, int step, int orthogonalStep, int borderLookupSize, int maxFails) 
{
	const int skewFactor = 4;
	const int initialRangeRadius = 2; // initialRange = 2*initialRangeRadius + 1
	int failsLeft = maxFails;
	int stepsLeft = borderLookupSize;
	int i;

	while(stepsLeft && failsLeft) 
	{
		bool borderFound = false;
		int range = ( (borderLookupSize - stepsLeft)/skewFactor + initialRangeRadius)*orthogonalStep;
		for(i = -range; i <= range && !borderFound; i += orthogonalStep)
			borderFound = borderFound || dataBorder[i];
		if(!borderFound)
			--failsLeft;
		dataBorder += step;
		--stepsLeft;
	}
	if(failsLeft > 0)
		return true;
	return false;
}


void A4Matcher::applyBaseCornerSearch(std::list<CvPoint>& cornersList, uchar *dataHorBorder, uchar *dataVerBorder, int horStep, int verStep)
{
    int step = imageResized->widthStep;
	
	const int kernelSize = 12;
	const int xyBorderLookupSize = 11;
	const int xyMaxFails = 2;
	const int xySafeShift = 2;
	int horOrthogonalStep = step;
	int verOrthogonalStep = 1;
	int whiteBodyDirectionX = horStep > 0 ? 1 : -1;
	int whiteBodyDirectionY = verStep > 0 ? 1 : -1;
	//const int diagonalProofLookupSize = 5;
	//const int diagonalMaxFails = 2;
	//const int diagonalSafeShift = 3; //Somehow several (not one as predicted) pixels are black on the corner

    uchar *data = (uchar *)imageResized->imageData;

    //uchar *dataUBorder = (uchar *)uBordersFactored->imageData;
    //uchar *dataRBorder = (uchar *)rBordersFactored->imageData;
    //uchar *dataCornerDetectorURSecond = (uchar *)cornerDetector2DerivativeUR->imageData;
    //uchar *dataCornerDetectorDRSecond = (uchar *)cornerDetector2DerivativeDR->imageData;
    //uchar *dataCornerDetectorURFirst = (uchar *)cornerDetector1DerivativeUR->imageData;
    //uchar *dataCornerDetectorDRFirst = (uchar *)cornerDetector1DerivativeDR->imageData;

	//printf("Begun analyzing w %d h %d \n", imageResized->width, imageResized->height);
	//FIXME kernelSize -> something more bugproof
    for (int i = kernelSize+1+xySafeShift; i < imageResized->height-kernelSize-1-xySafeShift; i++) 
	{
        for (int j = kernelSize+1+xySafeShift; j < imageResized->width-kernelSize-1-xySafeShift; j++) 
		{
			bool upperBorderDetected = applyBorderDetector(dataHorBorder + step*i + j + whiteBodyDirectionX*xySafeShift, horStep, horOrthogonalStep, xyBorderLookupSize, xyMaxFails);
			if(upperBorderDetected)
			{
				//printf("upperBorderDetected: %d %d\n", j-1, i+1);
				bool rightBorderDetected = applyBorderDetector(dataVerBorder + step*(i+whiteBodyDirectionY*xySafeShift) + j, verStep, verOrthogonalStep, xyBorderLookupSize, xyMaxFails);
				if(rightBorderDetected)
				{
					//printf("rightBorderDetected: %d %d\n", j-1, i+1);
					//int test = dataCornerDetectorDRFirst[step*(i-1) + j-1] + dataCornerDetectorDRFirst[step*(i-1) + j-2] + dataCornerDetectorDRFirst[step*(i+1) + j+1] + dataCornerDetectorDRFirst[step*(i+2) + j+1]
					//			+ dataCornerDetectorDRFirst[step*i + j-1] + dataCornerDetectorDRFirst[step*i + j-2] + dataCornerDetectorDRFirst[step*(i+1) + j] + dataCornerDetectorDRFirst[step*(i+2) + j];
					//bool diagonalUpperDerivativeDetected = applyBorderDetector(dataCornerDetectorDRFirst + step*i + j - diagonalSafeShift, -1, step, diagonalProofLookupSize, diagonalMaxFails);
					//bool diagonalRightDerivativeDetected = applyBorderDetector(dataCornerDetectorDRFirst + step*(i+diagonalSafeShift) + j, step,  1, diagonalProofLookupSize, diagonalMaxFails);
						//test >= 4*255; //true;
					//if(diagonalUpperDerivativeDetected && diagonalRightDerivativeDetected)
					//{
						printf("\n===\ndiagonalDerivativeDetected: %d %d\n", j + whiteBodyDirectionX, i + whiteBodyDirectionY);
						bool whiteBodyDetected = 1; //applyWhiteBodyDetector(whiteBodyDirectionX, whiteBodyDirectionY, j + whiteBodyDirectionX, i + whiteBodyDirectionY);
						if(whiteBodyDetected) 
						{
							printf("Corner found: %d %d\n", j + whiteBodyDirectionX, i + whiteBodyDirectionY);
							cornersList.push_back(cvPoint(j + whiteBodyDirectionX, i + whiteBodyDirectionY));
						}
					//}
				}
			}
		}
	}
}

void A4Matcher::applyURCornerSearch()
{
    int step = imageResized->widthStep;
	applyBaseCornerSearch(URCorners, (uchar *)uBordersFactored->imageData, (uchar *)rBordersFactored->imageData, -1, step);
}

void A4Matcher::applyULCornerSearch()
{
    int step = imageResized->widthStep;
	applyBaseCornerSearch(ULCorners, (uchar *)uBordersFactored->imageData, (uchar *)lBordersFactored->imageData, 1, step);
}

void A4Matcher::applyDRCornerSearch()
{
    int step = imageResized->widthStep;
	applyBaseCornerSearch(DRCorners, (uchar *)dBordersFactored->imageData, (uchar *)rBordersFactored->imageData, -1, -step);
}

void A4Matcher::applyDLCornerSearch()
{
    int step = imageResized->widthStep;
	applyBaseCornerSearch(DLCorners, (uchar *)dBordersFactored->imageData, (uchar *)lBordersFactored->imageData, 1, -step);
}

void A4Matcher::findCorners() 
{
	/*
	applyURCornerSearch();
	applyULCornerSearch();
	applyDRCornerSearch();
	applyDLCornerSearch();
	*/
}

bool A4Matcher::findA4() 
{
	bool foundFlag = false;
	const float angleTreshold = static_cast<float>(M_PI/24); //7.5 degrees
	const float a4sidesRatio = 99.0f/70.0f; //1.414, rational approximation of sqrt(2)
	const float a4sidesRatioDelta = 0.2f;

	for(std::list<CvPoint>::iterator itDL = DLCorners.begin(); itDL != DLCorners.end(); ++itDL)
	{
		CvPoint& ptDL = *itDL;
		for(std::list<CvPoint>::iterator itDR = DRCorners.begin(); itDR != DRCorners.end(); ++itDR)
		{
			CvPoint& ptDR = *itDR;
			float DLDRangle = atanf( abs(ptDR.y - ptDL.y)/static_cast<float>(abs(ptDR.x - ptDL.x)) );
			if( (ptDL.x < ptDR.x)&&(DLDRangle < angleTreshold) )
			{
				//printf("2 pt matched\n");
				for(std::list<CvPoint>::iterator itUL = ULCorners.begin(); itUL != ULCorners.end(); ++itUL)
				{
					CvPoint& ptUL = *itUL;
					float DLULangle = atanf( abs(ptDL.y - ptUL.y)/static_cast<float>(abs(ptUL.x - ptDL.x)) );
					if( (ptDL.y > ptUL.y)&&(DLULangle > M_PI/2 - angleTreshold) )
					{
						for(std::list<CvPoint>::iterator itUR = URCorners.begin(); itUR != URCorners.end(); ++itUR)
						{ 
							CvPoint& ptUR = *itUR;
							float DRURangle = atanf( abs(ptUR.y - ptDR.y)/static_cast<float>(abs(ptUR.x - ptDR.x)) );
							float ULURangle = atanf( abs(ptUR.y - ptUL.y)/static_cast<float>(abs(ptUR.x - ptUL.x)) );
							if( (ptDL.y > ptUR.y)&&(DRURangle > M_PI/2 - angleTreshold)&&(ULURangle < angleTreshold) )
							{
								float hor1 = std::hypotf( static_cast<float>(ptDL.x - ptDR.x), static_cast<float>(ptDL.y - ptDR.y) );
								float hor2 = std::hypotf( static_cast<float>(ptUL.x - ptUR.x), static_cast<float>(ptUL.y - ptUR.y) );
								float vert1 = std::hypotf( static_cast<float>(ptUL.x - ptDL.x), static_cast<float>(ptUL.y - ptDL.y) );
								float vert2 = std::hypotf( static_cast<float>(ptUR.x - ptDR.x), static_cast<float>(ptUR.y - ptDR.y) );
								if( (a4sidesRatio - a4sidesRatioDelta < hor1/vert1) && (hor1/vert1 < a4sidesRatio + a4sidesRatioDelta) &&
									(a4sidesRatio - a4sidesRatioDelta < hor2/vert2) && (hor2/vert2 < a4sidesRatio + a4sidesRatioDelta) )
								{
									foundA4.push_back(A4Record(ptUL, ptUR, ptDL, ptDR));
									foundFlag = true;
								}
							}
						}
					}
				}
			}
		}
	}
	return foundFlag;
}


void A4Matcher::dump()
{
    //namedWindow( "src", CV_WINDOW_AUTOSIZE );
    //namedWindow( "imageResized", CV_WINDOW_AUTOSIZE );
    //cv::namedWindow( "imageResized", CV_WINDOW_AUTOSIZE );
	//printf("#\n");
	/*
	for(std::list<CvPoint>::iterator it = ULCorners.begin(); it != ULCorners.end(); ++it) {
		cvDrawCircle(image, *it, debugShowSize, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	for(std::list<CvPoint>::iterator it = URCorners.begin(); it != URCorners.end(); ++it) {
		cvDrawCircle(image, *it, debugShowSize, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	for(std::list<CvPoint>::iterator it = DLCorners.begin(); it != DLCorners.end(); ++it) {
		cvDrawCircle(image, *it, 3*debugShowSize, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	for(std::list<CvPoint>::iterator it = DRCorners.begin(); it != DRCorners.end(); ++it) {
		cvDrawCircle(image, *it, 3*debugShowSize, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	for(std::list<A4PreDetectedRecord>::iterator it = A4PreDetected.begin(); it != A4PreDetected.end(); ++it) {
		cvDrawRect(image, (*it).ulpt, (*it).drpt, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	*/
	/*
	for(std::list<CvRect>::iterator it = foundA4.begin(); it != foundA4.end(); ++it) {
		cvDrawRect(imageResized, cvPoint((*it).x, (*it).y), cvPoint((*it).x - (*it).width, (*it).y - (*it).height), CV_RGB(255, 255, 255), 3, 8, 0);
	}
	for(std::list<CvRect>::iterator it = foundA4.begin(); it != foundA4.end(); ++it) {
		cvDrawRect(image, cvPoint(2*(*it).x, 2*(*it).y), cvPoint(2*((*it).x - (*it).width), 2*((*it).y - (*it).height)), CV_RGB(255, 255, 255), 3, 8, 0);
	}
	*/
	//cvDrawCircle(imageResized, cvPoint(imageResized->width/2, imageResized->height/2), 2, CV_RGB(255, 255, 255), 1, 8, 0);
	//cvShowImage("image", image);
	//cvShowImage("redChannelResized", redChannelResized);
	//cvShowImage("greenChannelResized", greenChannelResized);
	//cvShowImage("blueChannelResized", blueChannelResized);
	//cvShowImage("imageCanny", imageCanny);
	//cvShowImage("imageCannyResized", imageCannyResized);
	//cvShowImage("imageResized", imageResized);
	//cvShowImage("verticalBorders", verticalBorders);
	//cvShowImage("horizontalBorders", horizontalBorders);
	//cvShowImage("cornerDetector1DerivativeDR", cornerDetector1DerivativeDR);
}

void A4Matcher::normalizePoints()
{
	for(std::list<CvPoint>::iterator it = ULCorners.begin(); it != ULCorners.end(); ++it) {
		(*it).x = (*it).x*resizeFactor + resizeFactor/2;
		(*it).y = (*it).y*resizeFactor + resizeFactor/2;
	}
	for(std::list<CvPoint>::iterator it = URCorners.begin(); it != URCorners.end(); ++it) {
		(*it).x = (*it).x*resizeFactor + resizeFactor/2;
		(*it).y = (*it).y*resizeFactor + resizeFactor/2;
	}
	for(std::list<CvPoint>::iterator it = DLCorners.begin(); it != DLCorners.end(); ++it) {
		(*it).x = (*it).x*resizeFactor + resizeFactor/2;
		(*it).y = (*it).y*resizeFactor + resizeFactor/2;
	}
	for(std::list<CvPoint>::iterator it = DRCorners.begin(); it != DRCorners.end(); ++it) {
		(*it).x = (*it).x*resizeFactor + resizeFactor/2;
		(*it).y = (*it).y*resizeFactor + resizeFactor/2;
	}
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

/*
void A4Matcher::addIntersectionsToCornersList()
{
	//First removing points of intersections overlapping with already found corner points
	bool foundFlag;
	for(std::list<CvPoint>::iterator itIntersection = lineClusterifier.intersections.begin(); itIntersection != lineClusterifier.intersections.end(); ) {
		foundFlag = false;
		for(std::list<CvPoint>::iterator it = ULCorners.begin(); it != ULCorners.end() && !foundFlag; ++it) {
			if( distanceManhattan(*itIntersection, *it) < cornerToIntersectionMaxDistance )
				foundFlag = true;
		}
		for(std::list<CvPoint>::iterator it = URCorners.begin(); it != URCorners.end() && !foundFlag; ++it) {
			if( distanceManhattan(*itIntersection, *it) < cornerToIntersectionMaxDistance )
				foundFlag = true;
		}
		for(std::list<CvPoint>::iterator it = DLCorners.begin(); it != DLCorners.end() && !foundFlag; ++it) {
			if( distanceManhattan(*itIntersection, *it) < cornerToIntersectionMaxDistance )
				foundFlag = true;
		}
		for(std::list<CvPoint>::iterator it = DRCorners.begin(); it != DRCorners.end() && !foundFlag; ++it) {
			if( distanceManhattan(*itIntersection, *it) < cornerToIntersectionMaxDistance )
				foundFlag = true;
		}
		if(foundFlag) {
			itIntersection = lineClusterifier.intersections.erase(itIntersection);
		} else
			++itIntersection;
	}
	//Then adding points of intersections to corners' lists of every type as we don't know which type are they exactly
	for(std::list<CvPoint>::iterator itIntersection = lineClusterifier.intersections.begin(); itIntersection != lineClusterifier.intersections.end(); ++itIntersection) {
		ULCorners.push_back(*itIntersection);
		URCorners.push_back(*itIntersection);
		DLCorners.push_back(*itIntersection);
		DRCorners.push_back(*itIntersection);
	}
}
*/

void A4Matcher::clearResults()
{
	ULCorners.clear();
	URCorners.clear();
	DLCorners.clear();
	DRCorners.clear();
	A4PreDetected.clear();
	foundA4.clear();
}

A4Matcher::~A4Matcher(void)
{
	clearMemory();
}

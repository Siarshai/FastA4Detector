#ifndef A4PREDETECTOR
#define A4PREDETECTOR

#include "Utils.h"
#include "A4MemoryBank.h"
#include "A4InfoStructures.h"

using std::max;
using std::min;

class A4PreDetector
{
	void normalizePoints(A4MemoryBank *memoryBank)
	{
		for(std::list<A4PreDetectedRecord>::iterator it = A4PreDetected.begin(); it != A4PreDetected.end(); ++it) {
			(*it).ulpt.x = (*it).ulpt.x*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).ulpt.y = (*it).ulpt.y*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).drpt.x = (*it).drpt.x*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).drpt.y = (*it).drpt.y*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).ulptBorder.x = (*it).ulptBorder.x*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).ulptBorder.y = (*it).ulptBorder.y*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).drptBorder.x = (*it).drptBorder.x*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
			(*it).drptBorder.y = (*it).drptBorder.y*memoryBank->resizeFactor + memoryBank->resizeFactor/2;
		}
	}
public:
	std::list<A4PreDetectedRecord> A4PreDetected;
	void detect(A4MemoryBank *memoryBank)
	{
		unsigned char *dataUBorders = (unsigned char *)memoryBank->uBordersFactored->imageData;
		unsigned char *dataDBorders = (unsigned char *)memoryBank->dBordersFactored->imageData;
		unsigned char *dataLBorders = (unsigned char *)memoryBank->lBordersFactored->imageData;
		unsigned char *dataRBorders = (unsigned char *)memoryBank->rBordersFactored->imageData;
		int *datauBordersIIFactored = (int *)memoryBank->uBordersIIFactored->imageData;
		int *datadBordersIIFactored = (int *)memoryBank->dBordersIIFactored->imageData;
		int *datalBordersIIFactored = (int *)memoryBank->lBordersIIFactored->imageData;
		int *datarBordersIIFactored = (int *)memoryBank->rBordersIIFactored->imageData;
		unsigned char *databufferFactored = (unsigned char *)memoryBank->bufferFactored->imageData;
		int step = memoryBank->uBordersIIFactored->widthStep/sizeof(int);
		int stepbufferFactored = memoryBank->bufferFactored->widthStep/sizeof(char);

		int searchWidth;
		int searchHeight;
		const float maxSizeDivisor = 1.0f/2.0f; //2.0f/3.0f; // <<<======================
		const int kernelResolution = 1;
		if(memoryBank->sizeFactored.width > memoryBank->sizeFactored.height*99/70) {
			searchWidth = static_cast<int>( memoryBank->sizeFactored.width*maxSizeDivisor ); //TODO: make it constant
			searchHeight = searchWidth*70/99;
		} else {
			searchHeight = static_cast<int>( memoryBank->sizeFactored.height*maxSizeDivisor ); //TODO: make it constant
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
			for(int i = 0; i < memoryBank->sizeFactored.height - searchHeight - 2*safetyHeightMargin; ++i) 
			{
				for(int j = 0; j < memoryBank->sizeFactored.width - searchWidth - 2*safetyWidthMargin; ++j)
				{
					if(databufferFactored[i*stepbufferFactored + j] == 0)
					{
						int whiteBodyBorders = pieceOfII(datauBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
						whiteBodyBorders += pieceOfII(datadBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
						whiteBodyBorders += pieceOfII(datarBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
						whiteBodyBorders += pieceOfII(datalBordersIIFactored, j + safetyWidthMargin, i + safetyHeightMargin, searchWidth, searchHeight, step);
						if(    whiteBodyBorders < 50*255 
							&& horizontalBlockPassII(datauBordersIIFactored, j, i, widthBorderBlock, safetyHeightMargin, step)
							&& verticalBlockPassII(datalBordersIIFactored, j, i, safetyWidthMargin, heightBorderBlock, step) 
							&& horizontalBlockPassII(datadBordersIIFactored, j, i + searchHeight + safetyHeightMargin, widthBorderBlock, safetyHeightMargin, step) 
							&& verticalBlockPassII(datarBordersIIFactored, j + searchWidth + safetyWidthMargin, i, safetyWidthMargin, heightBorderBlock, step) )
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

			searchHeight -= kernelResolution; //safetyHeightMargin;
			searchWidth = searchHeight*99/70;
		
			safetyWidthMargin = max(searchHeight/safetyMarginDivisor, kernelResolution); //TODO: note they switched places
			safetyHeightMargin = max(searchWidth/safetyMarginDivisor, kernelResolution);
			widthBorderBlock = (searchWidth + 2*safetyWidthMargin)/4; //4 border blocks
			heightBorderBlock = (searchHeight + 2*safetyHeightMargin)/4; //4 border blocks
		}
		normalizePoints(memoryBank);
	}
};


#endif //A4PREDETECTOR
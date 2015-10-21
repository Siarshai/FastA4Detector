#include "stdafx.h"
#include "A4PreciseDetector.h"


CvPoint A4PreciseDetector::findPreciseBorderAlignedLinesFindLineSubroutine(unsigned char *dataOrigin, int step, int xOffset, int yOffset)
{
	localHoughTransformer.setPictureSpace(dataOrigin + yOffset*step + xOffset);
	CvPoint line;
	try {
		line = localHoughTransformer.analyze();
	} catch(...) { throw; }
	line.y += static_cast<int>(-xOffset*sin(line.x*M_PI/180.0) + yOffset*cos(line.x*M_PI/180.0));
	return line;
}


void A4PreciseDetector::detect(A4MemoryBank *memoryBank, std::list<A4PreDetectedRecord> &A4PreDetected)
{
	for(A4PreDetectedRecord a4pd : A4PreDetected)
	{
		int x = a4pd.ulptBorder.x;
		int y = a4pd.ulptBorder.y;

		int widthBorderBlock = a4pd.widthBorderBlock*memoryBank->resizeFactor;
		int heightBorderBlock = a4pd.heightBorderBlock*memoryBank->resizeFactor;
		int safetyWidthMargin = a4pd.safetyWidthMargin*memoryBank->resizeFactor;
		int safetyHeightMargin = a4pd.safetyHeightMargin*memoryBank->resizeFactor;
		
		unsigned char *dataUBorders = (unsigned char *)memoryBank->uBorders->imageData;
		unsigned char *dataDBorders = (unsigned char *)memoryBank->dBorders->imageData;
		unsigned char *dataLBorders = (unsigned char *)memoryBank->lBorders->imageData;
		unsigned char *dataRBorders = (unsigned char *)memoryBank->rBorders->imageData;
		int step = memoryBank->uBorders->widthStep;
		
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
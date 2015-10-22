#include "stdafx.h"
#include "Utils.h"
#include "BorderAnalyzer.h"



LocalBorderAnalyzer::LocalBorderAnalyzer(int amaxAftermath, 
										 double agradientMidTreshold, double agradientHighTreshold, 
										 double adarknessLowTreshold, double adarknessMidTreshold, double adarknessHighTreshold,
										 double acolorfulnessLowTreshold, double acolorfulnessMidTreshold, double acolorfulnessHighTreshold,
										 double adarknessWeightMaxPenalty, double adarknessWeightMaxBoon,
										 double acolorfulnessMaxPenalty, double acolorfulnessMaxBoon) :
	lastR(0), lastG(0), lastB(0), 
	last2R(0), last2G(0), last2B(0), 
	aftermath(0), maxAftermath(amaxAftermath),
	gradientMidTreshold(agradientMidTreshold), gradientHighTreshold(agradientHighTreshold),  //gradientMidTreshold(10.0), gradientHighTreshold(12.0), 
	darknessLowTreshold(adarknessLowTreshold), darknessMidTreshold(adarknessMidTreshold), darknessHighTreshold(adarknessHighTreshold),  //darknessLowTreshold(90.0), darknessMidTreshold(110.0), darknessHighTreshold(130.0), 
	colorfulnessLowTreshold(acolorfulnessLowTreshold), colorfulnessMidTreshold(acolorfulnessMidTreshold), colorfulnessHighTreshold(acolorfulnessHighTreshold), //colorfulnessLowTreshold(120.0*500.0), colorfulnessMidTreshold(120.0*1000.0), colorfulnessHighTreshold(120.0*1200.0), 
	gradientWeight(0), darknessWeight(0), //darknessLowTreshold(240)
	darknessWeightMaxPenalty(adarknessWeightMaxPenalty), darknessWeightMaxBoon(adarknessWeightMaxBoon), // -0.2  0.05
	colorfulnessMaxPenalty(acolorfulnessMaxPenalty), colorfulnessMaxBoon(acolorfulnessMaxBoon) // -0.2 0.06
	{} 

bool LocalBorderAnalyzer::analyze(int r, int g, int b)
{
	//double cleanGradient = abs((r - last2R)/2) + abs((g - last2G)/2) + abs((b - last2B)/2); // sumOfAbsoluteValues(firstDerivative(r, last2R), firstDerivative(g, last2G), firstDerivative(b, last2B));
	double cleanGradient = (last2R - r)/2 + (last2G - g)/2 + (last2B - b)/2;
	if(cleanGradient > 3*gradientHighTreshold)
		cleanGradient = 3*gradientHighTreshold;
	double weightedGradient = gradientWeight*cleanGradient;

	last2R = lastR;
	last2G = lastG;
	last2B = lastB;
	lastR = r;
	lastG = g;
	lastB = b;
	if(aftermath) 
	{
		--aftermath;
		return true;
	}
	if(weightedGradient > gradientHighTreshold) 
	{
		aftermath = maxAftermath;
		gradientWeight = 0.75;
		return true;
	}
			
	double darkness = sumOfAbsoluteValues(r, g, b)/3;

	if(darkness < darknessLowTreshold) 
	{
		gradientWeight = 0;
		return false;
	}
	darknessWeight = generalSlopeFunction(darkness, darknessWeightMaxPenalty, darknessLowTreshold, darknessWeightMaxBoon, darknessHighTreshold); 
	gradientWeight += darknessWeight;
			
	double colors = colorfulness(r, g, b)*(0.6 - darknessWeight); // <<
	if(colors > colorfulnessHighTreshold)
	{
		gradientWeight = 0;
		return false;
	}
	
	if(darkness < darknessHighTreshold) 
	{
		double penalty = (darknessHighTreshold - darkness)*2/1000.0;
		gradientWeight += generalSlopeFunction(colors, colorfulnessMaxBoon - penalty, colorfulnessLowTreshold, colorfulnessMaxPenalty - penalty, colorfulnessHighTreshold); 
	}

	if(gradientWeight < 0.0)
		gradientWeight = 0.0;
	if(gradientWeight > 1.0)
		gradientWeight = 1.0;
	return false;
}


void LocalBorderAnalyzer::invalidate()
{
	last2R = 0;
	last2G = 0;
	last2B = 0;
	lastR = 0;
	lastG = 0;
	lastB = 0;
	aftermath = 0;
	gradientWeight = 0.0;
	darknessWeight = 0.0;
}

void LocalBorderAnalyzer::discardAftermath()
{
	aftermath = 0;
}



void ChoirOfLocalBorderAnalyzers::response(int initialPoint, int shiftMultiplier, unsigned char *dataRed, unsigned char *dataGreen, unsigned char *dataBlue, unsigned char *dataBorders) 
{
	int numberOfOk = 0; 
	for(int k = 0; k < numberOfAnalyzers; ++k)
		numberOfOk += ba[k].analyze(dataRed[initialPoint + shiftMultiplier*(k - numberOfAnalyzers/2)], dataGreen[initialPoint + shiftMultiplier*(k - numberOfAnalyzers/2)], dataBlue[initialPoint + shiftMultiplier*(k - numberOfAnalyzers/2)]);
	if(numberOfOk > agreementFactor) {
		dataBorders[initialPoint] = 255;
		invalidate();
	} else
		dataBorders[initialPoint] = 0;
}


void ChoirOfLocalBorderAnalyzers::invalidate()
{	
	for(auto b = ba.begin(); b != ba.end(); ++b)
		(*b).invalidate();
}


BorderAnalyzer::BorderAnalyzer(int numberOfAnalyzers, LocalBorderAnalyzer& lba) : 
	cba(numberOfAnalyzers, numberOfAnalyzers*3/4, lba)
{
} 


void BorderAnalyzer::prepareDerivativesSearchTemplatesBase(IplImage *rc, IplImage *gc, IplImage *bc, 
													  IplImage *ubord, IplImage *dbord, IplImage *lbord, IplImage *rbord, 
													  IplImage *ubordII, IplImage *dbordII, IplImage *lbordII, IplImage *rbordII, 
													  IplImage *buff, int numberOfAnalyzers)
{

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


void BorderAnalyzer::prepareDerivativesSearchTemplates(A4MemoryBank *memoryBank)
{	
	const int numberOfAnalyzers = 44; 
	const int numberOfAnalyzersFactored = numberOfAnalyzers/memoryBank->resizeFactor; 

	prepareDerivativesSearchTemplatesBase(memoryBank->redChannelResized, memoryBank->greenChannelResized, memoryBank->blueChannelResized, 
													  memoryBank->uBordersFactored, memoryBank->dBordersFactored, memoryBank->lBordersFactored, memoryBank->rBordersFactored, 
													  memoryBank->uBordersIIFactored, memoryBank->dBordersIIFactored, memoryBank->lBordersIIFactored, memoryBank->rBordersIIFactored, 
													  memoryBank->bufferFactored, numberOfAnalyzersFactored);

	prepareDerivativesSearchTemplatesBase(memoryBank->redChannel, memoryBank->greenChannel, memoryBank->blueChannel, 
													  memoryBank->uBorders, memoryBank->dBorders, memoryBank->lBorders, memoryBank->rBorders, 
													  memoryBank->uBordersII, memoryBank->dBordersII, memoryBank->lBordersII, memoryBank->rBordersII, 
													  memoryBank->buffer, numberOfAnalyzers);
}
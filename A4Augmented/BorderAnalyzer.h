#ifndef LOCALBORDERANALYZER_H
#define LOCALBORDERANALYZER_H

#include "Utils.h"
#include "A4MemoryBank.h"

using std::vector;

class LocalBorderAnalyzer
{
private:
	uchar lastR;
	uchar lastG;
	uchar lastB;
	uchar last2R;
	uchar last2G;
	uchar last2B;
	int aftermath;
	int maxAftermath;
	double gradientHighTreshold;
	double gradientMidTreshold;
	double darknessLowTreshold;
	double darknessMidTreshold;
	double darknessHighTreshold;
	double colorfulnessLowTreshold;
	double colorfulnessMidTreshold;
	double colorfulnessHighTreshold;
	double gradientWeight;
	double darknessWeight;
public:
	LocalBorderAnalyzer(int amaxAftermath) : lastR(0), lastG(0), lastB(0), last2R(0), last2G(0), last2B(0), aftermath(0), maxAftermath(amaxAftermath),
		gradientMidTreshold(10.0), gradientHighTreshold(12.0), 
		darknessLowTreshold(90.0), darknessMidTreshold(110.0), darknessHighTreshold(130.0), 
		colorfulnessLowTreshold(120.0*500.0), colorfulnessMidTreshold(120.0*1000.0), colorfulnessHighTreshold(120.0*1200.0), gradientWeight(0), darknessWeight(0) {} //darknessLowTreshold(240)
	bool analyze(int r, int g, int b);
	void invalidate();
	void discardAftermath();
};


class ChoirOfLocalBorderAnalyzers 
{
private:
	vector<LocalBorderAnalyzer> ba;
	int numberOfAnalyzers, agreementFactor;
public:
	ChoirOfLocalBorderAnalyzers(int anumberOfAnalyzers, int aaftermathFactor, int aagreementFactor) : ba(anumberOfAnalyzers, LocalBorderAnalyzer(aaftermathFactor)), 
		numberOfAnalyzers(anumberOfAnalyzers), agreementFactor(aagreementFactor) {}
	void response(int initialPoint, int shiftMultiplier, unsigned char *dataRed, unsigned char *dataGreen, unsigned char *dataBlue, unsigned char *dataBorders) 
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
	void invalidate()
	{	
		for(auto b = ba.begin(); b != ba.end(); ++b)
			(*b).invalidate();
	}
};



class BorderAnalyzer
{
public:
	void prepareDerivativesSearchTemplatesBase(IplImage *rc, IplImage *gc, IplImage *bc, 
													  IplImage *ubord, IplImage *dbord, IplImage *lbord, IplImage *rbord, 
													  IplImage *ubordII, IplImage *dbordII, IplImage *lbordII, IplImage *rbordII, 
													  IplImage *buff, int numberOfAnalyzers);
	void prepareDerivativesSearchTemplates(A4MemoryBank *memoryBank);
};

#endif
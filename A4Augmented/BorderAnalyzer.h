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
	double darknessWeightMaxPenalty;
	double darknessWeightMaxBoon;
	double colorfulnessMaxPenalty;
	double colorfulnessMaxBoon;
public:
	LocalBorderAnalyzer(int amaxAftermath, 
										 double agradientMidTreshold, double agradientHighTreshold, 
										 double adarknessLowTreshold, double adarknessMidTreshold, double adarknessHighTreshold,
										 double acolorfulnessLowTreshold, double acolorfulnessMidTreshold, double acolorfulnessHighTreshold,
										 double adarknessWeightMaxPenalty, double adarknessWeightMaxBoon,
										 double acolorfulnessMaxPenalty, double acolorfulnessMaxBoon );
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
	ChoirOfLocalBorderAnalyzers(int anumberOfAnalyzers, int aagreementFactor, LocalBorderAnalyzer& lba) : ba(anumberOfAnalyzers, lba), 
		numberOfAnalyzers(anumberOfAnalyzers), agreementFactor(aagreementFactor) {}
	void response(int initialPoint, int shiftMultiplier, unsigned char *dataRed, unsigned char *dataGreen, unsigned char *dataBlue, unsigned char *dataBorders);
	void invalidate();
};



class BorderAnalyzer
{
	ChoirOfLocalBorderAnalyzers cba;
public:
	BorderAnalyzer(int numberOfAnalyzers, LocalBorderAnalyzer& lba);
	void prepareDerivativesSearchTemplatesBase(IplImage *rc, IplImage *gc, IplImage *bc, 
													  IplImage *ubord, IplImage *dbord, IplImage *lbord, IplImage *rbord, 
													  IplImage *ubordII, IplImage *dbordII, IplImage *lbordII, IplImage *rbordII, 
													  IplImage *buff, int numberOfAnalyzers);
	void prepareDerivativesSearchTemplates(A4MemoryBank *memoryBank);
};

#endif
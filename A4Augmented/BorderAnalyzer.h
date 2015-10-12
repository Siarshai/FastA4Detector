#ifndef BORDERANALYZER_H
#define BORDERANALYZER_H

#include "Utils.h"


class BorderAnalyzer
{
private:
	uchar lastR;
	uchar lastG;
	uchar lastB;
	uchar last2R;
	uchar last2G;
	uchar last2B;
	int aftermath;
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
	BorderAnalyzer() : lastR(0), lastG(0), lastB(0), last2R(0), last2G(0), last2B(0), aftermath(2),
		gradientMidTreshold(10.0), gradientHighTreshold(12.0), 
		darknessLowTreshold(90.0), darknessMidTreshold(110.0), darknessHighTreshold(130.0), 
		colorfulnessLowTreshold(120.0*500.0), colorfulnessMidTreshold(120.0*1000.0), colorfulnessHighTreshold(120.0*1200.0), gradientWeight(0) {} //darknessLowTreshold(240)
	bool analyze(int r, int g, int b);
	void invalidate();
};

#endif
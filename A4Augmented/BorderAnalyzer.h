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
	bool aftermath;
	const double gradientHighTreshold;
	const double gradientMidTreshold;
	const double darknessLowTreshold;
	const double darknessMidTreshold;
	const double darknessHighTreshold;
	const double colorfulnessLowTreshold;
	const double colorfulnessMidTreshold;
	const double colorfulnessHighTreshold;
	double gradientWeight;
	double darknessWeight;
public:
	BorderAnalyzer() : lastR(0), lastG(0), lastB(0), last2R(0), last2G(0), last2B(0), aftermath(false),
		gradientMidTreshold(10.0), gradientHighTreshold(15.0), 
		darknessLowTreshold(100.0), darknessMidTreshold(120.0), darknessHighTreshold(140.0), 
		colorfulnessLowTreshold(120.0*400.0), colorfulnessMidTreshold(120.0*900.0), colorfulnessHighTreshold(120.0*900.0), gradientWeight(0) {} //darknessLowTreshold(240)
	bool analyze(int r, int g, int b);
	void invalidate();
};

#endif
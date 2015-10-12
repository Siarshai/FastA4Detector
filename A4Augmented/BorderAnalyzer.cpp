#include "stdafx.h"
#include "Utils.h"
#include "BorderAnalyzer.h"


bool BorderAnalyzer::analyze(int r, int g, int b)
{
	double cleanGradient = abs((r - last2R)/2) + abs((g - last2G)/2) + abs((b - last2B)/2); // sumOfAbsoluteValues(firstDerivative(r, last2R), firstDerivative(g, last2G), firstDerivative(b, last2B));
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
	darknessWeight = generalSlopeFunction(darkness, -0.2, darknessLowTreshold, 0.05, darknessHighTreshold); 
	gradientWeight += darknessWeight;
			
	double colors = colorfulness(r, g, b)*(0.6 - darknessWeight);
	if(colors > colorfulnessHighTreshold)
	{
		gradientWeight = 0;
		return false;
	}
	
	if(darkness < darknessHighTreshold) 
	{
		double penalty = (darknessHighTreshold - darkness)*2/1000.0;
		gradientWeight += generalSlopeFunction(colors, 0.06 - penalty, colorfulnessLowTreshold, -0.2 - penalty, colorfulnessHighTreshold); 
	}

	if(gradientWeight < 0.0)
		gradientWeight = 0.0;
	if(gradientWeight > 1.0)
		gradientWeight = 1.0;
	return false;
}


void BorderAnalyzer::invalidate()
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

void BorderAnalyzer::discardAftermath()
{
	aftermath = 0;
}

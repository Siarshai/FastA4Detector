#include "stdafx.h"
#include <cmath>
#include "A4Matcher.h"


A4Matcher::A4Matcher()
{
	memoryBankPImpl = new A4MemoryBank();
	borderAnalyzerPImpl = new BorderAnalyzer();
	preDetectorPImpl = new A4PreDetector();
	preciseDetectorPImpl = new A4PreciseDetector();
}

/*
void A4Matcher::initMemory(CvSize size, int resizeFactor)
{
	memoryBankPImpl->clearMemory();
	memoryBankPImpl->initMemory(size, resizeFactor);
}
*/

void A4Matcher::clearMemory()
{
	memoryBankPImpl->clearMemory();
}

void A4Matcher::setAndAnalyseImage(IplImage *aimage)
{
	clearResults();
	memoryBankPImpl->consumeImage(aimage);
	borderAnalyzerPImpl->prepareDerivativesSearchTemplates(memoryBankPImpl);
	preDetectorPImpl->detect(memoryBankPImpl);
	preciseDetectorPImpl->detect(memoryBankPImpl, preDetectorPImpl->A4PreDetected);
}


void A4Matcher::clearResults()
{
	preDetectorPImpl->A4PreDetected.clear();
	preciseDetectorPImpl->A4PreciseDetected.clear();
}

A4Matcher::~A4Matcher(void)
{
	clearMemory();
}

std::list<A4PreDetectedRecord> & A4Matcher::getPreResults()
{
	return preDetectorPImpl->A4PreDetected;
}

std::list<A4PreciseDetectedRecord> & A4Matcher::getPreciseResults()
{
	return preciseDetectorPImpl->A4PreciseDetected;
}
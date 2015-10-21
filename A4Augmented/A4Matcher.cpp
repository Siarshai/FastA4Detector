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
	preciseDetectorPImpl->detect(memoryBankPImpl, preDetectorPImpl->getResults());
}


void A4Matcher::clearResults()
{
	preDetectorPImpl->clear();
	preciseDetectorPImpl->clear();
}

A4Matcher::~A4Matcher(void)
{
	clearMemory();
}

const std::list<A4PreDetectedRecord> & A4Matcher::getPreResults()
{
	return preDetectorPImpl->getResults();
}

const std::list<A4PreciseDetectedRecord> & A4Matcher::getPreciseResults()
{
	return preciseDetectorPImpl->getResults();
}
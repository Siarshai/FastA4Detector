#include "stdafx.h"
#include <cmath>
#include "A4Matcher.h"
#include "TextRemover.h"


A4Matcher::A4Matcher(BorderAnalyzer* aBorderDetector, APreDetector *aPreDetector, APreciseDetector *aPreciseDetector, int aresizeFactor) : resizeFactor(aresizeFactor)
{
	memoryBankPImpl = new A4MemoryBank();
	borderAnalyzerPImpl = aBorderDetector;
	preDetectorPImpl = aPreDetector; 
	preciseDetectorPImpl = aPreciseDetector;
}

A4Matcher::~A4Matcher()
{
	delete memoryBankPImpl;
	delete borderAnalyzerPImpl;
	delete preDetectorPImpl;
	delete preciseDetectorPImpl;
}


void A4Matcher::clearMemory()
{
	memoryBankPImpl->clearMemory();
}

void A4Matcher::setAndAnalyseImage(IplImage *aimage)
{
	clearResults();
	memoryBankPImpl->consumeImage(aimage, resizeFactor);


	memoryBankPImpl->dumpFactored();
	/*
	TextRemover tr(3);
	tr.process(memoryBankPImpl);
	memoryBankPImpl->dumpFactored();
	*/
	borderAnalyzerPImpl->prepareDerivativesSearchTemplates(memoryBankPImpl);

	memoryBankPImpl->dumpBordersFactored();

	preDetectorPImpl->detect(memoryBankPImpl);
	preciseDetectorPImpl->detect(memoryBankPImpl, preDetectorPImpl->getResults());
}


void A4Matcher::clearResults()
{
	preDetectorPImpl->clear();
	preciseDetectorPImpl->clear();
}

const std::list<A4PreDetectedRecord> & A4Matcher::getPreResults()
{
	return preDetectorPImpl->getResults();
}

const std::list<A4PreciseDetectedRecord> & A4Matcher::getPreciseResults()
{
	return preciseDetectorPImpl->getResults();
}


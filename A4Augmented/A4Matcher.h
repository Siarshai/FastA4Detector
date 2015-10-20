#ifndef A4MATCHER_H
#define A4MATCHER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <array>
#include <unordered_map>

#include "BorderAnalyzer.h"
#include "A4PreDetector.h"
#include "A4PreciseDetector.h"


class A4Matcher
{
	A4MemoryBank *memoryBankPImpl;
	BorderAnalyzer *borderAnalyzerPImpl;
	A4PreDetector *preDetectorPImpl;
	A4PreciseDetector *preciseDetectorPImpl;

	void clearMemory();

public:

	void initMemory(CvSize size, int resizeFactor);
	void setAndAnalyseImage(IplImage *aimage);
	void clearResults();

	std::list<A4PreDetectedRecord> &getPreResults();
	std::list<A4PreciseDetectedRecord> &getPreciseResults();
	
	A4Matcher();
	~A4Matcher(void);
};


#endif
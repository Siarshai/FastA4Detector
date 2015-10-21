#ifndef A4PRECISEDETECTOR
#define A4PRECISEDETECTOR


#include "Utils.h"
#include "LocalHoughTransformer.h"
#include "A4MemoryBank.h"
#include "A4InfoStructures.h"


class A4PreciseDetector
{
	LocalHoughTransformer localHoughTransformer;
	CvPoint findPreciseBorderAlignedLinesFindLineSubroutine(unsigned char *dataOrigin, int step, int xOffset, int yOffset);
public:
	std::list<A4PreciseDetectedRecord> A4PreciseDetected;
	void detect(A4MemoryBank *memoryBank, std::list<A4PreDetectedRecord> &A4PreDetected);
};


#endif //A4PRECISEDETECTOR
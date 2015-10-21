#ifndef A4PRECISEDETECTOR
#define A4PRECISEDETECTOR

#include "APreciseDetector.h"
#include "LocalHoughTransformer.h"


class A4PreciseDetector : public APreciseDetector
{
	LocalHoughTransformer localHoughTransformer;
	CvPoint findPreciseBorderAlignedLinesFindLineSubroutine(unsigned char *dataOrigin, int step, int xOffset, int yOffset);
public:
	void detect(A4MemoryBank *memoryBank, const std::list<A4PreDetectedRecord> &A4PreDetected);
};


#endif //A4PRECISEDETECTOR
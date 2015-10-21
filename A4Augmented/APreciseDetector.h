#ifndef APRECISEDETECTOR
#define APRECISEDETECTOR


#include "Utils.h"
#include "A4MemoryBank.h"
#include "A4InfoStructures.h"


class APreciseDetector
{
protected:
	std::list<A4PreciseDetectedRecord> A4PreciseDetected;
public:
	const std::list<A4PreciseDetectedRecord> &getResults() { return A4PreciseDetected; }
	void clear() { A4PreciseDetected.clear(); }
	virtual ~APreciseDetector() {}
	virtual void detect(A4MemoryBank *memoryBank, const std::list<A4PreDetectedRecord> &A4PreDetected) = 0;
};

#endif //APRECISEDETECTOR
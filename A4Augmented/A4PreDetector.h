#ifndef A4PREDETECTOR
#define A4PREDETECTOR

#include "Utils.h"
#include "A4MemoryBank.h"
#include "A4InfoStructures.h"

using std::max;
using std::min;

class A4PreDetector
{
	void normalizePoints(A4MemoryBank *memoryBank);
public:
	std::list<A4PreDetectedRecord> A4PreDetected;
	void detect(A4MemoryBank *memoryBank);
};


#endif //A4PREDETECTOR
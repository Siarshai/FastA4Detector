#ifndef A4PREDETECTOR
#define A4PREDETECTOR

#include "APreDetector.h"

class A4PreDetector : public APreDetector
{
	void normalizePoints(A4MemoryBank *memoryBank);
public:
	void detect(A4MemoryBank *memoryBank);
};


#endif //A4PREDETECTOR
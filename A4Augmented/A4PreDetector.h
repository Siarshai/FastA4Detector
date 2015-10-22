#ifndef A4PREDETECTOR
#define A4PREDETECTOR

#include "APreDetector.h"

class A4PreDetector : public APreDetector
{
	int maxBordersInside;
	void normalizePoints(A4MemoryBank *memoryBank);
public:
	A4PreDetector(int amaxBordersInside);
	void detect(A4MemoryBank *memoryBank);
};


#endif //A4PREDETECTOR
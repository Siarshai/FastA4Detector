#ifndef APREDETECTOR
#define APREDETECTOR

#include "Utils.h"
#include "A4MemoryBank.h"
#include "A4InfoStructures.h"

class APreDetector
{
protected:
	std::list<A4PreDetectedRecord> A4PreDetected;
public:
	const std::list<A4PreDetectedRecord> &getResults() { return A4PreDetected; }
	void clear() { A4PreDetected.clear(); }
	virtual ~APreDetector() {}
	virtual void detect(A4MemoryBank *memoryBank) = 0;
};

#endif //APREDETECTOR
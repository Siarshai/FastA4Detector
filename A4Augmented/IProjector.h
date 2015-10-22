#ifndef IPROJECTOR
#define IPROJECTOR

#include "Utils.h"
#include "A4InfoStructures.h"


class IProjector
{
public:
	virtual ~IProjector() {}
	virtual void project(IplImage* dst, A4PreciseDetectedRecord dstRecord) = 0;
};
	
#endif //IPROJECTOR
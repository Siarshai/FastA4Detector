#ifndef IGRABBER
#define IGRABBER

#include "Utils.h"
#include "A4InfoStructures.h"
#include <memory>


class IGrabber
{
public:
	virtual ~IGrabber() {}
	virtual void grab(IplImage* src, A4PreciseDetectedRecord dstRecord) = 0;
};


#endif //IGRABBER
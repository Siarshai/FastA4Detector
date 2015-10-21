#ifndef IMAGEPROCESSOR
#define IMAGEPROCESSOR

#include "Utils.h"
#include "A4InfoStructures.h"
#include "A4MemoryBank.h"
#include <memory>


class IImageSplitter
{
public:
	virtual void process(IplImage *src, A4MemoryBank* dstDissected) = 0;
};


class IImageProcessor
{
public:
	virtual void process(A4MemoryBank* src) = 0;
};


#endif //IMAGEPROCESSOR
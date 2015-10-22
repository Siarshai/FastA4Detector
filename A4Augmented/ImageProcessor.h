#ifndef IMAGEPROCESSOR
#define IMAGEPROCESSOR

#include "Utils.h"
#include "A4InfoStructures.h"
#include "A4MemoryBank.h"
#include <memory>


class IImageProcessor
{
public:
	virtual ~IImageProcessor() {}
	virtual void process(A4MemoryBank* src) = 0;
};


#endif //IMAGEPROCESSOR
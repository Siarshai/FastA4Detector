#ifndef BASICIMAGESPLITTER
#define BASICIMAGESPLITTER

#include "ImageProcessor.h"

class BasicImageSplitter : public IImageSplitter
{
public:
	void process(IplImage *src, A4MemoryBank* dstDissected);
};


#endif //BASICIMAGESPLITTER
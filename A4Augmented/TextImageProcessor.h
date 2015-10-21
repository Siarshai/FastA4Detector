#ifndef TEXTIMAGEPROCESSOR
#define TEXTIMAGEPROCESSOR

#include "ImageProcessor.h"

class TextImageProcessor : public IImageProcessor
{
public:
	void process(A4MemoryBank* src);
};


#endif //TEXTIMAGEPROCESSOR
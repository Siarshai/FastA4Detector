#ifndef TEXTREMOVER
#define TEXTREMOVER

#include "ImageProcessor.h"


class TextRemover : public IImageProcessor
{
	LocalHistogrammAnalyzer histAnalyzer;
	int radius;
	void processChannel(IplImage* channel, int step);
public:
	TextRemover(int aradius) : radius(aradius) {}
	void process(A4MemoryBank* src);
};


#endif //TEXTREMOVER
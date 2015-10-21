#include "stdafx.h"
#include "TextImageProcessor.h"


void TextImageProcessor::process(A4MemoryBank* src)
{
	cvThreshold( src->redChannel, src->redChannel, 255, 255, CV_THRESH_OTSU | CV_THRESH_BINARY );
	cvThreshold( src->greenChannel, src->greenChannel, 255, 255, CV_THRESH_OTSU | CV_THRESH_BINARY );
	cvThreshold( src->blueChannel, src->blueChannel, 255, 255, CV_THRESH_OTSU | CV_THRESH_BINARY );
}
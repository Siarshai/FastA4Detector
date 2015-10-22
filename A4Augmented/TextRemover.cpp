#include "stdafx.h"
#include "TextRemover.h"


void TextRemover::process(A4MemoryBank* src)
{
	processChannel(src->redChannelResized, src->redChannelResized->widthStep);
	processChannel(src->greenChannelResized, src->greenChannelResized->widthStep);
	processChannel(src->blueChannelResized, src->blueChannelResized->widthStep);
}



void TextRemover::processChannel(IplImage* channel, int step)
{
	uchar *data = (uchar*)channel->imageData;
	int pix, subpix;
	uchar cpix;
	bool isDetected;
	for(int i = radius; i < channel->height - radius; ++i) 
	{
		for(int j = radius; j < channel->width - radius; ++j)
		{
			pix = 0;
			isDetected = true;
			for(int k = -1; k <= 1; ++k)
			{
				if(isDetected)
				{
					for(int l = -1; l <= 1; ++l)
					{
						if( (subpix = histAnalyzer.analyze(data + step*(i + k*radius) + j + l*radius, radius, radius, step, 50, radius, radius)) != -1 )
							pix += subpix;
						else
						{
							isDetected = false;
							break;
						}
					}
				}
				else
					break;
			}
			if(isDetected)
			{
				cpix = pix / 9;
				for(int k = radius; k < 2*radius; ++k)
				{
					for(int l = radius; l < 2*radius; ++l)
					{
						data[step*i + j] = std::max(data[step*i + j], cpix);
					}
				}
			}
		}
	}
}
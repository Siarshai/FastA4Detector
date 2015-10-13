#include "stdafx.h"
#include "LocalHoughTransformer.h"

LocalHoughTransformer::LocalHoughTransformer() {}


void LocalHoughTransformer::setPictureSpace(unsigned char *apictureSpace)
{
	pictureSpace = apictureSpace;
	clear();
}

void LocalHoughTransformer::reset(int awidth, int aheight, unsigned char *apictureSpace)
{
	width = awidth;
	height = aheight;
	pictureSpace = apictureSpace;
	clear();
}


void LocalHoughTransformer::fullReset(int aminAngle, int amaxAngle, int awidth, int aheight, int astep, unsigned char *apictureSpace)
{
	width = awidth;
	height = aheight;
	pictureSpace = apictureSpace;
	minAngleGlob = aminAngle;
	maxAngleGlob = amaxAngle;
	step = astep;
	maxRhoGlob = static_cast<int>( sqrtf(awidth*awidth + aheight*aheight) );
	angleRangeGlob = maxAngleGlob - minAngleGlob;
	parameterSpace.resize(2*maxRhoGlob*angleRangeGlob);
	clear();
}

CvPoint LocalHoughTransformer::analyze()
{
	int maxVal, maxAlpha, maxRho;
	maxVal = maxAlpha = maxRho = -1;
	for(int x = 0; x < width; ++x)
	{
		for(int y = 0; y < height; ++y)
		{
			int pix = pictureSpace[y*step + x];
			if(pix != 0)
			{
				for(int alpha = minAngleGlob; alpha < maxAngleGlob; ++alpha) 
				{
					int rhoMod = static_cast<int>( -x*sin(alpha*M_PI/180.0) + y*cos(alpha*M_PI/180.0) ) + maxRhoGlob; // TODO: without maxRhoGlob
					int alphaMod = alpha - minAngleGlob;
					for(int i = -2; i <= 2; ++i) //-2 2
					{
						for(int j = 2; j <= 2; ++j)
						{
							if(alphaMod + i >= 0 && alphaMod + i < angleRangeGlob && rhoMod + j >= 0 && rhoMod + j < 2*maxRhoGlob)
							{
								parameterSpace[alphaMod*2*maxRhoGlob + rhoMod] += 3 - max( abs(i), abs(j) );
								if(maxVal < parameterSpace[alphaMod*2*maxRhoGlob + rhoMod])
								{
									maxVal = parameterSpace[alphaMod*2*maxRhoGlob + rhoMod];
									maxAlpha = alphaMod;
									maxRho = rhoMod;
								}
							}
						}
					}
				}
			}
		}
	}
	if(maxVal == -1)
	{
		throw new std::exception("No lines detected");
	}
	clear();
	return cvPoint(maxAlpha + minAngleGlob, maxRho - maxRhoGlob);
}

	
void LocalHoughTransformer::clear()
{
	for(int alpha = 0; alpha < angleRangeGlob; ++alpha) 
	{
		for(int rhoMod = 0; rhoMod < 2*maxRhoGlob; ++rhoMod)
		{
			parameterSpace[alpha*2*maxRhoGlob + rhoMod] = 0;
		}
	}
}
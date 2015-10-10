#ifndef LOCAL_HOUGH_TRANSFORMER
#define LOCAL_HOUGH_TRANSFORMER

#include "Utils.h"
#include <vector>

using std::vector;
using std::max;

class LocalHoughTransformer
{
public:
	LocalHoughTransformer(int aminAngle, int amaxAngle, int awidth, int aheight, int astep, char *apictureSpace) : 
		minAngleGlob(aminAngle), maxAngleGlob(amaxAngle), 
		width(awidth), height(aheight), step(astep), pictureSpace(apictureSpace)
	{
		maxRhoGlob = static_cast<int>( sqrtf(awidth*awidth + aheight*aheight) );
		angleRangeGlob = maxAngleGlob - minAngleGlob;
		parameterSpace.resize(2*maxRhoGlob*angleRangeGlob);
	}
	CvPoint analyze()
	{
		
                    //rho = int(-i*sin(alpha*pi/180.0) + j*cos(alpha*pi/180.0)) + max_rho
                    //hough_data[alpha][rho] += abs_gradient
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
						int rhoMod = static_cast<int>( -x*sin(alpha*M_PI/180.0) + y*cos(alpha*M_PI/180.0) + maxRhoGlob );
						int alphaMod = alpha - minAngleGlob;
						for(int i = -2; i <= 2; ++i)
						{
							for(int j = -2; j <= 2; ++j)
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
		return cvPoint(maxAlpha + minAngleGlob, maxRho - maxRhoGlob);
	}
private:
	int minAngleGlob, maxAngleGlob, maxRhoGlob, angleRangeGlob;
	int width, height;
	int step;
	char *pictureSpace;
	vector<int> parameterSpace;
};


#endif



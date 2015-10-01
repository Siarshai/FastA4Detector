#include "stdafx.h"
#include "Utils.h"

//Thresholding functions, trimming dark pixels
double darknessThresholdPenalty(unsigned char R, unsigned char G, unsigned char B)
{
	const int thresholdHi = 330;
	const int thresholdMid = 210;
	const int thresholdLo = 150;
	const float midpoint = 0.75f;
	float sum = static_cast<float>( R + G + B );
	if (sum > thresholdHi)
		return 1.0f;
	else if (sum < thresholdLo)
		return 0.0f;
	else if (sum > thresholdMid)
		return (1 - midpoint)*(sum - thresholdMid)/(thresholdHi - thresholdMid) + midpoint;
	return midpoint*(sum - thresholdLo)/(thresholdMid - thresholdLo);
}

//Non-linear function making colorful and dark pixels become even darker
double darkness(unsigned char R, unsigned char G, unsigned char B)
{
	//float sum = R + G + B;
	return 1 + sqrtf( static_cast<float>((255-R))*(255-R) + static_cast<float>((255-G))*(255-G) + static_cast<float>((255-B))*(255-B) )/255; //(3*255 - sum)*(3*255 - sum)/2/255/255;
}

double colorfulness(unsigned char R, unsigned char G, unsigned char B)
{
	return ((B - G)*(B - G) + (G - R)*(G - R) + (R - B)*(R - B))/120.0f;
}

double matchForegroundTrimBlack(unsigned int pixel, unsigned int meanForeground, double standartDeviation) 
{
	if(pixel > meanForeground - 2*standartDeviation)
		return 1;
	else if (pixel < meanForeground - 3*standartDeviation)
		return 0;
	else {
		return (pixel - (meanForeground - 3*standartDeviation))/standartDeviation;
	}
}

unsigned int distanceManhattan(CvPoint pt1, CvPoint pt2)
{
	return abs(pt1.x - pt2.x) + abs(pt1.y - pt2.y);
}

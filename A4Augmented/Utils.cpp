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


CvPoint lineIntersection(double a1, double b1, double c1, double a2, double b2, double c2)
{
	double det = a1*b2 - b1*a2;
	double det1 = c1*b2 - b1*c2;
	double det2 = a1*c2 - c1*a2;
	double x = det1/det;
	double y = det2/det;
	return cvPoint(x, y);
}

CvPoint lineIntersection(double alpha1, double rho1, double alpha2, double rho2)
{
	return lineIntersection( sin(alpha1), -cos(alpha1), rho1,  sin(alpha2), -cos(alpha2), rho2); 
}

bool horizontalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step)
{
	int borderPixels1b = pieceOfII(dataII, x,				   y, searchWidth, searchHeight, step); 
	int borderPixels2b = pieceOfII(dataII, x +   searchWidth,  y, searchWidth, searchHeight, step);
	int borderPixels3b = pieceOfII(dataII, x + 2*searchWidth,  y, searchWidth, searchHeight, step);
	int borderPixels4b = pieceOfII(dataII, x + 3*searchWidth,  y, searchWidth, searchHeight, step);
	return blockThreshold(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, searchWidth*255/searchHeight*4/5);
}


bool verticalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step)
{
	int borderPixels1b = pieceOfII(dataII, x,  y,					searchWidth, searchHeight, step); 
	int borderPixels2b = pieceOfII(dataII, x,  y + searchHeight,    searchWidth, searchHeight, step);
	int borderPixels3b = pieceOfII(dataII, x,  y + 2*searchHeight,  searchWidth, searchHeight, step);
	int borderPixels4b = pieceOfII(dataII, x,  y + 3*searchHeight,  searchWidth, searchHeight, step);
	return blockThreshold(borderPixels1b, borderPixels2b, borderPixels3b, borderPixels4b, searchWidth*255/searchHeight*4/5);
}


std::pair<CvPoint, CvPoint> fromLineToTwoPoints(CvPoint& line, int frameWidth, int frameHeight)
{
	int alpha = line.x;
	int rho = -line.y;
	CvPoint pt1;
	CvPoint pt2;
	if(alpha == 0) 
	{
		pt1 = cvPoint(0, -rho);
		pt2 = cvPoint(frameWidth, -rho); 
	} else if(alpha == 90)
	{
		pt1 = cvPoint(rho, 0);
		pt2 = cvPoint(rho, frameHeight);
	} else
	{
		pt1 = cvPoint( rho/sin(alpha*M_PI/180.0), 0);
		pt2 = cvPoint( (rho + cos(alpha*M_PI/180.0)*frameHeight)/sin(alpha*M_PI/180.0), frameHeight);
	}
	return std::pair<CvPoint, CvPoint>(pt1, pt2);
}
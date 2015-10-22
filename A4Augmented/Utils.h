#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <vector>
#include <tuple>

double darknessThresholdPenalty(unsigned char R, unsigned char G, unsigned char B);
double darkness(unsigned char R, unsigned char G, unsigned char B);
double colorfulness(unsigned char R, unsigned char G, unsigned char B);
double matchForegroundTrimBlack(unsigned int pixel, unsigned int meanForeground, double standartDeviation);
unsigned int distanceManhattan(CvPoint pt1, CvPoint pt2);
CvPoint lineIntersection(double a1, double b1, double c1, double a2, double b2, double c2);
CvPoint lineIntersection(double alpha1, double rho1, double alpha2, double rho2);
bool horizontalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step);
bool verticalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step);
std::pair<CvPoint, CvPoint> fromLineToTwoPoints(CvPoint& line, int frameWidth, int frameHeight);

template<typename T>
T pieceOfII(T *data, int x, int y, int searchWidth, int searchHeight, int step)
{
	return data[(y + searchHeight)*step + (x + searchWidth)] - 
		data[y*step + (x + searchWidth)] -
		data[(y + searchHeight)*step + x] +
		data[y*step + x];
}

template<typename T>
bool blockThreshold(T val1, T val2, T val3, T val4, T threshold)
{
	int res = 0;
	if(val1 >= threshold)
		++res;
	if(val2 >= threshold)
		++res;
	if(val3 >= threshold)
		++res;
	if(val4 >= threshold)
		++res;
	if(res >= 3)
		return true;
	return false;
}

template <typename T>
unsigned int sumOfAbsoluteValues(T r, T g, T b)
{
	return abs(r) + abs(g) + abs(b);
}

template <typename T>
int secondDerivative(T val0, T val1, T val2, T val3, T val4)
{
	return (val0 + 8*val1 - 18*val2 + 8*val3 + val4)/12;
}

template <typename T>
int firstDerivativeSecondPrecision(T val0, T val1, T val2, T val3, T val4)
{
	return (val0 - 9*val1 + 9*val3 - val4)/16;
}

template <typename T>
int firstDerivative(T val0, T val2)
{
	return (static_cast<int>(val2) - static_cast<int>(val0))/2;
}

template <typename T>
int firstDerivativeGradual(T val0, T val1, T val2, T val3, T val4)
{
	return (val4 + val3 - val1 - val0)/4;
}

template <typename T>
double generalSlopeFunction(T x, T loVal, T loThreshold, T hiVal, T hiThreshold) {
	if(x > hiThreshold)
		return hiVal;
	else if (x < loThreshold)
		return loVal;
	else {
		return (hiVal - loVal)*(x - loThreshold)/(hiThreshold - loThreshold) + loVal;
	}
}

//TODO: It can be easily optimized
class LocalHistogrammAnalyzer
{
public:
	int hist[256];
	int histI[256];
	bool isHistIValid;
	LocalHistogrammAnalyzer()
	{
		discard();
	}
	void discard() 
	{
		for(int i = 0; i < 256; ++i)
		{
			hist[i] = 0;
			histI[i] = 0;
		}
		isHistIValid = false;
	}
	void makeHist(uchar *data, int height, int width, int step)
	{
		isHistIValid = false;
		for(int i = 0; i < height; ++i)
		{
			for(int j = 0; j < width; ++j)
			{
				++hist[ *(data + step*i + j) ];
			}
		}
	}
	void makeHistI()
	{
		isHistIValid = true;
		histI[0] = hist[0];
		for(int i = 1; i < 256; ++i)
		{
			histI[i] = histI[i-1] + hist[i];
		}
	}
	//returns color of most left pixel from right side of gap
	int gapSearch(int gapSize, int gapThreshold, int clusterThreshold)
	{
		if(isHistIValid == true)
			makeHistI();
		for(int i = 255; i > gapSize; --i)
		{
			if( (histI[i] - histI[i-gapSize] < gapThreshold) && (histI[255] - histI[i] > clusterThreshold) && (histI[i-gapSize] - histI[0] > clusterThreshold)) 
			{
				return i;
			}
		}
		return -1;
	}
	
	int analyze(uchar *data, int height, int width, int step, int gapSize, int gapThreshold, int clusterThreshold)
	{
		discard();
		makeHist(data, height, width, step);
		makeHistI();
		return gapSearch(gapSize, gapThreshold, clusterThreshold);
	}
};

#endif // UTILS_H
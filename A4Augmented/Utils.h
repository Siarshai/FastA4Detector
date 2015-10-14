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


double darknessThresholdPenalty(unsigned char R, unsigned char G, unsigned char B);
double darkness(unsigned char R, unsigned char G, unsigned char B);
double colorfulness(unsigned char R, unsigned char G, unsigned char B);
double matchForegroundTrimBlack(unsigned int pixel, unsigned int meanForeground, double standartDeviation);
unsigned int distanceManhattan(CvPoint pt1, CvPoint pt2);
CvPoint lineIntersection(double a1, double b1, double c1, double a2, double b2, double c2);
CvPoint lineIntersection(double alpha1, double rho1, double alpha2, double rho2);
bool horizontalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step);
bool verticalBlockPassII(int *dataII, int x, int y, int searchWidth, int searchHeight, int step);

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

#endif // UTILS_H
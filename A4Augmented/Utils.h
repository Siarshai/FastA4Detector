#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/contrib/contrib.hpp"

double darknessThresholdPenalty(unsigned char R, unsigned char G, unsigned char B);
double darkness(unsigned char R, unsigned char G, unsigned char B);
double colorfulness(unsigned char R, unsigned char G, unsigned char B);
double matchForegroundTrimBlack(unsigned int pixel, unsigned int meanForeground, double standartDeviation);

unsigned int distanceManhattan(CvPoint pt1, CvPoint pt2);


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
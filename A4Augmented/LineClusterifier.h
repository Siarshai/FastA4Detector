#ifndef LineClusterifier_H
#define LineClusterifier_H

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

#include "PointClusterifier.h"

using namespace std;


class LineClusterifier
{
public:
	//Somewhat forward declaring
	struct LineClusterRecord {
		float phi;
		float rho;
		int weight;
		LineClusterRecord(float aphi, float arho, int aweight): phi(aphi), rho(arho), weight(aweight) {}
	};

private:
	PointClusterifier pointClusterifier;

	IplImage* image;

	float phiVariance;
	float rhoVariance;

	bool classifyLine(float phi, float rho, list<LineClusterRecord>& cluster);
	void lineIntersection(float phi1, float rho1, float phi2, float rho2);
	
public:
	
	list<LineClusterRecord> horizontalLineClusters;
	list<LineClusterRecord> verticalLineClusters;
	list<CvPoint> intersections;
	void analyseImage(IplImage *aimage);
	void dump();
	LineClusterifier(float aphiVariance, float arhoVariance);
	~LineClusterifier(void);
};


#endif LineClusterifier_H
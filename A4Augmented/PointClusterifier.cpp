#include "stdafx.h"
#include "PointClusterifier.h"


PointClusterifier::PointClusterifier(float aresolutionDistance) : resolutionDistance(aresolutionDistance)
{
}


PointClusterifier::~PointClusterifier(void)
{
}


void PointClusterifier::clasterifyList(std::list<CvPoint>& pointsList, short xbias, short ybias)
{
	bool foundFlag = false;
	float key;

	for(std::list<CvPoint>::const_iterator itpt = pointsList.begin(); itpt != pointsList.end(); ++itpt)
	{
		const CvPoint& pt = *itpt;
		for(std::list<PointClusterRecord>::iterator itcr = tmpPoints.begin(); itcr != tmpPoints.end(); ++itcr) 
		{
			PointClusterRecord& cr = *itcr;
			if( ( abs(pt.x - cr.meanx) < resolutionDistance )&&( abs(pt.y - cr.meany) < resolutionDistance ) )
			{
				cr.meanx = (cr.meanx*cr.weight + pt.x)/(cr.weight + 1);
				cr.meany = (cr.meany*cr.weight + pt.y)/(cr.weight + 1);
				++cr.weight;
				key = xbias*pt.x + ybias*pt.y;
				if(key > cr.actualkey) {
					cr.actualkey = key;
					cr.actualx = pt.x;
					cr.actualy = pt.y;
				}
				foundFlag = true;
				break;
			}
		}
		if(!foundFlag) 
			tmpPoints.push_back(PointClusterRecord(pt, 1));
		else
			foundFlag = false;
	}

	pointsList.clear();
	//if(xbias == 0 && ybias == 0) {
		for(std::list<PointClusterRecord>::const_iterator itcr = tmpPoints.begin(); itcr != tmpPoints.end(); ++itcr) 
			pointsList.push_back( cvPoint( static_cast<int>((*itcr).actualx), static_cast<int>((*itcr).actualy) ) );
	/*} else {
		for(std::list<PointClusterRecord>::iterator itcr = tmpPoints.begin(); itcr != tmpPoints.end(); ++itcr) {
			int newx = static_cast<int>(  (*itcr).actualx + xbias*ceil( sqrtf( static_cast<float>((*itcr).weight) ) )/2  );
			int newy = static_cast<int>(  (*itcr).actualy + ybias*ceil( sqrtf( static_cast<float>((*itcr).weight) ) )/2  );
			pointsList.push_back( cvPoint( newx, newy ) );
		}
	}*/
	tmpPoints.clear();
}
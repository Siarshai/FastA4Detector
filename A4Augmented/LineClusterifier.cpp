#include "stdafx.h"
#include "LineClusterifier.h"


LineClusterifier::LineClusterifier(float aphiVariance, float arhoVariance): phiVariance(aphiVariance), rhoVariance(arhoVariance), pointClusterifier(5.0f)
{
}


LineClusterifier::~LineClusterifier(void)
{
}

void LineClusterifier::analyseImage(IplImage *aimage)
{	
	horizontalLineClusters.clear();
	verticalLineClusters.clear();
	intersections.clear();

	const float linePhiDelta = static_cast<float>( M_PI/12 );
	
	image = cvCreateImage(cvGetSize(aimage), IPL_DEPTH_8U, 1);
	//image = aimage;
	//IplImage *contours = cvCreateImage(cvGetSize(aimage), IPL_DEPTH_8U, 1);
	cvCanny(aimage, image, 75, 200);

	CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
	lines = cvHoughLines2( image, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 80);
		
	printf("lines %d\n", lines->total);

    for(int i = 0; i < lines->total; i++ ) 
	{
		//TODO: use cv::partition
        float* line = (float*)cvGetSeqElem(lines, i); 
		float phi = line[1];
		if( (phi > M_PI - linePhiDelta) || (phi < linePhiDelta)) 
		{
			//vertical (!) line
			classifyLine(line[1], line[0], verticalLineClusters);
		} else if( (phi > M_PI/2 - linePhiDelta) && (phi < M_PI/2 + linePhiDelta) )
		{
			//horizontal (!) line
			classifyLine(line[1], line[0], horizontalLineClusters);
		}
	}

	printf("verical clusters %d\n", verticalLineClusters.size());
	printf("horizontal clusters %d\n", horizontalLineClusters.size());

	//Populating intersections' list
    for(list<LineClusterRecord>::iterator ithor = horizontalLineClusters.begin(); ithor != horizontalLineClusters.end(); ++ithor ) 
	{
		float thetaHor = (*ithor).phi;
		float rhoHor = (*ithor).rho;

		for(list<LineClusterRecord>::iterator itver =  verticalLineClusters.begin(); itver != verticalLineClusters.end(); ++itver ) 
		{
			float thetaVer = (*itver).phi;
			float rhoVer = (*itver).rho;
			lineIntersection(thetaHor, rhoHor, thetaVer, rhoVer);
		}
	}
	
	printf("intersections %d\n", intersections.size());

	pointClusterifier.clasterifyList(intersections);

	printf("intersections clusters %d\n", intersections.size());
	
	dump();
	
	// Clear the memory storage which was used before
	cvClearMemStorage(storage);

	// Release memory
	cvReleaseMemStorage(&storage);
}


void LineClusterifier::lineIntersection(float phi1, float rho1, float phi2, float rho2)
{
	float a1 = cos(phi1), b1 = sin(phi1); 
	float a2 = cos(phi2), b2 = sin(phi2); 
	float detMain = a1*b2 - b1*a2;
	if( abs(detMain) < 0.000001 )
		return; //parallels (it should not happen though)
	float det1 = rho1*b2 - b1*rho2;
	float det2 = a1*rho2 - rho1*a2;
	CvPoint pt;
	pt.x = static_cast<int>(det1/detMain);
	pt.y = static_cast<int>(det2/detMain);
	intersections.push_back(pt);
}

void LineClusterifier::dump()
{
    for(list<LineClusterRecord>::iterator it = horizontalLineClusters.begin(); it != horizontalLineClusters.end(); ++it ) 
	{
		float theta = (*it).phi;
		float rho = (*it).rho;

        CvPoint pt1, pt2; 
        double a = cos(theta), b = sin(theta); 
        double x0 = a*rho, y0 = b*rho; 
        pt1.x = cvRound(x0 + 1000*(-b)); 
        pt1.y = cvRound(y0 + 1000*(a)); 
        pt2.x = cvRound(x0 - 1000*(-b)); 
        pt2.y = cvRound(y0 - 1000*(a)); 
        cvLine( image, pt1, pt2, CV_RGB(255, 255, 255), 2, 8 ); 
    } 
    for(list<LineClusterRecord>::iterator it = verticalLineClusters.begin(); it != verticalLineClusters.end(); ++it ) 
	{
		float theta = (*it).phi;
		float rho = (*it).rho;

        CvPoint pt1, pt2; 
        double a = cos(theta), b = sin(theta); 
        double x0 = a*rho, y0 = b*rho; 
        pt1.x = cvRound(x0 + 1000*(-b)); 
        pt1.y = cvRound(y0 + 1000*(a)); 
        pt2.x = cvRound(x0 - 1000*(-b)); 
        pt2.y = cvRound(y0 - 1000*(a)); 
        cvLine( image, pt1, pt2, CV_RGB(255, 255, 255), 2, 8 ); 
    } 

	for(list<CvPoint>::iterator it =  intersections.begin(); it != intersections.end(); ++it ) 
	{
		cvCircle(image,  cvPoint((*it).x, (*it).y), 5, CV_RGB(255, 255, 255), 3, 8, 0);
	}
	cvShowImage("image", image);
    //char c = cvWaitKey(1000000);
}


bool LineClusterifier::classifyLine(float phi, float rho, list<LineClusterRecord>& cluster)
{
	for(list<LineClusterRecord>::iterator it = cluster.begin(); it != cluster.end(); ++it) 
	{
		if( abs( (*it).phi - phi ) < phiVariance && abs( (*it).rho - rho ) < rhoVariance )
		{
			(*it).rho = ((*it).weight*(*it).rho + rho)/((*it).weight + 1);
			(*it).phi = ((*it).weight*(*it).phi + phi)/((*it).weight + 1);
			++(*it).weight;
			return true;
		}
	}
	cluster.push_back( LineClusterRecord(phi, rho, 1) );
	return false;
}
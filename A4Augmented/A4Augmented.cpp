#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <set>
#include "A4Matcher.h"
#include "LocalHoughTransformer.h"

//Pass an arbitrary big number to set camera's best resolution
#define MAXRESOLUTION 10000

#define FROM_PICTURES 1

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	#ifdef FROM_PICTURES
		char* imageNames[] = { 
			//"images/hou1.jpg", "images/hou2.jpg", "images/hou3.jpg", "images/hou4.jpg", "images/hou5.jpg", "images/hou6.jpg",
			"images/hou7.jpg", "images/hou8.jpg", "images/hou9.jpg", "images/hou10.jpg"
			/*
			"images/easy/1.jpg", "images/easy/2.jpg",
			"images/garageH/1.JPG", "images/garageH/2.jpg", 
			"images/garageH/3.jpg", "images/garageH/4.jpg", "images/garageH/5.jpg", "images/garageH/6.jpg",
			"images/garageV/1.jpg", "images/garageV/2.jpg", "images/garageV/3.jpg", "images/garageV/4.jpg", "images/garageV/5.jpg",
			"images/own/1.JPG", "images/own/2.JPG", "images/own/3.JPG", "images/own/4.JPG", "images/own/5.JPG", "images/own/6.JPG" */ };
		const int size = 4;
		int i = 0;
	#else
		CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); 
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, MAXRESOLUTION);
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, MAXRESOLUTION);
	#endif //FROM_PICTURES
	
	A4Matcher am;

    IplImage* frame = 0;

	while(true)
	{
		#ifdef FROM_PICTURES
			++i;
			i = i % size;
			frame = cvLoadImage(imageNames[i]);
			if (frame == NULL) {
				printf("Can not load static image\n");
				waitKey();
				exit(-1);
			}
		#else
			frame = cvQueryFrame( capture );
			if (!frame) 
			{
				cout << "Cannot read a frame from camera" << endl;
				break;
			}
		#endif //FROM_PICTURES
		
		LocalHoughTransformer lht(-30, 30, frame->width, frame->height, frame->widthStep, (char *)frame->imageData);
		CvPoint line = lht.analyze();
		int alpha = line.x;
		int rho = -line.y;
		printf("alpha %d, rho %d\n", alpha, rho);
		CvPoint pt1;
		CvPoint pt2;
		if(alpha == 0) 
		{
			pt1 = cvPoint(0, -rho);
			pt2 = cvPoint(frame->width, -rho); 
		} else if(alpha == 90)
		{
			pt1 = cvPoint(rho, 0);
			pt2 = cvPoint(rho, frame->height);
		} else
		{
			pt1 = cvPoint( rho/sin(alpha*M_PI/180.0), 0);
			pt2 = cvPoint( (rho + cos(alpha*M_PI/180.0)*frame->height)/sin(alpha*M_PI/180.0) , frame->height);
		}
		cvDrawLine(frame, pt1, pt2, CV_RGB(0, 255, 0), 1, 8, 0);

		/*
		am.setAndAnalyseImage(frame);
		
		for(std::list<A4Matcher::A4Record>::iterator it = am.foundA4.begin(); it != am.foundA4.end(); ++it) {
			cvDrawLine(frame, (*it).DL, (*it).DR, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).DR, (*it).UR, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).UR, (*it).UL, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).UL, (*it).DL, CV_RGB(0, 255, 0), 2, 8, 0);
		}
		
		for(std::list<A4Matcher::A4PreDetectedRecord>::iterator it = am.A4PreDetected.begin(); it != am.A4PreDetected.end(); ++it) {
			cvDrawRect(frame, (*it).ulpt, (*it).drpt, CV_RGB(0, 0, 255), 1, 8, 0);
			cvDrawRect(frame, (*it).ulptBorder, (*it).drptBorder, CV_RGB(0, 255, 255), 1, 8, 0);
		}
		*/

        cvShowImage("capture", frame);
		
		#ifdef FROM_PICTURES
			char c = cvWaitKey(100000);
		#else
			char c = cvWaitKey(10);
		#endif
        if (c == 27) { // ESC
			break;
        }
	}
	
	#ifndef FROM_PICTURES
		cvReleaseCapture( &capture );
	#endif //FROM_PICTURES

    return 0;
}
#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <set>
#include "A4Matcher.h"
#include "LocalHoughTransformer.h"
#include "A4Projector.h"


//Pass an arbitrary big number to set camera's best resolution
#define MAXRESOLUTION 10000

#define FROM_PICTURES 1

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	#ifdef FROM_PICTURES
		char* imageNames[] = { 
			/*
			"images/hou1.jpg", "images/hou2.jpg", "images/hou3.jpg", "images/hou4.jpg", "images/hou5.jpg", "images/hou6.jpg",
			"images/hou7.jpg", "images/hou8.jpg", "images/hou9.jpg", "images/hou10.jpg"  */
			//"images/easy/1.jpg", "images/easy/2.jpg",
			/*"images/clean/hou1.jpg", "images/clean/hou4.jpg", "images/clean/hou2.jpg", "images/clean/hou3.jpg", 
			"images/clean/hou5.jpg", "images/clean/hou6.jpg", "images/clean/hou7.jpg", "images/clean/hou8.jpg", */
			"images/garageH/1.JPG", "images/garageH/2.jpg", 
			"images/garageH/3.jpg", "images/garageH/4.jpg", 
			"images/garageH/5.jpg", "images/garageH/6.jpg",
			"images/garageV/1.jpg", "images/garageV/2.jpg", "images/garageV/3.jpg", "images/garageV/4.jpg", "images/garageV/5.jpg",
			"images/own/1.JPG", "images/own/2.JPG", "images/own/3.JPG", "images/own/4.JPG", "images/own/5.JPG", "images/own/6.JPG" };
		const int size = 17;
		int i = 0;
	#else
		CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); 
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, MAXRESOLUTION);
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, MAXRESOLUTION);
	#endif //FROM_PICTURES
	
	A4Matcher am;
	//SimpleImageProjector a4p("images/projection.jpg");
	std::list<pair<char*, int>> *text = new std::list<pair<char*, int>>();
	text->push_back(pair<char*, int>("Test Test Test", 5));
	text->push_back(pair<char*, int>("WorksWorksWorks", 10));
	text->push_back(pair<char*, int>("y a a a a a a a a a a a y", 15));
	TextAnimatedProjector a4p(text);

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
			
		am.setAndAnalyseImage(frame);
		
		for(std::list<A4PreDetectedRecord>::iterator it = am.A4PreDetected.begin(); it != am.A4PreDetected.end(); ++it) {
			cvDrawRect(frame, (*it).ulpt, (*it).drpt, CV_RGB(0, 0, 255), 1, 8, 0);
			cvDrawRect(frame, (*it).ulptBorder, (*it).drptBorder, CV_RGB(0, 255, 255), 1, 8, 0);
		}
		
		for(A4PreciseDetectedRecord pdr : am.A4PreciseDetected)
		{
			cvDrawCircle(frame, pdr.UR, 1, CV_RGB(255, 0, 0), 2, 8, 0);
			cvDrawCircle(frame, pdr.UL, 1, CV_RGB(255, 0, 0), 2, 8, 0);
			cvDrawCircle(frame, pdr.DL, 1, CV_RGB(255, 0, 0), 2, 8, 0);
			cvDrawCircle(frame, pdr.DR, 1, CV_RGB(255, 0, 0), 2, 8, 0);
			//cvDrawLine(frame, pt1, pt2, CV_RGB(0, 200, 0), 1, 8, 0);
			#ifdef DEBUG_LINES_INFORMATION
			auto twoPoints = fromLineToTwoPoints(pdr.lineHorDL, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineHorDR, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineHorUL, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineHorUR, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineVerDL, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineVerDR, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineVerUL, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			twoPoints = fromLineToTwoPoints(pdr.lineVerUR, frame->width, frame->height);
			cvDrawLine(frame, twoPoints.first, twoPoints.second, CV_RGB(0, 200, 0), 1, 8, 0);
			#endif DEBUG_LINES_INFORMATION
			
			a4p.findTransform(pdr);
			a4p.project(frame);
		}
		am.clearResults();

        cvShowImage("frame", frame);
		
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
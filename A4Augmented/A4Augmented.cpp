#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <set>
#include "A4Matcher.h"

//Pass an arbitrary big number to set camera's best resolution
#define MAXRESOLUTION 10000

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

	char* imageNames[] = { 
		//"images/easy/1.jpg", "images/easy/2.jpg",
		//"images/garageH/1.JPG", "images/garageH/2.jpg", 
		"images/garageH/3.jpg", "images/garageH/4.jpg", "images/garageH/5.jpg", "images/garageH/6.jpg",
		"images/garageV/1.jpg", "images/garageV/2.jpg", "images/garageV/3.jpg", "images/garageV/4.jpg", "images/garageV/5.jpg",
		"images/own/1.JPG", "images/own/2.JPG", "images/own/3.JPG", "images/own/4.JPG", "images/own/5.JPG", "images/own/6.JPG" };
	const int size = 15;
	/*
    IplImage *frame = cvLoadImage("4.JPG");
	if (frame == NULL) {
		printf("Can not load static image\n");
		waitKey();
		exit(-1);
	}
	*/
	//smoothing
	//IplImage *frame =  cvCreateImage(cvSize(tmp->width, tmp->height), IPL_DEPTH_8U, 3);
	//cvSmooth(tmp, frame, CV_BILATERAL, 5);
	//cvSmooth(tmp, frame, CV_GAUSSIAN, 5);
	

	A4Matcher am;
	
	/*
	CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); 
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, MAXRESOLUTION);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, MAXRESOLUTION);
	*/
    IplImage* frame = 0;
	int i = 0;

	while(true)
	{
		
		frame = cvLoadImage(imageNames[i]);
		if (frame == NULL) {
			printf("Can not load static image\n");
			waitKey();
			exit(-1);
		}

		/*
        frame = cvQueryFrame( capture );// read a new frame from video
		if (!frame) //if not success, break loop
		{
			cout << "Cannot read a frame from camera" << endl;
			break;
		}
		*/
		
		am.setAndAnalyseImage(frame);
		
		for(std::list<A4Matcher::A4Record>::iterator it = am.foundA4.begin(); it != am.foundA4.end(); ++it) {
			cvDrawLine(frame, (*it).DL, (*it).DR, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).DR, (*it).UR, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).UR, (*it).UL, CV_RGB(0, 255, 0), 2, 8, 0);
			cvDrawLine(frame, (*it).UL, (*it).DL, CV_RGB(0, 255, 0), 2, 8, 0);
		}
        cvShowImage("capture", frame);
		am.dump();
		
        char c = cvWaitKey(100000);
        if (c == 27) { // нажата ESC
			break;
        }

		++i;
		i = i % size;
	}

    //cvReleaseCapture( &capture );

    return 0;
}
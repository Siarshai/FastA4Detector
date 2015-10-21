#include "stdafx.h"
#include "A4Projector.h"


SimpleImageProjector::SimpleImageProjector(char* pathToProjection)
{
	projection = cvLoadImage(pathToProjection);
	if (projection == nullptr) {
		throw std::exception("Can not load projection image");
	}
	transformMat = cvCreateMat(3, 3, CV_32FC1);
	corners[0] = cvPoint2D32f(0, 0);
	corners[1] = cvPoint2D32f(projection->width-1, 0);
	corners[2] = cvPoint2D32f(0, projection->height-1);
	corners[3] = cvPoint2D32f(projection->width-1, projection->height-1);
}


SimpleImageProjector::~SimpleImageProjector()
{
	delete transformMat;
	cvReleaseImage(&projection);
}


void SimpleImageProjector::project(IplImage* dst, A4PreciseDetectedRecord dstRecord)
{
	CvPoint2D32f dstCorners[4];
	dstCorners[0] = cvPoint2D32f(dstRecord.UL.x, dstRecord.UL.y);
	dstCorners[1] = cvPoint2D32f(dstRecord.UR.x, dstRecord.UR.y);
	dstCorners[2] = cvPoint2D32f(dstRecord.DL.x, dstRecord.DL.y);
	dstCorners[3] = cvPoint2D32f(dstRecord.DR.x, dstRecord.DR.y);
	cvWarpPerspective(projection, dst, transformMat, CV_INTER_LINEAR);
}


TextAnimatedProjector::TextAnimatedProjector(std::list<std::pair<char*, int>> *atext)
{
	projection = cvCreateImage(cvSize(1748, 1240), 8, 3);
	if (projection == nullptr) {
		throw std::exception("Can not create projection image");
	}
    cvSet(projection, cvScalar(255, 255, 255));

	textAnchor = cvPoint( projection->width/2, projection->height/2 );
	color = CV_RGB(0, 0, 0);
	text = atext;
	currentText = text->begin();

    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 5.0, 5.0, 0, 3, CV_AA);
	cvGetTextSize((*currentText).first, &font, &textSize, &baseline);
	cvPutText(projection, (*currentText).first, cvPoint( projection->width/2 - textSize.width/2, projection->height/2 ), &font, color);
	//cvGetTextSize(const char* text_string, const CvFont* font, CvSize* text_size, int* baseline)

	transformMat = cvCreateMat(3, 3, CV_32FC1);
	corners[0] = cvPoint2D32f(0, 0);
	corners[1] = cvPoint2D32f(projection->width-1, 0);
	corners[2] = cvPoint2D32f(0, projection->height-1);
	corners[3] = cvPoint2D32f(projection->width-1, projection->height-1);
	ticks = 0;
}


TextAnimatedProjector::~TextAnimatedProjector()
{
	delete transformMat;
	delete text;
	cvReleaseImage(&projection);
}


void TextAnimatedProjector::changeCaption()
{
	++ticks;
	if(ticks > (*currentText).second)
	{
		++currentText;
		if(currentText == text->end())
			currentText = text->begin();
		cvSet(projection, cvScalar(255, 255, 255));
		cvGetTextSize((*currentText).first, &font, &textSize, &baseline);
		cvPutText(projection, (*currentText).first, cvPoint( projection->width/2 - textSize.width/2, projection->height/2 ), &font, color);
	}
}


void TextAnimatedProjector::project(IplImage* dst, A4PreciseDetectedRecord dstRecord)
{
	CvPoint2D32f dstCorners[4];
	dstCorners[0] = cvPoint2D32f(dstRecord.UL.x, dstRecord.UL.y);
	dstCorners[1] = cvPoint2D32f(dstRecord.UR.x, dstRecord.UR.y);
	dstCorners[2] = cvPoint2D32f(dstRecord.DL.x, dstRecord.DL.y);
	dstCorners[3] = cvPoint2D32f(dstRecord.DR.x, dstRecord.DR.y);
	transformMat = cvGetPerspectiveTransform(corners, dstCorners, transformMat);
	cvWarpPerspective(projection, dst, transformMat, CV_INTER_LINEAR);
	changeCaption();
}

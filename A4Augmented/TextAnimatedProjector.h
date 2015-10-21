#ifndef TEXTANIMATEDPROJECTOR
#define TEXTANIMATEDPROJECTOR

#include "IProjector.h"


class TextAnimatedProjector : public IProjector {
    IplImage* projection;
    CvFont font;
	CvSize textSize;
	int baseline;
	CvScalar color;
	CvPoint textAnchor;
	CvMat* transformMat;
	CvPoint2D32f corners[4]; //UL, UR, DL, DR;
	std::list<std::pair<char*, int>> *text;
	std::list<std::pair<char*, int>>::iterator currentText;
	int ticks;
	void changeCaption();
public:
	TextAnimatedProjector(std::list<std::pair<char*, int>> *atext);
	~TextAnimatedProjector();
	void project(IplImage* dst, A4PreciseDetectedRecord dstRecord);
};


#endif
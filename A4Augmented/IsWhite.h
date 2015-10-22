#ifndef ISWHITE
#define ISWHITE

#include "IPixelClassifier.h"

class IsWhite : public IPixelClassifier
{
public:
	IsWhite() {}
	float classify(uchar r, uchar g, uchar b) {}
};


#endif //ISWHITE
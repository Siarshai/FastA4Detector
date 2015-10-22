#ifndef IPIXELCLASSIFIER
#define IPIXELCLASSIFIER

#include "Utils.h"

class IPixelClassifier
{
public:
	virtual ~IPixelClassifier() {}
	virtual float classify(uchar r, uchar g, uchar b);
};


#endif //IPIXELCLASSIFIER
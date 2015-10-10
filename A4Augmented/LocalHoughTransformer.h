#ifndef LOCAL_HOUGH_TRANSFORMER
#define LOCAL_HOUGH_TRANSFORMER

#include "Utils.h"
#include <vector>

using std::vector;
using std::max;

class LocalHoughTransformer
{
public:
	LocalHoughTransformer(int aminAngle, int amaxAngle, int awidth, int aheight, int astep, unsigned char *apictureSpace);
	void reset(int awidth, int aheight, unsigned char *apictureSpace);
	CvPoint analyze();
	void clear();
private:
	int minAngleGlob, maxAngleGlob, maxRhoGlob, angleRangeGlob;
	int width, height;
	int step;
	unsigned char *pictureSpace;
	vector<int> parameterSpace;
};


#endif



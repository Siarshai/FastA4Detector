#ifndef AA4MATCHERGEARSFACTORY
#define AA4MATCHERGEARSFACTORY

#include "Utils.h"
#include "BorderAnalyzer.h"
#include "APreDetector.h"
#include "APreciseDetector.h"
#include "A4Matcher.h"

class A4MatcherGearsAbstractFactory
{
public:
	virtual ~A4MatcherGearsAbstractFactory() {}
	virtual BorderAnalyzer* getBorderAnalyzer() = 0;
	virtual APreDetector* getPreDetector() = 0;
	virtual APreciseDetector* getPreciseDetector() = 0;
	//TODO: Move to builder
	virtual A4Matcher* getMatcher() = 0;
};


#endif //AA4MATCHERGEARSFACTORY
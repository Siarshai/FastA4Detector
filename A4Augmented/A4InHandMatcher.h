#ifndef A4INHANDMATCHER
#define A4INHANDMATCHER

#include "Utils.h"
#include "A4MatcherGearsAbstractFactory.h"

class A4InHandMatcher : public A4MatcherGearsAbstractFactory
{
public:
	BorderAnalyzer* getBorderAnalyzer()
	{
		//TODO: make those constant
		return new BorderAnalyzer(44, LocalBorderAnalyzer(44/4, 10.0, 12.0, 90.0, 110.0, 130.0, 120.0*500.0, 120.0*1000.0, 120.0*1200.0, -0.2, 0.05, -0.2, 0.06));
	}
	APreDetector* getPreDetector() 
	{
		return new A4PreDetector(50);
	}
	APreciseDetector* getPreciseDetector() 
	{
		return new A4PreciseDetector();
	}
	//TODO: Move to builder
	A4Matcher* getMatcher() 
	{
		return new A4Matcher(
				getBorderAnalyzer(),
				getPreDetector(),
				getPreciseDetector(),
				4
			);
	}
};

#endif //A4INHANDMATCHER
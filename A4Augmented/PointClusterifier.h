#pragma once

class PointClusterifier
{
private:
	
	struct PointClusterRecord {
		float meanx;
		float meany;
		float actualx;
		float actualy;
		float actualkey;
		int weight;
		PointClusterRecord(CvPoint apt, int aweight): meanx(static_cast<float>(apt.x)), meany(static_cast<float>(apt.y)), actualx(static_cast<float>(apt.x)), actualy(static_cast<float>(apt.y)), actualkey(0), weight(aweight) {}
		PointClusterRecord(float ax, float ay, int aweight): meanx(ax), meany(ay), actualx(ax), actualy(ay), actualkey(0), weight(aweight) {}
		PointClusterRecord(int ax, int ay, int aweight): meanx(static_cast<float>(ax)), meany(static_cast<float>(ay)), actualx(static_cast<float>(ax)), actualy(static_cast<float>(ay)), actualkey(0), weight(aweight) {}
	};


	std::list<PointClusterRecord> tmpPoints;
	float resolutionDistance;

public:
	PointClusterifier(float aresolutionDistance);
	~PointClusterifier(void);
	void clasterifyList(std::list<CvPoint>& corners, short xbias = 0, short ybias = 0);
};


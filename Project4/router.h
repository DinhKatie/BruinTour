#ifndef ROUTER_H
#define ROUTER_H
#include <queue>
#include "base_classes.h"
#include "geodb.h"

class Router : public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db) : geodb(geo_db) {}
	virtual ~Router() {}
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
		const GeoPoint& pt2) const;

private:
	const GeoDatabaseBase& geodb;

	struct Cell {
		GeoPoint point;
		double f_score;
		Cell(const GeoPoint& p, double f) : point(p), f_score(f) {}
	};

	struct lowestFScore {
		bool operator()(const Cell& pt1, const Cell& pt2) const {
			return pt1.f_score > pt2.f_score;
		}
	};
	vector<GeoPoint> reconstructPath(HashMap<GeoPoint>& cameFrom, GeoPoint cur) const;
};

#endif

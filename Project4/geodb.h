#ifndef GEODB_H
#define GEODB_H

#include "base_classes.h"
#include "HashMap.h"
#include "geotools.h"
#include "geopoint.h"

class GeoDatabase : public GeoDatabaseBase
{
public:
	GeoDatabase();
	virtual ~GeoDatabase();
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi,
		GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
		const;
	virtual std::string get_street_name(const GeoPoint& pt1,
		const GeoPoint& pt2) const;

private:
	struct lineSeg {
		int numPois;
		GeoPoint p1;
		GeoPoint p2;
		lineSeg(int p, GeoPoint one, GeoPoint two) : numPois(p), p1(one), p2(two) {}
	};

	HashMap<GeoPoint> m_pointsOfInterest;
	HashMap<std::string> m_lineMap; //maps string reps of a line segment to its street name
	HashMap<vector<GeoPoint>> m_connectedPoints; //maps string rep of one geopoint to its connected ones

	//Creates midpoints on segments with POIs
	void forPOI(const lineSeg& ls, const string& street_name);
	//Creates paths for each POI
	void POIPath(const string& lat, const string& lon, const GeoPoint& pt1, const GeoPoint& pt2);
};
#endif
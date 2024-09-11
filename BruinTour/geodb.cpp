
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "geodb.h"
#include "geotools.h"

using namespace std;

GeoDatabase::GeoDatabase() {}

GeoDatabase::~GeoDatabase() {}

bool GeoDatabase::load(const std::string& map_data_file)
{
	ifstream inf(map_data_file);
	if (!inf)
	{
		cerr << "Error: Cannot open mapdata.txt!" << endl;
		return false;
	}

	string line;
	while (getline(inf, line))
	{
		string street_name, startPoint1, startPoint2, endPoint1, endPoint2;
		int nPois;
		string poiName;
		string p1, p2;

		//Get street name
		street_name = line;

		//Get line segment points
		getline(inf, line);
		istringstream iss(line);
		iss >> startPoint1 >> startPoint2 >> endPoint1 >> endPoint2;

		getline(inf, line);
		nPois = stoi(line);

		//Add to street
		lineSeg ls = lineSeg(nPois, GeoPoint(startPoint1, startPoint2), GeoPoint(endPoint1, endPoint2));
		if (nPois > 0)
			forPOI(ls, street_name);

		m_lineMap.insert(startPoint1 + "," + startPoint2 + " " + endPoint1 + "," + endPoint2, street_name);
		m_connectedPoints[startPoint1 + "," + startPoint2].push_back(GeoPoint(endPoint1, endPoint2));
		m_connectedPoints[endPoint1 + "," + endPoint2].push_back(GeoPoint(startPoint1, startPoint2));

		//Loop for each POI
		for (int i = 0; i < nPois; i++)
		{
			getline(inf, line, '|');
			poiName = line;
			getline(inf, line);
			iss.clear();
			iss.str(line);
			iss >> p1 >> p2;
			m_pointsOfInterest.insert(poiName, GeoPoint(p1, p2));
			POIPath(p1, p2, GeoPoint(startPoint1, startPoint2), GeoPoint(endPoint1, endPoint2));
		}
	}
	return true;
}

void GeoDatabase::forPOI(const lineSeg& ls, const string& street_name)
{
	GeoPoint middle = midpoint(ls.p1, ls.p2);

	m_lineMap.insert(ls.p1.to_string() + " " + middle.to_string(), street_name);
	m_lineMap.insert(ls.p2.to_string() + " " + middle.to_string(), street_name);

	m_connectedPoints[ls.p1.to_string()].push_back(middle);
	m_connectedPoints[middle.to_string()].push_back(GeoPoint(ls.p1.sLatitude, ls.p1.sLongitude));
	m_connectedPoints[ls.p2.to_string()].push_back(middle);
	m_connectedPoints[middle.to_string()].push_back(GeoPoint(ls.p2.sLatitude, ls.p2.sLongitude));
}

void GeoDatabase::POIPath(const string& lat, const string& lon, const GeoPoint& pt1, const GeoPoint& pt2)
{
	GeoPoint middle = midpoint(pt1, pt2);
	m_lineMap.insert(lat + "," + lon + " " + middle.to_string(), "a path");
	m_connectedPoints[lat + "," + lon].push_back(middle);
	m_connectedPoints[middle.to_string()].push_back(GeoPoint(lat, lon));
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	const GeoPoint* value = m_pointsOfInterest.find(poi);
	if (value == nullptr)
		return false;
	point = *value;
	return true;
}

string GeoDatabase::get_street_name(const GeoPoint& pt1,
	const GeoPoint& pt2) const
{
	string line = pt1.to_string() + " " + pt2.to_string();
	string line2 = pt2.to_string() + " " + pt1.to_string();
	const string* street = m_lineMap.find(line);
	const string* street2 = m_lineMap.find(line2);
	if (street == nullptr && street2 == nullptr)
		return "";
	else if (street == nullptr)
		return *street2;
	else if (street2 == nullptr)
		return *street;
	else if (*street == *street2)
		return *street;
	else return *street;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
	const std::vector<GeoPoint>* points = m_connectedPoints.find(pt.to_string());
	return *points;
}
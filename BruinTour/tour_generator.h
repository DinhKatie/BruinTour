#ifndef TOUR_H
#define TOUR_H

#include "base_classes.h"

class TourGenerator : public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geodb(geodb), m_route(router) {}
	virtual ~TourGenerator() {}
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
	const GeoDatabaseBase& m_geodb;
	const RouterBase& m_route;

	std::string findDirection(const GeoPoint& pt1, const GeoPoint& pt2) const;
	std::string findTurn(double degree) const;
};
#endif

#include <vector>
#include "tourcmd.h"
#include "stops.h"
#include "tour_generator.h"
#include "geotools.h"
using namespace std;

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
	vector<TourCommand> instruct;
	string startpoi, commentary;
	
	//Read the first line and output commentary
	if (stops.get_poi_data(0, startpoi, commentary))
	{
		TourCommand comm = TourCommand();
		comm.init_commentary(startpoi, commentary);
		instruct.push_back(comm);
	}
	else
		return instruct;

	//Continue with rest of lines
	GeoPoint gp1, gp2;
	string poi, desc;
	int i = 1;
	while (stops.get_poi_data(i, poi, desc))
	{
		if (!m_geodb.get_poi_location(startpoi, gp1) || !m_geodb.get_poi_location(poi, gp2))
			return instruct;
		//Begin routing
		vector<GeoPoint> path = m_route.route(gp1, gp2);

		for (int i = 0; i < path.size() - 1; i++)
		{
			//Calculate distance, direction, street name to create tour commands for each line segment
			double distance = distance_earth_miles(path[i], path[i + 1]);
			string dir = findDirection(path[i], path[i + 1]);
			string streetName = m_geodb.get_street_name(path[i], path[i + 1]);

			TourCommand proceed = TourCommand();
			proceed.init_proceed(dir, streetName, distance, path[i], path[i + 1]);
			instruct.push_back(proceed);

			//Check for turn, make sure GeoPoint[i+2] exists for comparison
			if (i + 2 < path.size())
			{
				//Check street name differences and angles
				if (m_geodb.get_street_name(path[i], path[i + 1]) != m_geodb.get_street_name(path[i + 1], path[i + 2]) &&
					angle_of_turn(path[i], path[i + 1], path[i + 2]) != 0)
				{
					//Streets with same directions should not have a turn
					if (findDirection(path[i], path[i + 1]) == findDirection(path[i + 1], path[i + 2])) continue;
					//Turn tour command
					string turn = findTurn(angle_of_turn(path[i], path[i + 1], path[i + 2]));
					TourCommand turnCmd = TourCommand();
					turnCmd.init_turn(turn, m_geodb.get_street_name(path[i + 1], path[i + 2]));
					instruct.push_back(turnCmd);
				}
			}
		}
		//Reached next point of interest
		TourCommand cmd = TourCommand();
		cmd.init_commentary(poi, desc);
		instruct.push_back(cmd);
		startpoi = poi;
		i++;
	}
	return instruct;
}

string TourGenerator::findDirection(const GeoPoint & pt1, const GeoPoint & pt2) const
{
	double angle = angle_of_line(pt1, pt2);

	if (angle < 22.5 && angle > 0)
		return "east";
	else if (angle < 67.5)
		return "northeast";
	else if (angle < 112.5)
		return "north";
	else if (angle < 157.5)
		return "northwest";
	else if (angle < 202.5)
		return "west";
	else if (angle < 247.5)
		return "southwest";
	else if (angle < 292.5)
		return "south";
	else if (angle < 337.5)
		return "southeast";
	else if (angle >= 337.5)
		return "east";
	else return "";
}

string TourGenerator::findTurn(double degree) const
{
	if (degree >= 1 && degree < 180)
		return "left";
	else if (degree >= 180 && degree <= 359)
		return "right";
	else return "";
}
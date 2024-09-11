#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include "router.h"
#include "HashMap.h"
#include "geotools.h"
using namespace std;

//Using A* search algorithm
vector<GeoPoint> Router::route(const GeoPoint & pt1, const GeoPoint & pt2) const
{
	priority_queue<Cell, vector<Cell>, lowestFScore> openSet;
	HashMap<GeoPoint> cameFrom;

	//For easy lookup into the priority queue
	HashMap<int> keepTrackOfOpenSet;

	// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
	HashMap<double> gScore;
	gScore[pt1.to_string()] = 0;

	//fScore[n] represents our current best guess as to
	// how cheap a path could be from start to finish if it goes through n.
	HashMap<double> fScore;
	fScore[pt1.to_string()] = distance_earth_km(pt1, pt2);

	openSet.push(Cell(pt1, fScore[pt1.to_string()]));
	keepTrackOfOpenSet[pt1.to_string()] = fScore[pt1.to_string()];

	while (!openSet.empty())
	{
		Cell current = openSet.top();
		openSet.pop();
		if (current.point.latitude == pt2.latitude && current.point.longitude == pt2.longitude)
		{
			if (cameFrom.size() == 0)
			{
				vector<GeoPoint> v;
				v.push_back(current.point);
				return v;
			}
			return reconstructPath(cameFrom, current.point);
		}

		for (GeoPoint gp : geodb.get_connected_points(current.point))
		{
			//Set values to default infinity as specified by A*
			if (gScore.find(gp.to_string()) == nullptr)
				gScore[gp.to_string()] = INFINITY;
			if (gScore.find(current.point.to_string()) == nullptr)
				gScore[current.point.to_string()] = INFINITY;

			double tentative_gScore = gScore[current.point.to_string()] + distance_earth_km(current.point, gp);
		
			if (tentative_gScore < gScore[gp.to_string()])
			{
				cameFrom[gp.to_string()] = current.point;
				gScore[gp.to_string()] = tentative_gScore;
				fScore[gp.to_string()] = tentative_gScore + distance_earth_km(gp, pt2);
				if (keepTrackOfOpenSet.find(gp.to_string()) == nullptr)
				{
					openSet.push(Cell(gp, fScore[gp.to_string()]));
					keepTrackOfOpenSet[gp.to_string()] = fScore[gp.to_string()];
				}
			}
		}
	}
	vector<GeoPoint> v;
	return v;
}

vector<GeoPoint> Router::reconstructPath(HashMap<GeoPoint>&cameFrom, GeoPoint cur) const
{
	vector<GeoPoint> total_path;
	total_path.push_back(cur);

	//Insert Geopoints from cameFrom to total_path in order
	while (cameFrom.find(cur.to_string()) != nullptr)
	{
		cur = cameFrom[cur.to_string()];
		total_path.insert(total_path.begin(), cur);
	}
	for (auto p : total_path)
		cerr << p.sLatitude + "," + p.sLongitude << "," << endl;
	return total_path;
}
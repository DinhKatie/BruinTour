#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "HashMap.h"
#include "geodb.h"
#include "router.h"
#include "base_classes.h"
#include "tour_generator.h"

using namespace std;

//int main()
//{
////	/*HashMap<double> nameToGPA;
////	
////	nameToGPA.insert("Carey", 3.5); 
////	nameToGPA.insert("David", 2.99);
////	nameToGPA.insert("Carey", 1);
////	nameToGPA["David"] = 21;
////	nameToGPA["Ben"] = 32;
////	nameToGPA["Linda"]= 1000;
////	nameToGPA["Caroline"];
////	nameToGPA["Carolin"];
////	nameToGPA["Caroli"];
////	nameToGPA["c"];
////
////	nameToGPA.print();*/
////
//	GeoDatabase gb;
//	gb.load("mapdata.txt"); //katie remember to change base_class.h back
//	//GeoPoint gp;
//	//gb.get_poi_location("Ackerman Union", gp);
//	//cerr << gp.sLatitude << ", " << gp.sLongitude << endl;
//	//GeoPoint gp2("34.0709682", "-118.4434844");
//	//GeoPoint gp3("34.0709688", "-118.4435538");
//	//GeoPoint gp4("34.0736122", "-118.4927669");
//	//GeoPoint gp5("34.0600768", "-118.4467216");
//	//cerr << gb.get_street_name(gp2, gp3) << endl;
//	//vector<GeoPoint> points = gb.get_connected_points(gp4);
//	//for (GeoPoint g : points)
//	//	cerr << g.sLatitude << ", " << g.sLongitude << endl;
//	//cerr << "------" << endl;
//	//points = gb.get_connected_points(gp5);
//	//for (GeoPoint g : points)
//	//	cerr << g.sLatitude << ", " << g.sLongitude << endl;
//	//cerr << "------" << endl;
//	//points = gb.get_connected_points(GeoPoint("34.0601422", "-118.4468929"));
//	//for (GeoPoint g : points)
//	//	cerr << g.sLatitude << ", " << g.sLongitude << endl;
//	//cerr << "-----" << endl;
//
//	//GeoPoint p1("34.0601422", "-118.4468929");
//	//GeoPoint p2("34.0600768", "-118.4467216");
//	//cout << gb.get_street_name(p1, p2) << endl; // writes "a path"
//	//cout << gb.get_street_name(p2, p1) << endl; // writes "a path"
//
//	//cerr << "-----" << endl;
//	//GeoPoint p11("34.0732851", "-118.4931016");
//	//GeoPoint p22("34.0736122", "-118.4927669");
//	//cout << gb.get_street_name(p11, p22) << endl; // writes "Glenmere Way"
//	//cout << gb.get_street_name(p22, p11) << endl;
//
//	//cerr << "-----" << endl;
//	//GeoPoint p111("34.0602175", "-118.4464952");
//	//GeoPoint p222("34.0600768", "-118.4467216");
//	//cout << gb.get_street_name(p111, p222) << endl; // writes "Kinross Avenue"
//	//cout << gb.get_street_name(p222, p111) << endl;
//
//	
//	GeoPoint gp1 = GeoPoint("34.0625329", "-118.4470263");
//	GeoPoint gp11 = GeoPoint("34.0625329", "-118.4470263");
//	GeoPoint gp2 = GeoPoint("34.0685657",  "-118.4489289");
//	GeoPoint gp22 = GeoPoint("34.0632405", "-118.4470467");
//	GeoPoint test = GeoPoint("34.0636344", "-118.4482275");
//	GeoPoint test2 = GeoPoint("34.0636457", "-118.4475203");
//	GeoPoint third = GeoPoint("34.0636533", "-118.4470480");
//	GeoPoint six = GeoPoint("34.0705851", "-118.4439429");
//	GeoPoint b = GeoPoint("34.0630614", "-118.4468781");
//	Router r(gb);
//	vector<GeoPoint> path = r.route(six, b);
//	cerr << endl << endl;
//	r.print(path);
//	/*
//	TourGenerator tb = TourGenerator(gb, r);
//	Stops stop = Stops();
//	stop.load("stops.txt");
//	vector<TourCommand> cmds = tb.generate_tour(stop);
//	GeoPoint point1, point2;
//	for (TourCommand t : cmds)
//	{
//		if (t.get_command_type() == TourCommand::commentary)
//			cerr << t.get_poi() << " -> " << t.get_commentary() << endl;
//		else if (t.get_command_type() == TourCommand::proceed)
//		{
//			t.get_points(point1, point2);
//			cerr << "Proceed " << t.get_distance() << " miles " << t.get_direction() << " on " << t.get_street() << " from "
//				<< point1.to_string() << " to " << point2.to_string() << endl;
//		}
//		else if (t.get_command_type() == TourCommand::turn)
//		{
//			cerr << "Turn " << t.get_direction() << " onto " << t.get_street() << endl;
//		}
//	}*/
//}

void print_tour(vector<TourCommand>&tcs)
{
	double total_dist = 0;
	std::string direction;
	double street_distance = 0;

	cout << "Starting tour...\n";

	for (size_t i = 0; i < tcs.size(); ++i)
	{
		if (tcs[i].get_command_type() == TourCommand::commentary)
		{
			cout << "Welcome to " << tcs[i].get_poi() << "!\n";
			cout << tcs[i].get_commentary() << "\n";
		}
		else if (tcs[i].get_command_type() == TourCommand::turn)
		{
			cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
		}
		else if (tcs[i].get_command_type() == TourCommand::proceed)
		{
			total_dist += tcs[i].get_distance();
			if (direction.empty())
				direction = tcs[i].get_direction();
			street_distance += tcs[i].get_distance();
			if (i + 1 < tcs.size() && tcs[i + 1].get_command_type() == TourCommand::proceed
				&& tcs[i + 1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
			{
				continue;
			}

			cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
			street_distance = 0;
			direction.clear();
		}
	}

	cout << "Your tour has finished!\n";
	cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char* argv[])
{
	/*if (argc != 3)
	{
		cout << "usage: BruinTour mapdata.txt stops.txt\n";
		return 1;
	}*/

	GeoDatabase geodb;
	if (!geodb.load("mapdata.txt"))
	{
		cout << "Unable to load map data: " << argv[1] << endl;
		return 1;
	}

	Router router(geodb);
	TourGenerator tg(geodb, router);

	Stops stops;
	if (!stops.load("stops.txt"))
	{
		cout << "Unable to load tour data: " << argv[2] << endl;
		return 1;
	}

	std::cout << "Routing...\n\n";

	vector<TourCommand> tcs = tg.generate_tour(stops);
	if (tcs.empty())
		cout << "Unable to generate tour!\n";
	else
		print_tour(tcs);
}
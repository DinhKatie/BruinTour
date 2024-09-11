# BruinTour

BruinTour is a project that takes in map data and generates an optimal tour route along given points of interests, along with captions for each.
Main.cpp currently contains many test examples.

## Input and Output
The input consists of two files:
1. mapdata.txt: A data file that contains details on thousands of streets in Westwood, and
lists the latitude and longitude of each street block by block (this data is derived from the
Open Street Map service). The data file also contains dozens of "points of interest"
(PoIs) that are candidates for the tour (e.g., Diddy Riese, Kerckhoff Hall, etc.)
2. stops.txt: A proposed list of points of interest (PoIs) for a given tour along with a
description of each PoI for the robot to vocalize, e.g.:
Ackerman Union|This is Ackerman where you buy stuff.
John Wooden Center|This is where you go to get swole.
Diddy Riese|They sell yummy cheap cookies here.

The output is a set of instructions for a BruinTour robot to follow in order to give its tour. Instructions fall into three different categories:
● Commentary: Instructs the robot to give commentary about the current PoI, e.g., "John
Wooden Center: This is where you get swole."
● Proceed: Instructs the robot to proceed in a particular direction on a street
● Turn: Instructs the robot to make a turn from one street onto another 

The output is shown as:
Starting tour...
Welcome to Ackerman Union!
This is Ackerman where you buy stuff.
Proceed 0.028 miles north on a path
Take a left turn on Bruin Walk
Proceed 0.098 miles west on Bruin Walk
Take a right turn on a path
Proceed 0.074 miles north on a path
Welcome to John Wooden Center!
This is where you go to get swole.

## GeoDataBase Class

The Geographic Database class:
1. Loads the GeoPoint data from a text data file into hashmaps.
2. Given a latitude/longitude coordinate c (GeoPoint), returns all
latitude/longitude coordinates (GeoPoints) that are at the other endpoint of street
segments that GeoPoint c is one endpoint of, according to mapdata.txt
3. Given a point of interest (e.g., Diddy Riese), return the GeoPoint where that point of
interest is located.
4. Given two GeoPoints (which are directly connected by a segment in the OSM map data),
return the street name associated with that segment of road (e.g., Westwood Blvd).

## Routing

Routing uses the A* algorithm to find the most optimal route between PoIs.




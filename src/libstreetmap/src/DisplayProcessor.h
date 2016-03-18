/*
 * DisplayProcessor.h
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

/* This file is the bridge between the graphics & user input and the map and event data */

#ifndef DISPLAYPROCESSOR_H
#define DISPLAYPROCESSOR_H

#include "mapProcessor.h"
#include <unordered_map>
#include <vector>
#include <sstream> // for processing intersection name searches
#include <string>
#include "StreetsDatabaseAPI.h"
#include "utility.h"
#include "UserInstruction.h"

#include "InstructionDisplayProcessor.h"



/****** UTILITY CLASSES ******/
/**
 * Container for data needed to draw street names
 */
class StreetName {
public:
	StreetName(std::string _name, double _X, double _Y, double _theta) {
		name = _name;
		X = _X;
		Y = _Y;
		theta = _theta;
	}
	std::string name;
	double X, Y, theta;
};


class TriangleXY {
public:
	TriangleXY(const std::pair<double, double> &_A, const std::pair<double, double> &_B, const std::pair<double, double> &_C) {
		A = _A;
		B = _B;
		C = _C;
	}
	std::pair<double, double> A, B, C;
};


class PointNode {
public:
	PointNode(std::pair<double, double> _XY, PointNode* _prev) {
		XY = _XY;
		prev = _prev;
		next = NULL;
	}
	std::pair<double, double> XY;
	PointNode *prev, *next;
	/**
	 * Whether or not the triangle formed by this node, prev and next does not contain any other points
	 * @return
	 */
	bool isEar();
};

/****** MAIN FUNCTIONS ******/

/**
 * Sends instruction to redraw everything on the screen (e.g. when scrolling occurs)
 * @param centerX X coordinate of new center of screen (in map coordinates)
 * @param centerY Y coordinate of new center of screen (in map coordinates)
 * @param zoomLevel how zoomed in the screen is (affects what to draw)
 */
void drawAll(double centerX, double centerY, double zoomLevel /* also include flags of what to draw/not draw */ );

/**
 * Calculates positions and angles required to draw street names on the map
 * @param intersectionIDs Vector of id's of intersections that are in the viewing window
 */
std::vector<StreetName> calcStreetNames(int* intersectionIDs, int numIntersections);


void processSettingsClick();    // settings button

/**
 * Find all intersections matching the string input from text field, return them in a vector
 * @param name the name of the intersection
 */
std::vector<unsigned> processFindClick(std::string name);

/**
 * Executes correct response to user's "find" commands
 * @param input the command entered by user
 */
void processUserCommand(std::string input);


std::vector<InstructionDisplayElement> parseStreetSegmentsToDirections(std::vector<unsigned> path);

/**
 * Executes correct response to user's "direction" commands
 * @param fromString
 * @param destinationString
 */
void processUserCommand(std::string fromString, std::string destinationString);

/**
 * Close the current map, open a new one and show it.
 * @param mapName The name of the new map's binary (e.g. toronto.streets.bin)
 */
void processNewMapClick(std::string mapName);


void processEventFilterClick(); // event type checkboxes

/**
 * Takes a set of points that define a polygon, returns a set of triangles that fully cover its area
 * Uses the method specified here: http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
 * @param
 * @return
 */
std::vector<TriangleXY> polygonToTriangles(std::vector<std::pair<double, double>> points);

/**
 * Print information about a given intersection to the terminal
 * @param intersectionID
 * @param name
 */
void printIntersectionInformation(unsigned intersectionID);

/**
 * Return the ID of the intersection closest to the point XY, print the information about that intersection
 * @param X X coordinate to find intersection near
 * @param Y Y coordinate to find intersection near
 */
unsigned processIntersectionClick(double X, double Y, double &intX, double &intY);


/****** HELPER FUNCTIONS ******/

/**
 * Returns true if point P is inside the triangle formed by ABC
 * @param P
 * @param A
 * @param B
 * @param C
 * @return
 */
bool _isInsideTriangle(std::pair<double, double> P, std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C);

/**
 * Calculate 2x the area of the triangle formed by ABC for the purpose of finding if a point is inside a triangle
 * @param A
 * @param B
 * @param C
 * @return
 */
double _calcTriangleArea(std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C);

/**
 * Returns the distance squared from the intersection of the given ID to a given XY point
 * @param intersectionID the ID of the intersection
 * @param X the X coordinate to find distance squared from
 * @param Y the Y coordinate to find distance squared from
 * @return
 */
double _intersectionToPointDistance(unsigned intersectionID, double X, double Y);

/**
 * Calculate values needed to draw a street name
 * @param streetSegmentInfo The StreetSegmentInfo of the street segment to draw the name on
 * @param nameStr The name of the street
 * @return A StreetName object containing the required data to draw the street name
 */
StreetName _computeStreetNamePosition(const StreetSegmentInfo& streetSegmentInfo, std::string nameStr);

#endif /* DISPLAYPROCESSOR_H */

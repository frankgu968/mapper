/*
 * StreetSegment.cpp
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

#include "StreetSegment.h"

StreetSegment::StreetSegment() {
	length = 0;
	type = unclassified;
	suitableForName = false;
	travelTime = 0.0;
	rectangles = NULL;
}

StreetSegment::StreetSegment(const StreetSegment &rhs){
	info = rhs.info;
	length = rhs.length;
	type = rhs.type;
	suitableForName = rhs.suitableForName;
	travelTime = rhs.travelTime;
	curvePoints = rhs.curvePoints;
	drawCurvePoints = rhs.drawCurvePoints;


	if(rhs.rectangles == NULL)
		rectangles = NULL;

	else{
		unsigned numRectangles = curvePoints.size()-1;
		rectangles = new StreetRectangle*[numRectangles];

		for (unsigned i=0; i<numRectangles; i++) {
			StreetRectangle otherRect = *rhs.rectangles[i];
			rectangles[i] = new StreetRectangle(otherRect);
		}
	}
}

void StreetSegment::operator=(const StreetSegment &rhs){
	info = rhs.info;
	length = rhs.length;
	type = rhs.type;
	suitableForName = rhs.suitableForName;
	travelTime = rhs.travelTime;
	curvePoints = rhs.curvePoints;
	drawCurvePoints = rhs.drawCurvePoints;


	if(rhs.rectangles == NULL)
		rectangles = NULL;

	else{
		unsigned numRectangles = curvePoints.size()-1;
		rectangles = new StreetRectangle*[numRectangles];

		for (unsigned i=0; i<numRectangles; i++) {
			StreetRectangle otherRect = *rhs.rectangles[i];
			rectangles[i] = new StreetRectangle(otherRect);
		}
	}
}

StreetSegment::~StreetSegment() {
	if (rectangles != NULL) {
		for (unsigned i=0; i<curvePoints.size() - 1; i++) {
			delete rectangles[i];
		}
		delete[] rectangles;
	}
}

/**
 * Initializes values with data from StreetsDatabaseAPI
 * @param streetSegmentID the ID of this street segment
 */
void StreetSegment::initialize(unsigned streetSegmentID) {
	info = getStreetSegmentInfo(streetSegmentID);
}

/**
 * Sets the length of the street, determines the travel time
 * @param length length of the street
 */
void StreetSegment::setLength(double _length) {
	length = _length;
	// calculate time to travel this street segment (in minutes). Length is in metres and speed in km/h
	travelTime = ((length / 1000) / info.speedLimit) * 60;
}

/**
 * Adds a curve point to the street segment
 * @param X the x coordinate
 * @param Y the y coordinate
 */
void StreetSegment::addCurvePoint(PointXY coord) {
	curvePoints.push_back(coord);
}

/**
 * Sets the type of the street (motorway, secondary etc.)
 * @param _type the type
 */
void StreetSegment::setStreetType(StreetSegmentType _type) {
	type = _type;
}

/**
 * Gets the type of the street (motorway, secondary etc.)
 * @return the type
 */
StreetSegmentType StreetSegment::getStreetType() {
	return type;
}

/**
 * Gets struct of street segment info, e.g. start/end intersections
 * @return struct of street segment info, e.g. start/end intersections
 */
StreetSegmentInfo StreetSegment::getInfo() {
	return info;
}

/**
 * Gets length of this street segment (in m)
 * @return
 */
double StreetSegment::getLength() {
	return length;
}

/**
 * Gets curve points of the street segment (as X,Y)
 * @return curve points of this street segment as X,Y. For each pair, first is X, second is Y
 */
std::vector<PointXY> StreetSegment::getCurvePoints() {
	return curvePoints;
}

/**
 * Gets curve points of the street segment (as X,Y, in adjusted drawing coordinates)
 * @return curve points of this street segment as X,Y. For each pair, first is X, second is Y
 */
std::vector<PointXY> StreetSegment::getDrawCurvePoints() {
	return drawCurvePoints;
}

/**
 * Check if this street segment is straight enough to be suitable draw the street name on, store this value
 * internally.
 */
void StreetSegment::determineSuitableForName() {
	// The method is to draw a straight line from the beginning to end of the segment, and if any
	// point deviates from this straight line by more than 5% of the length of the segment on either
	// the X or Y axis then the segment is unsuitable to draw a name on.

	// defaults to true, is set to false if any curve point deviates too much
	suitableForName = true;

	// calculate line between the start and end points
	PointXY first = curvePoints[0];
	PointXY last = curvePoints[curvePoints.size() - 1];
	double slope = (last.y - first.y) / (last.x - first.x);
	double distanceLimit = 0.05 * sqrt( (last.y - first.y) * (last.y - first.y) + (last.x - first.x) * (last.x - first.x) );

	// horizontal and vertical difference from the i'th point to the starting point
	double dx, dy;
	// horizontal and vertical difference from the i'th point to the corresponding point on
	// the line with the same Y and X value, respectively
	double xDistance, yDistance;

	// iterate over all curve points other than the first and last
	unsigned finalIndex = curvePoints.size() - 1;
	for (unsigned i=1; i<finalIndex; i++) {
		// check for allowable X deviation by taking y translation / slope and comparing to the curve point
		dy = curvePoints[i].y - first.y;
		xDistance = first.x + dy / slope;
		if (xDistance >= distanceLimit) {
			suitableForName = false;
			break;
		}

		// check for allowable Y deviation by taking x translation * slope and comparing to the curve point
		dx = curvePoints[i].x - first.x;
		yDistance = first.y + dx * slope;
		if (yDistance >= distanceLimit) {
			suitableForName = false;
			break;
		}
	}
}

/**
 * Get the rectangles that define the street segment for drawing
 * @return Array of pointers to StreetRectangles, each of which contain 4 PointXY's that define the rectangle
 */
StreetRectangle** StreetSegment::getRectangles() const {
	return rectangles;
}

/**
 * Fills drawCurvePoints with the right values based on the existing XY coordinates and the map scale factors
 * @param offset The center point of the map
 * @param scale The amount to multiply the shifted coordinates by to get a -1000 to 1000 range for X values
 */
void StreetSegment::setDrawCoordinates(PointXY drawingMapOffset, double drawingMapScale) {
	drawCurvePoints.resize(curvePoints.size()); // pre-allocate memory
	for (unsigned i=0; i<curvePoints.size(); i++) {
		drawCurvePoints[i].x = (curvePoints[i].x - drawingMapOffset.x) * drawingMapScale;
		drawCurvePoints[i].y = (curvePoints[i].y - drawingMapOffset.y) * drawingMapScale;
	}

	// generate the rectangles for drawing
	unsigned numRectangles = curvePoints.size() - 1;
	rectangles = new StreetRectangle*[numRectangles];
	// set street thickness based on street type
	double streetThickness;
	if (type == motorway) {
		streetThickness = 6 * drawingMapScale;
	}
	else if (type == primary) {
		streetThickness = 4 * drawingMapScale;
	}
	else if (type == secondary) {
		streetThickness = 3 * drawingMapScale;
	}
	else {
		streetThickness = 2 * drawingMapScale;
	}

	for (unsigned i=0; i<numRectangles; i++) {
		rectangles[i] = new StreetRectangle(drawCurvePoints[i], drawCurvePoints[i+1], streetThickness);
	}
}

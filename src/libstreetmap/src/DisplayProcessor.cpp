/*
 * DisplayProcessor.cpp
 *
 *  Created on: 18/02/2016
 *      Author: william
 */

#include "DisplayProcessor.h"

/**
 * Takes a set of points that define a polygon, returns a set of triangles that fully cover its area
 * Uses the method specified here: http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
 * @param
 * @return
 */
std::vector<TriangleXY> polygonToTriangles(std::vector<std::pair<double, double>> points) {
	// check for at least 3 points in polygon
	if (points.size() < 3) {
		std::vector<TriangleXY> empty;
		return empty;
	}

	// set up circular doubly linked list of points
	PointNode* head = new PointNode(points[0], NULL);
	PointNode* current = head;
	for (unsigned i=1; i<points.size(); i++) {
		current->next = new PointNode(points[i], current);
		current = current->next;
	}
	// close off circular list
	current->next = head;
	head->prev = current;

	// vector of triangles to return
	std::vector<TriangleXY> triangles;

	// go through list continuously until there are only 3 points left
	current = head;
	while (current->next->next != current->prev) {
		// if current point is an ear, it can be removed as a triangle with the previous and next points
		if (current->isEar()) {
			// add triangle
			TriangleXY toAdd(current->XY, current->prev->XY, current->next->XY);
			triangles.push_back(toAdd);
			// remove point
			current->prev->next = current->next;
			current->next->prev = current->prev;
			PointNode *tmp = current;
			current = current->next;
			delete tmp;
		}
		else {
			// move to next point
			current = current->next;
		}
	}

	// add triangle of last 3 points to vector
	TriangleXY toAdd(current->XY, current->prev->XY, current->next->XY);
	triangles.push_back(toAdd);

	// free memory
	delete current->prev;
	delete current->next;
	delete current;

	return triangles;
}

/**
 * Returns true if point P is inside the triangle formed by ABC
 * @param P
 * @param A
 * @param B
 * @param C
 * @return
 */
bool _isInsideTriangle(std::pair<double, double> P, std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C) {
	// method is: if the sum of areas formed by P and each pair of points AB, AC, BC is less than the
	// area of ABC, P is inside the triangle
	double area  = _calcTriangleArea(A, B, C);
	double area1 = _calcTriangleArea(P, A, B);
	double area2 = _calcTriangleArea(P, A, C);
	double area3 = _calcTriangleArea(P, B, C);

	if (area1 + area2 + area3 < area) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Calculate 2x the area of the triangle formed by ABC for the purpose of finding if a point is inside a triangle
 * @param A
 * @param B
 * @param C
 * @return
 */
double _calcTriangleArea(std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C) {
	// using the determinant of the triangle to find area efficiently
	return abs(A.first * (B.second  - C.second) + B.first * (C.second - A.second) + C.first * (A.second - B.second));
}

/**
 * Returns the distance squared from the intersection of the given ID to a given XY point
 * @param intersectionID the ID of the intersection
 * @param X the X coordinate to find distance squared from
 * @param Y the Y coordinate to find distance squared from
 * @return
 */
double _intersectionToPointDistance(unsigned intersectionID, double X, double Y) {
	// get XY coordinates of the intersection
	PointXY startXY = convertLatLonToXY(
			getIntersectionPosition(intersectionID),
			MapData::instance()->getScalingFactor());
	// calculate the distance from the point to the center of the viewing window
	double dx = startXY.x - X;
	double dy = startXY.y - Y;
	return dx*dx + dy*dy;
}

/**
 * Whether or not the triangle formed by this node, prev and next does not contain any other points
 * @return
 */
bool PointNode::isEar() {
	// set up triangle formed by this and adjacent points
	std::pair<double, double> A, B, C;
	A = XY;
	B = prev->XY;
	C = next->XY;

	// loop through list until all but this and adjacent points have been looped through
	PointNode* current = next->next;
	bool ear = true;
	while (current != prev) {
		if (_isInsideTriangle(current->XY, A, B, C)) {
			ear = false;
			break;
		}
		current = current->next;
	}

	return ear;
}

/**
 * Calculates positions and angles required to draw street names on the map
 * @param intersectionIDs Vector of id's of intersections that are in the viewing window
 */
std::vector<StreetName> calcStreetNames(int* intersectionIDs, int numIntersections) {
	// method is: iterate through all intersections, for each street segment which is part of a street that
	// doesn't have a name currently on it, if the street segment is suitable to draw a name on then draw the name.
	// Never draw names on more than 1 street segment per intersection for aesthetic reasons.

	// THERE SHOULD BE A LIST OF STREET SEGMENTS THAT HAVE NAMES ON THEM, WHEN CALLED THIS WILL SEARCH
	// AND FOR EACH ONE NO LONGER PRESENT, PLACE THAT STREETS NAME AGAIN

	// mapData object
	MapData* map = MapData::instance();
	// the StreetNames to return
	std::vector<StreetName> streetNames;

	// if a street ID is in this vector, the name's already been drawn
	std::vector<unsigned> alreadyDrawnStreets;

	// loop through all intersections
	for (unsigned i=0; i<numIntersections; i++) {
		// loop through all street segments
		std::vector<unsigned> streetSegs = find_intersection_street_segments(intersectionIDs[i]);
		for (unsigned j=0; j<streetSegs.size(); j++) {
			// check if this street has already had its name drawn
			StreetSegmentInfo streetSegmentInfo = map->getStreetSegments()[streetSegs[j]].getInfo();
			// get the streetID of the street the segment is part of
			unsigned streetID = streetSegmentInfo.streetID;
			bool alreadyDrawn = false;
			for (unsigned k=0; k<alreadyDrawnStreets.size(); k++) {
				if (streetID == alreadyDrawnStreets[k]) {
					alreadyDrawn = true;
					break;
				}
			}
			if (!alreadyDrawn) {
				// check if street name is known
				std::string nameStr = getStreetName(streetSegmentInfo.streetID);
				if (nameStr != "<unknown>") {
					StreetName name = _computeStreetNamePosition(streetSegmentInfo, nameStr);
					streetNames.push_back(name);
				}

				// so street doesn't have its name on screen more than once
				alreadyDrawnStreets.push_back(streetID);

				// don't draw more than 1 street name connected to one intersection (would look too crowded)
				break;
			}
		}
	}

	return streetNames;
}

/**
 * Calculate values needed to draw a street name
 * @param streetSegmentInfo The StreetSegmentInfo of the street segment to draw the name on
 * @param nameStr The name of the street
 * @return A StreetName object containing the required data to draw the street name
 */
StreetName _computeStreetNamePosition(const StreetSegmentInfo& streetSegmentInfo, std::string nameStr) {
	// MapData instance
	MapData* map = MapData::instance();

	// get the IDs of the start and end intersections
	unsigned startIntersectionID = streetSegmentInfo.from;
	unsigned endIntersectionID = streetSegmentInfo.to;

	// get the XY positions of the start and end intersections
	PointXY startIntersectionXY = convertLatLonToXY(
						getIntersectionPosition(startIntersectionID),
						map->getScalingFactor());

	PointXY endIntersectionXY = convertLatLonToXY(
							getIntersectionPosition(endIntersectionID),
							map->getScalingFactor());

	// calculate difference between start and end points
	double dx = endIntersectionXY.x - startIntersectionXY.x;
	double dy = endIntersectionXY.y - startIntersectionXY.y;

	// calculate X,Y (weighted average, 1/4 of the way from startXY to endXY)
	double X = startIntersectionXY.x + 0.25 * dx;
	double Y = startIntersectionXY.y + 0.25 * dy;

	// calculate angle, counterclockwise from positive x axis
	double theta = atan(dy/dx) / DEG_TO_RAD;

	// if the street is one-way, add an arrow to the street name indicating its direction
	if (streetSegmentInfo.oneWay) {
		// determine which direction the arrow should go
		if (dx < 0) {
			nameStr = "<-" + nameStr;
		}
		else {
			nameStr = nameStr + " ->";
		}
	}

	// construct StreetName object to return
	StreetName streetName(nameStr, X, Y, theta);
	return streetName;
}

/**
 * Close the current map, open a new one and show it.
 * @param mapName The name of the new map's binary (e.g. toronto.streets.bin)
 */
void processNewMapClick(std::string mapName) {
	// TODO: Suspend graphics during this period? Reset graphics to center of new map?
	if (mapName != "") {
		close_map();
		load_map(mapName);
	}
}



/**
 * Find all intersections matching the string input from text field, return them in a vector
 * @param name the name of the intersection
 */
std::vector<unsigned> processFindClick(std::string name) {
	std::cout << "Received name: " << name << std::endl;

	// to return
	std::vector<unsigned> intersectionIDs;
	std::stringstream lineStream(name);

	// parse out streets
	std::string street1, street2, tmp;
	bool firstStreet = true;

	while (lineStream >> tmp) {
		if (firstStreet) {
			// if next word is "and", that means first street name has been fully received
			if (tmp == "and" || tmp == "&") {
				firstStreet = false;
			}
			else {
				if (street1 == "") {
					street1 = tmp;
				}
				else {
					street1 += " " + tmp;
				}
			}
		}
		else {
			if (street2 == "") {
				street2 = tmp;
			}
			else {
				street2 += " " + tmp;
			}
		}
	}

	std::cout << "First street: " << street1 << std::endl;
	std::cout << "Second street: " << street2 << std::endl;

	// find intersection IDs
	intersectionIDs = find_intersection_ids_from_street_names(street1, street2);

	std::cout << "Found " << intersectionIDs.size() << " intersection ids" << std::endl;

	// print info about intersections (name & location)
	for (unsigned i=0; i<intersectionIDs.size(); i++) {
		// print info about intersection (name & location)
		printIntersectionInformation(intersectionIDs[i]);
	}

	return intersectionIDs;
}

/**
 * Executes correct response to user's commands
 * @param input the command entered by user
 */
void processUserCommand(std::string input){
	//Load instruction
	UserInstruction newInstr(input);

	//Get instruction parameters
	instructionTypes instrType = newInstr.getInstructionType();

	//Choose correct response to input

	//Invalid input
	if(instrType == Invalid_Command){
		//TODO: TELL USER COMMAND INVALID
	}

	//User searched for a street
	else if (instrType == Find_Street){
		unsigned streetId = newInstr.getId(0);

		//TODO: GET DRAW COORDS, CENTRE MAP ON STREET
	}

	//User searched for an intersection
	else if (instrType == Find_Intersection){
		unsigned intersectionId = newInstr.getId(0);

		//TODO: GET DRAW COORDS, CENTRE MAP ON INTERSECTION
	}
}

/**
 * Executes correct response to user's "direction" commands
 * @param fromString
 * @param destinationString
 */
void processUserCommand(std::string fromString, std::string destinationString){
	//Load instruction
	UserInstruction newInstr(fromString, destinationString);

	//Get instruction parameters
	instructionTypes instrType = newInstr.getInstructionType();

	//Choose correct response to input

	//Invalid input
	if(instrType == Invalid_Command){
		//TODO:TELL USER COMMAND IS INVALID
	}

	//Path finding between two intersections
	else if (instrType == Path_Two_Intersection){
		unsigned startIntersection = newInstr.getId(0);
		unsigned endIntersection = newInstr.getId(1);

		//TODO: FIND PATH, DISPLAY PATH AND INSTRUCTIONS
	}

	//Path finding from intersection to POI
	else if (instrType == Path_Intersection_to_POI){
		unsigned startIntersection = newInstr.getId(0);
		std::string poiName = getPointOfInterestName(newInstr.getId(1));

		//TODO: FIND PATH, DISPLAY PATH AND INSTRUCTIONS
	}
}

std::vector<InstructionDisplayElement> parseStreetSegmentsToDirections(std::vector<unsigned> path){
	InstructionDisplayProcessor processor(path);

	return processor.getInstructions();
}

/**
 * Print information about a given intersection to the terminal
 * @param intersectionID
 */
void printIntersectionInformation(unsigned intersectionID) {
	std::cout << "Intersection " << getIntersectionName(intersectionID) << "(" << intersectionID << ") is at location " << getIntersectionPosition(intersectionID) << std::endl;
}

/**
 * Return the ID of the intersection closest to the point XY, print the information about that intersection
 * @param X X coordinate to find intersection near
 * @param Y Y coordinate to find intersection near
 */
unsigned processIntersectionClick(double X, double Y, double &intX, double &intY) {
	// TODO: MAKE THIS CHOOSE WHICH KD TREE TO QUERY BASED ON ZOOM LEVEL
	int intersectionID = find_closest_point(SmallIntersections, PointXY(X,Y));
	printIntersectionInformation(intersectionID);
	auto temp = convertLatLonToXY(getIntersectionPosition(intersectionID), MapData::instance()->getScalingFactor());
	intX = (temp.x/1000000+6.37)*1000;
	intY = (temp.y/1000000-4.85)*1000;
	return intersectionID;
}

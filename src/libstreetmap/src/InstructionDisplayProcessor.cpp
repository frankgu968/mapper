/*
 * InstructionDisplayProcessor.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: eddd
 */

#include "InstructionDisplayProcessor.h"

InstructionDisplayProcessor::InstructionDisplayProcessor(std::vector<unsigned> _path) {

	//Check if no path found
	if(_path.size() == 0){
		pathIsEmpty = true;
	}

	//Breaks the path into operations
	processPath(_path);

	//Calculate the transition between each road segment
}

InstructionDisplayProcessor::~InstructionDisplayProcessor() {
	// TODO Auto-generated destructor stub
}

/**
 * Processes path to separate it into unique street operations (e.g. turns)
 * @param path the street segments to process
 */
void InstructionDisplayProcessor::processPath(std::vector<unsigned> &path){

	//Display start message initial path
	StreetSegmentProcessingElement preElem(path[0]);

	//Load initial path direction

	//Iterate through entire path
	for(unsigned i = 0; i<path.size(); i++){
		unsigned numSegsInStreet = 0;
		double streetDistance = 0;

		StreetSegmentProcessingElement curElem(path[i]);

		//Iterate through all elements belonging to the same street
		do {
			//Increment counters
			numSegsInStreet++;
			streetDistance += curElem.length;

			//Iterate to next element
			preElem = curElem;
			i++;
			curElem.loadNewSegment(path[i]);
		}while(i<(path.size()) && preElem.streetId == curElem.streetId);

		//Add street distance information
		InstructionDisplayElement newElem;
		newElem.setName(preElem.streetName);
		newElem.setDistance(streetDistance);
		newElem.setProcessingElements(preElem, curElem);
		setDirections(preElem, curElem);

		//Calculate number of streets intersect here
		MapData* map = MapData::instance();
		unsigned intersectionId = preElem.trueIntersectionDestination;
		IntersectionXY** intersections =  map->getSmallIntersections();
		unsigned numStreetsAtIntersect = intersections[intersectionId]->getNumStreetSegs();

		preElem.numStreetsAtDestinationIntersection = numStreetsAtIntersect;

		//Add instruction from this street
		instructions.push_back(newElem);
	}
}

/**
 * Finds the transitions occuring in every message to be displayed
 */
void InstructionDisplayProcessor::calculateTransitions(){
	//Iterate through all display processer elements except the last one
	//As the last element has no transition
	for(unsigned i = 0; i<instructions.size(); i++){
		//Get processing elements required
		InstructionDisplayElement newDisplayElem = instructions[i];
		StreetSegmentProcessingElement fromSeg = newDisplayElem.getFromProcessingElement();
		StreetSegmentProcessingElement toSeg = newDisplayElem.getToProcessingElement();

		//Calculate the turn parameters

		//Only two roads at intersection - continue straight
		if(fromSeg.numStreetsAtDestinationIntersection == 2){
			newDisplayElem.setTurnStyle(Straight);
		}

		//Pulling on to highway
		else if (fromSeg.type == motorway && toSeg.type != motorway){

		}
		//Off highway
		else if (fromSeg.type != motorway && toSeg.type == motorway){

		}

		//Just a normal turn; calculate type
		else{
			//Get turn angle
			double angle = getAngleBetweenStreetSegs(fromSeg, toSeg);
			double absAngle = abs(angle);



			//Get cardinality, if required

			//

		}
		//Save new element
		instructions[i] = newDisplayElem;
	}
}

/**
 * Sets the direction in which the user is traveling
 * @param from the originating street segment
 * @param to the second street segment
 */
void InstructionDisplayProcessor::setDirections(StreetSegmentProcessingElement &from, StreetSegmentProcessingElement &to){
	//Both are traveling forwards
	if(from.intersectionTo == to.intersectionFrom){
		from.isBackwards = false;
		to.isBackwards = false;

		from.trueIntersectionDestination = from.intersectionTo;
	}

	//Backwards on first, forwards on second
	else if(from.intersectionFrom == to.intersectionFrom){
		from.isBackwards = true;
		to.isBackwards = false;

		from.trueIntersectionDestination = from.intersectionFrom;
	}

	//Driving backwards along both segments
	else if (from.intersectionFrom == to.intersectionTo){
		from.isBackwards = true;
		to.isBackwards = true;

		from.trueIntersectionDestination = from.intersectionFrom;
	}

	//Forwards on first, backwards on second
	else if (from.intersectionTo == to.intersectionTo){
		from.isBackwards = false;
		to.isBackwards = true;

		from.trueIntersectionDestination = from.intersectionTo;
	}
}

/**
 * Gets a vector pointing in the direction of travel along this street segment
 * Can get the direction at the start, or at the end of the segment
 * @param elem the street segment for which the direction will be found
 * @param atEnd True if the direction at the very end of the segment is desired
 * @return pair of doubles representing <X,Y> vector
 */
std::pair<double,double> InstructionDisplayProcessor::getDirectionVector(StreetSegmentProcessingElement elem, bool atEnd){
	unsigned segmentId = elem.segmentId;
	std::pair<double,double> direction;

	//Get Street Segment object
	MapData* map = MapData::instance();
	StreetSegment curSeg = map->getStreetSegments()[segmentId];

	//Points
	PointXY startPoint, endPoint;
	std::vector<PointXY> points = curSeg.getCurvePoints();
	unsigned numPoints = points.size();

	//Get direction at very end of the street
	if(atEnd){
		//Reverse orientation - goes from second point to the starting location
		if(elem.isBackwards){
			startPoint = points[1];
			endPoint = points[0];
		}

		//Normal orientation - goes from second last point to very last point
		else{
			startPoint = points[numPoints-2];
			endPoint = points[numPoints-1];
		}
	}

	//Get direction at the start of the street
	else{
		//Reverse orientation - goes from last point to second last point
		if(elem.isBackwards){
			startPoint = points[numPoints-1];
			endPoint = points[numPoints-2];
		}

		//Normal orientation - goes from first point to second point
		else{
			startPoint = points[0];
			endPoint = points[1];
		}
	}

	//Calculate direction
	direction.first = endPoint.x - startPoint.x;
	direction.second = endPoint.y - startPoint.y;

	return direction;
}

/**
 * Calculates the angle between two street segments
 * @param fromSeg starting segment
 * @param toSeg ending segment
 * @return angle [-180, 180] degrees, measured from the direction of fromSeg
 */
double InstructionDisplayProcessor::getAngleBetweenStreetSegs(StreetSegmentProcessingElement fromSeg, StreetSegmentProcessingElement toSeg){
	//Get vectors for each pair
	std::pair<double,double> startDirection, endDirection;

	startDirection = getDirectionVector(fromSeg, true);
	endDirection = getDirectionVector(toSeg, false);

	//Calculate angle
	double angle = getAngleTwoVectors(startDirection,endDirection);

	return angle;
}



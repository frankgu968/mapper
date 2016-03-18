#include "StreetSegment.h"
#include "string"
#include "MapData.h"

/*
 * StreetSegmentProcessingElement.h
 *
 *  Created on: Mar 17, 2016
 *      Author: eddd
 *
 *  A simple data holding class, for use in processing of street segments
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_STREETSEGMENTPROCESSINGELEMENT_H_
#define MAPPER_LIBSTREETMAP_SRC_STREETSEGMENTPROCESSINGELEMENT_H_


struct StreetSegmentProcessingElement{
	/**
	 * Loads all data for a new segment
	 * @param _segmentId ID of segment to load
	 */
	void loadNewSegment(unsigned _segmentId){
		segmentId = _segmentId;
		isBackwards = false;

		//Get Street Segment Info
		StreetSegment curSeg = MapData::instance()->getStreetSegments()[segmentId];
		type = curSeg.getStreetType();

		StreetSegmentInfo segInfo = curSeg.getInfo();
		intersectionTo = segInfo.to;
		intersectionFrom = segInfo.from;
		streetId = segInfo.streetID;
		streetName = getStreetName(streetId);
		length = curSeg.getLength();

		direction = "";
	}

	//Default constructor
	StreetSegmentProcessingElement(){
		turnAngle = 0;
		streetName = "";
		direction = "";
		type = unclassified;
		numStreetsAtDestinationIntersection = 0;
		isBackwards = false;
		length = 0;
		segmentId = streetId = 0;
		intersectionFrom = intersectionTo = trueIntersectionDestination = 0;
	}

	//Constructor
	StreetSegmentProcessingElement(unsigned _segmentId){
		loadNewSegment(_segmentId);
	}

	//Copy constructor
	StreetSegmentProcessingElement(const StreetSegmentProcessingElement& rhs){
		loadNewSegment(rhs.segmentId);
	}


	//Assignment operator
	void operator=(const StreetSegmentProcessingElement& rhs){
		loadNewSegment(rhs.segmentId);
	}

	bool isBackwards;
	double length, turnAngle;
	std::string streetName, direction;
	StreetSegmentType type;
	unsigned numStreetsAtDestinationIntersection;

	//Street identification
	unsigned segmentId, streetId;

	//Intersection ids
	unsigned intersectionFrom, intersectionTo; //destinat as defined by OSM - may be incorrect for instructions if 2 way street
	unsigned trueIntersectionDestination; //the actual destination (in case driving backwards on this seg)

};



#endif /* MAPPER_LIBSTREETMAP_SRC_STREETSEGMENTPROCESSINGELEMENT_H_ */

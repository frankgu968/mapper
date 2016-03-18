/*
 * InstructionDisplayProcessor.h
 *
 *  Created on: Mar 15, 2016
 *      Author: eddd
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_INSTRUCTIONDISPLAYPROCESSOR_H_
#define MAPPER_LIBSTREETMAP_SRC_INSTRUCTIONDISPLAYPROCESSOR_H_

#define STREETS_AT_INTERSECTION_THRESHOLD 5

#include "InstructionDisplayElement.h"
#include "StreetSegment.h"
#include "MapData.h"
#include "StreetSegmentProcessingElement.h"
#include "utility.h"


class InstructionDisplayProcessor {
public:
	InstructionDisplayProcessor(std::vector<unsigned> _path);
	virtual ~InstructionDisplayProcessor();

	std::vector<InstructionDisplayElement> getInstructions(){
		return instructions;
	}



private:
	std::vector<InstructionDisplayElement> instructions;
	bool pathIsEmpty;


	/**
	 * Function to handle all processing of a path
	 */
	void processPath(std::vector<unsigned> &path);

	/**
	 * Finds the transitions occuring in every message to be displayed, and generates the output message
	 */
	void calculateTransitions();

	/**
	 * Sets the direction in which the user is traveling
	 * @param from the originating street segment
	 * @param to the second street segment
	 */
	void setDirections(StreetSegmentProcessingElement &from, StreetSegmentProcessingElement &to);


	/**
	 * Gets a vector pointing in the direction of travel along this street segment
	 * Can get the direction at the start, or at the end of the segment
	 * @param elem the street segment for which the direction will be found
	 * @param atEnd True if the direction at the very end of the segment is desired
	 * @return pair of doubles representing <X,Y> vector
	 */
	std::pair<double,double> getDirectionVector(StreetSegmentProcessingElement elem, bool atEnd);

	/**
	 * Calculates the angle between two street segments
	 * @param fromSeg starting segment
	 * @param toSeg ending segment
	 * @return angle [-180, 180] degrees, measured from the direction of fromSeg
	 */
	double getAngleBetweenStreetSegs(StreetSegmentProcessingElement fromSeg, StreetSegmentProcessingElement toSeg);

	/**
	 * Returns the appropriate turn style, given an angle of turn
	 * @param angle
	 * @return turnStyle
	 */
	turnStyle calculateTurnStyle(double angle);
};

#endif /* MAPPER_LIBSTREETMAP_SRC_INSTRUCTIONDISPLAYPROCESSOR_H_ */

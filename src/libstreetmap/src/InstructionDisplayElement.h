/*
 * InstructionDisplayElement.h
 *
 *  Created on: Mar 15, 2016
 *      Author: eddd
 */

#ifndef MAPPER_LIBSTREETMAP_INSTRUCTIONDISPLAYELEMENT_H_
#define MAPPER_LIBSTREETMAP_INSTRUCTIONDISPLAYELEMENT_H_

#include <iostream>
#include "StreetSegmentProcessingElement.h"

enum turnStyle{
	Undefined,
	Straight,
	Slight_Right,
	Slight_Left,
	Right,
	Left,
	Extreme_Right,
	Extreme_Left,
	On_Highway,
	Off_Highway
};

/**
 * Simple data holding header only class
 * Holds description for the current section being traversed
 */

class InstructionDisplayElement {
public:
	InstructionDisplayElement(){
		distance = 0;
		numIds = 0;

		streetName = "";
		displayMessage = "";
		turn = Undefined;
	}
	virtual ~InstructionDisplayElement(){

	}

	/**
	 * Returns output display message
	 * e.g. "Turn right onto College Street"
	 * @return message
	 */
	std::string getDisplayMessage(){
		return displayMessage;
	}
	/**
	 * Getter of street name
	 * @return street name
	 */
	std::string getStreetName(){
		return streetName;
	}

	/**
	 * Returns distance of this driving segment (meters)
	 * @return distance
	 */
	double getDistance(){
		return distance;
	}

	/**
	 * Gets the initial street segment data for this operation
	 * @return from segment
	 */
	StreetSegmentProcessingElement getFromProcessingElement(){
		return from;
	}

	/**
	 * Gets the final street segment data for this operation
	 * @return to segment
	 */
	StreetSegmentProcessingElement getToProcessingElement(){
		return to;
	}

	/**
	 * Getter for turn style
	 * @return turn style
	 */
	turnStyle getTurnStyle(){
		return turn;
	}
	/**
	 * Sets the street name
	 * @param _name the new name
	 */
	void setName(std::string _name){
			streetName = _name;
	}

	/**
	 * Sets the segments travel distance
	 * @param _distance
	 */
	void setDistance(double _distance){
		distance = _distance;
	}

	/**
	 * Sets the output display message
	 * @param _message the new message
	 */
	void setDisplayMessage(std::string _message){
		displayMessage = _message;
	}

	/**
	 * Stores the street segment transition this represents
	 * @param _from
	 * @param _to
	 */
	void setProcessingElements(StreetSegmentProcessingElement _from, StreetSegmentProcessingElement _to){
		from = _from;
		to = _to;
	}

	/**
	 * Setter for turn style
	 * @param _style
	 */
	void setTurnStyle(turnStyle _style){
		turn  = _style;
	}


	/**
	 * Basic display of object (for debugging purposes only)
	 * @param os stream
	 * @param ide display element
	 * @return reference to stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const InstructionDisplayElement& ide){
		    os << ide.streetName << " " << ide.distance << "m";
		    return os;
	}

private:
	//Fields to display
	std::string displayMessage, streetName;

	//Data
	double distance;
	unsigned numIds;
	turnStyle turn;
	StreetSegmentProcessingElement from, to;
};


#endif /* MAPPER_LIBSTREETMAP_INSTRUCTIONDISPLAYELEMENT_H_ */

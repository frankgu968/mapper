/*
 * UserInstruction.h
 *
 *  Created on: Mar 13, 2016
 *      Author: eddd
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_USERINSTRUCTION_H_
#define MAPPER_LIBSTREETMAP_SRC_USERINSTRUCTION_H_

#include "MapData.h"
#include "MapElement.h"
#include "mapProcessor.h"
#include "string.h"

//Enum to identify type of command
enum instructionTypes{
	Invalid_Command,
	Find_Street,
	Find_Intersection,
	Path_Two_Intersection,
	Path_Intersection_to_POI
};

class UserInstruction {
public:
	virtual ~UserInstruction();

	/**
	 * Creates a "find" instruction
	 * e.g. the user just types in "Bay and College"
	 * @param userInput user's input
	 */
	UserInstruction(std::string userInput);

	/**
	 * Creates a "pathfind" instruction
	 * e.g. User types in "Bay and College" and "University and College"
	 * @param fromString location to start - must intersection
	 * @param destString destination - could be intersection or POI
	 */
	UserInstruction(std::string fromString, std::string toString);

	/**
	 * Returns type of instruction found
	 * Type is Invalid_Command if improper input detected
	 * @return	instruction type
	 */
	instructionTypes getInstructionType(){	return instructionType;	}

	/**
	 * Returns requested id found
	 * @return id
	 */
	int getId(int idx){
		if(idx >= parsedData.size())
			return -1;

		return parsedData[idx];
	}

	/**
	 * If this is a path to a POI, returns the name of the destination POI
	 * @return
	 */
	std::string getPOIName() {
		return poiName;
	}

private:
	instructionTypes instructionType;
	std::vector<unsigned> parsedData;
	std::string poiName; // name of the destination POI, if applicable

	void parseCommand();

	/**
	 * Checks if a street is in database/can be autocompleted to one
	 * @param input string to check
	 * @return index of street, or -1 if not found
	 */
	int parseStreet(std::string input);

	/**Overloaded function
	 * Checks for an intersection in the input, separated by separator value
	 * @param input The string to search
	 * @param streetSeparator the separating keyword ('and" or "&")
	 * @return intersection ID if found, -1 if not found
	 */
	int parseIntersection(std::string input, std::string streetSeperator);

	/**Overloaded function
	 * Looks for an intersection, checking "and" and "&" separators
	 * Returns -1 if no intersection found
	 * @param input the user's query
	 * @return index of intersection found
	 */
	int parseIntersection(std::string input);

	/**
	 * Looks for a POI
	 * @param input string to query for POI
	 * @return index of POI if found
	 */
	int parsePOI(std::string input);

	/**
	 * Capitalizes all separate words in a string
	 * @param input the string to process
	 * @return capitalized string
	 */
	std::string capitalizeAllWords(std::string input);
};

#endif /* MAPPER_LIBSTREETMAP_SRC_USERINSTRUCTION_H_ */

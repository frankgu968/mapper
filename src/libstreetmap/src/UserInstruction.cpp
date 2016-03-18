/*
 * UserInstruction.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: eddd
 */

#include "UserInstruction.h"


/**
 * Creates a "find" instruction
 * e.g. the user just types in "Bay and College"
 * @param userInput user's input
 */
UserInstruction::UserInstruction(std::string _userInput){
	instructionType = Invalid_Command;
	int idx = -1;

	//Check if user entered just a street
	idx = parseStreet(_userInput);

	//Street could be matched
	if(idx >= 0){
		instructionType = Find_Street;
		parsedData.push_back((unsigned)idx);
	}

	//No street, check if user entered an intersection
	else {
		idx = parseIntersection(_userInput);

		//Intersection was found
		if(idx >= 0){
			instructionType = Find_Intersection;
			parsedData.push_back((unsigned)idx);
		}

		//Intersection was not found
		else{

		}
	}


}

/**
 * Creates a "pathfind" instruction
 * e.g. User types in "Bay and College" and "University and College"
 * @param fromString location to start - must intersection
 * @param destString destination - could be intersection or POI
 */
UserInstruction::UserInstruction(std::string fromString, std::string destString){
	instructionType = Invalid_Command;
	int foundIdx1, foundIdx2;

	//Parse from location - must be an intersection
	foundIdx1 = parseIntersection(fromString);

	//Only proceed if a valid starting intersection was found
	if(foundIdx1 != -1){
		//Check if second street is an intersection
		foundIdx2 = parseIntersection(destString);

		//Intersection was found - store
		if(foundIdx2 != -1){
			//Save instruction data
			parsedData.push_back(foundIdx1);
			parsedData.push_back(foundIdx2);

			instructionType = Path_Two_Intersection;
		}

		//Intersection not found - check if second is POI
		else{
			foundIdx2 = parsePOI(destString);

			//POI found - store
			if(foundIdx2 != -1){
				parsedData.push_back(foundIdx1);
				parsedData.push_back(foundIdx2);
				poiName = destString;

				instructionType = Path_Intersection_to_POI;
			}
			else {
				std::cout << "ERROR: Couldn't generate UserInstruction. " << destString << " is an invalid intersection or POI name." << std::endl;
			}
		}
	}
	else {
		std::cout << "ERROR: Couldn't generate UserInstruction. " << fromString << " is an invalid intersection name." << std::endl;
	}
}

UserInstruction::~UserInstruction() {
	// TODO Auto-generated destructor stub
}


/**
 * Returns index of longest road for a vector of road ids
 * @param roadIds id of longest road
 */
unsigned getLongestRoadsIdx(std::vector<unsigned> &roadIds){
	double maxLength = -1;
	unsigned maxIdx = 0;

	//Return index of largest street
	for (unsigned i = 0; i<roadIds.size(); i++){
		double curLength = find_street_length(roadIds[i]);

		//Check if segment is longer
		if(curLength > maxLength){
			maxLength = curLength;
			maxIdx = i;
		}
	}

	return maxIdx;
}

/**
 * Checks if a street is in database/can be autocompleted to one
 * @param input string to check
 * @return index of street, or -1 if not found
 */
int UserInstruction::parseStreet(std::string input){
	//Get complete reference to name
	std::vector<std::string> completedNames = autocompleteAllResults(input);

	//No known object can be made with this name
	if(completedNames.size() == 0)
		return -1;

	//Query for every possible result
	for(unsigned i = 0; i<completedNames.size(); i++){

		//Attempt to query map for this name
		std::vector<unsigned> streetIds = find_street_ids_from_name(completedNames[i]);

		//Return the first valid street found
		if(streetIds.size() != 0){
			return streetIds[getLongestRoadsIdx(streetIds)];
		}
	}

	//No street could be found
	return -1;
}

/**Overloaded function
 * Looks for an intersection, checking "and" and "&" separators
 * Returns -1 if no intersection found
 * @param input the user's query
 * @return index of intersection found
 */
int UserInstruction::parseIntersection(std::string input){
	int idx = -1;

	//Check for empty input
	if(input.size() == 0)
		return -1;

	//Check for an intersection separated by "and"
	idx = parseIntersection(input, "and");

	//Intersection not found - try "&"
	if(idx == -1)
		idx = parseIntersection(input, "&");

	return idx;
}

/**
 * Checks for an intersection in the input
 * @param input The string to search
 * @param streetSeparator the separating keyword ('and" or "&")
 * @return intersection ID if found, -1 if not found
 */
int UserInstruction::parseIntersection(std::string input, std::string streetSeperator){
	unsigned idx = 0;
	bool intersectionFound = false;
	std::vector<unsigned> indices;

	//Search through every instance of the separator found in the input
	while(intersectionFound == false && idx < input.size()){
		//Look for the separator
		size_t foundId = input.find(streetSeperator, idx);

		//Separator not found - exit
		if(foundId == std::string::npos)
			return -1;

		//Try to find an intersection with parsed strings
		//Get separate streets
		std::string rawFirstStreet = input.substr(0,foundId-1);
		std::string rawSecondStreet = input.substr(foundId + streetSeperator.size() + 1, 99999999);

		//Autocorrect the streets
		std::vector<std::string> allFirstStreets = autocompleteAllResults(rawFirstStreet);
		std::vector<std::string> allSecondStreets = autocompleteAllResults(rawSecondStreet);


		//Intersection was found
		if(indices.size() > 0)
			intersectionFound = true;

		//Iterate through all first streets
		for(unsigned firstI = 0; firstI<allFirstStreets.size(); firstI++){
			std::string firstStreet = allFirstStreets[firstI];

			//Iterate through all second streets
			for(unsigned secondI = 0; secondI<allSecondStreets.size(); secondI++){
				std::string secondStreet = allSecondStreets[secondI];

				//Query for intersection
				indices = find_intersection_ids_from_street_names(firstStreet, secondStreet);

				//Intersection was found
				if(indices.size() > 0){
					intersectionFound = true;
					return indices[0];
				}
			}
		}
		//Update index to point to scanned location
		idx = static_cast<unsigned>(foundId) + 1;
	}

	//If code makes it here, no intersection could be found
	return -1;
}

/**
 * Looks for a POI
 * @param input string to query for POI
 * @return index of POI if found
 */
int UserInstruction::parsePOI(std::string input){
	//Get all possible autocompletes for the string
	std::vector<std::string> autocompletes = autocompleteAllResults(input);

	//Iterate through all possible strings
	for(unsigned i = 0; i<autocompletes.size(); i++){
		//Check that string corresponds to valid POI
		nameIDMap* nameMap = MapData::instance()->getPOINameMap();
		auto it = nameMap->find(autocompletes[i]);

		//POI found - return index of an instance of the POI
		if(it != nameMap->end())
			return it->second;
	}

	//No POI could be found
	return -1;
}

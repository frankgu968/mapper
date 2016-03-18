/* 
 * File:   Street.cpp
 * Author: William Kingsford
 * 
 * Created on January 21, 2016, 10:19 AM
 */

#include "Street.h"

Street::Street() {
}

Street::~Street() {
}

/**
* Adds an intersection to the intersection map.
* @param secondStreetID The ID of the street this street intersects with
* @param intersectionID The ID of the intersection formed by the two streets
*/
void Street::addIntersection(unsigned secondStreetID, unsigned intersectionID) {
    intersectionMap.emplace(secondStreetID, intersectionID);
}

/**
* Gets the ID of the intersection of this street and another street.
* @param secondStreetID The ID of the street this street intersects with
* @return A vector of the IDs of the intersection(s) of this street and 
*          the other street. Empty vector if no such intersection exists.
*/
std::vector<unsigned> Street::getIntersection(unsigned secondStreetID) {
    // this takes into account the possibility of two streets having more than
    // one intersection with each other
    auto range = intersectionMap.equal_range(secondStreetID);
    // vector of ID's to return
    std::vector<unsigned> retVal;
    // check that there exists an intersection between the two streets
    if (range.first != intersectionMap.end()) {
        // add all intersections to the return vector
        for (auto iter = range.first; iter != range.second; ++iter) {
        	retVal.push_back(iter->second);
        }
    }
    return retVal;
}
/**
* Adds a street segment to the list of segments that make up this street.
* @param streetSegmentID
*/
void Street::addStreetSegment(unsigned streetSegmentID) {
   this->streetSegmentIDs.push_back(streetSegmentID);
}
/**
* Gets vector of street segment IDs that make up this street.
* @return vector of street segment IDs that make up this street
*/
std::vector<unsigned> Street::getStreetSegmentIDs() {
   return (this->streetSegmentIDs);
}

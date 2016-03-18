/* 
 * File:   Street.h
 * Author: William Kingsford
 *
 * Created on January 21, 2016, 10:19 AM
 */

#ifndef STREET_H
#define STREET_H

#include<vector>
#include<unordered_map>
#include<utility> // for pair
#include<algorithm> // for for_each

/**
 * idMap is a map from unsigned->unsigned, used for mapping one type of ID to another.
 */
typedef std::unordered_multimap<unsigned, unsigned> idMap;

/** 
 * Object to hold pre-processed data structures for streets. Contains intersections
 * that occur on this street, indexed by the ID of the street it intersects with, 
 * and a vector of street segments that make up this street.
 */
class Street {
public:
    Street();
    virtual ~Street();
    
    /**
     * Adds an intersection to the intersection map.
     * @param secondStreetID The ID of the street this street intersects with
     * @param intersectionID The ID of the intersection formed by the two streets
     */
    void addIntersection(unsigned secondStreetID, unsigned intersectionID);
    /**
     * Gets the ID of the intersection of this street and another street.
     * @param secondStreetID The ID of the street this street intersects with
     * @return A vector of the IDs of the intersection(s) of this street and 
     *          the other street. Empty vector if no such intersection exists.
     */
    std::vector<unsigned> getIntersection(unsigned secondStreetID);
    /**
     * Adds a street segment to the list of segments that make up this street.
     * @param streetSegmentID
     */
    void addStreetSegment(unsigned streetSegmentID);
    /**
     * Gets vector of street segment IDs that make up this street.
     * @return vector of street segment IDs that make up this street
     */
    std::vector<unsigned> getStreetSegmentIDs();
private:
    // vector of IDs of all street segments that make up this street
    std::vector<unsigned> streetSegmentIDs;
    // map of intersecting street ID => intersection ID
    idMap intersectionMap;
};

#endif /* STREET_H */


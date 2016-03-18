/*
 * m3.cpp
 *
 *  Created on: 8/03/2016
 *      Author: william
 */

#include "m3.h"

// vector of IDs of POI's matching some constraint (e.g. specific name)
std::vector<unsigned> poiSubsetIDs;

// Returns a path (route) between the start intersection and the end
// intersection, if one exists. If no path exists, this routine returns
// an empty (size == 0) vector. If more than one path exists, the path
// with the shortest travel time is returned. The path is returned as a vector
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned
                   intersect_id_start, unsigned intersect_id_end) {
	// uses A* to find paths between two intersections
	double timeTaken;
	return findShortestPathAstar(intersect_id_start, intersect_id_end, timeTaken);
}

// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path) {
	// check for empty path
	if (path.size() == 0) {
		return 0.0;
	}

	double totalTime = 0;
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	if (streetSegments == NULL) {
		std::cout << "=====MAP NOT LOADED!!!!=====" << std::endl;
		return 0.0;
	}

	// add travel time for first segment
	totalTime += streetSegments[path[0]].getTravelTime();

	// for subsequent street segments, take into account turning distance
	unsigned numSegments = path.size();
	for (unsigned i=1; i<numSegments; i++) {
		totalTime += streetSegments[path[i]].getTravelTime();
		// check for turn
		if (streetSegments[path[i-1]].getInfo().streetID != streetSegments[path[i]].getInfo().streetID) {
			totalTime += 0.25;
		}
	}

	return totalTime;
}

// Returns the shortest travel time path (vector of street segments) from
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned
                   intersect_id_start, std::string point_of_interest_name) {
	// Very fast method:
	// 1) Use ANN to find geographically closest point of interest matching the name
	// 2) Use A* to find shortest time to this point
	// 3) Use ANN to find POIs matching the name, within a radius equal to current shortest time * speed limit,
	// 			i.e. the widest radius a better solution could possibly be within
	// 4) Depending on how many points there are:
	//    (few points) Use A* to find which of these points is the fastest and return the answer
	//    (many points) Use Dijkstra to find which of these points is the fastest and return the answer

	// the MapData instance
	MapData* map = MapData::instance();
	// the POI objects of the map
	PoiXY** poiObjects = map->getPointsOfInterest();
	// the intersection objects of the map (small intersections contains all intersections)
	IntersectionXY** intersectionObjects = map->getSmallIntersections();

	// the path to return
	std::vector<unsigned> bestPath;

	// first we get the IDs of the POI's and put them into the vector poiSubsetIDs

	// unordered_multimap of street names => street ids
	nameIDMap* poiNameMap = MapData::instance()->getPOINameMap();
	// iterator for street IDs that match the street name
	auto range = poiNameMap->equal_range(point_of_interest_name);

	// empty the existing vector
	poiSubsetIDs.clear();

	// push these names onto the ID vector
	for (auto iter = range.first; iter != range.second; ++iter) {
		poiSubsetIDs.push_back(iter->second);
	}

	// check that any POIs matched
	if (poiSubsetIDs.size() == 0) {
		return bestPath;
	}
	// if there's only 1 of this POI, return the path to it
	else if (poiSubsetIDs.size() == 1) {
		// find the intersection that's closest to this POI
		unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[0]]->getCoordinates()[0]);
		return find_path_between_intersections(intersect_id_start, nearestIntersection);
	}
	// else we have more than one POI of this type

	// Now we initialize an ANN kd tree for this POI name, allowing us to quickly find nearby POIs with this name
	// ANNInterface requires a function pointer that, given the index of an element, returns its PointXY location.
	ANNInterface *singlePOITree = new ANNInterface (poiSubsetIDs.size(), &getPOIPositionFromSubset);

	// use this ANNInterface to find the matching POI that's closest geographically to the start intersection
	PointXY startPosition = intersectionObjects[intersect_id_start]->getCoordinates()[0];
	unsigned bestPOIIndex = (unsigned) singlePOITree->findClosestPoint(startPosition);
	unsigned bestPOIID = poiSubsetIDs[bestPOIIndex];

	// find the intersection that's closest to this POI
	unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[bestPOIID]->getCoordinates()[0]);

	// find the path to this closest intersection

	//bestPath = find_path_between_intersections(intersect_id_start, nearestIntersection);
	double shortestTimeTaken;
	bestPath = findShortestPathAstar(intersect_id_start, nearestIntersection, shortestTimeTaken);
	/*if (bestPath.size() != 0) {
		shortestTimeTaken = compute_path_travel_time(bestPath); // in minutes
	}
	else {
		shortestTimeTaken = FLT_MAX;
	}*/

	// now do a radius search to find all matching POIs that are capable of being better than this one
	int numMatches; // ANN modifies this to hold the correct value
	// largest possible distance that a POI could be within and be faster (ideal is one straight street segment at maximum speed limit)
	double radius = shortestTimeTaken * 1000 * SPEED_LIMIT / 60; // shortestTimeTaken is minutes, SPEED_LIMIT is km/h, looking for answer in meters

	int* possiblyBetterPOIIndices = singlePOITree->findPointsWithinRadius(startPosition, radius*radius, numMatches);

	// if there is only 1 point, we have already found the best path
	if (numMatches == 1) {
		delete singlePOITree;
		return bestPath;
	}
	// if there are only a small number of points, A* will be faster
	else if (numMatches <= 3) {
		for (unsigned i=0; i<numMatches; i++) {
			// don't re-evaluate the one we already have
			if (possiblyBetterPOIIndices[i] != bestPOIIndex) {
				nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[possiblyBetterPOIIndices[i]]]->getCoordinates()[0]);
				//std::vector<unsigned> possiblyBest = find_path_between_intersections(intersect_id_start, nearestIntersection);
				double nextTimeTaken;
				std::vector<unsigned> possiblyBest = findShortestPathAstar(intersect_id_start, nearestIntersection, nextTimeTaken);
				// if it's faster than the current best, replace it
				//if (compute_path_travel_time(possiblyBest) < shortestTimeTaken) {
				if (nextTimeTaken < shortestTimeTaken) {
					//shortestTimeTaken = compute_path_travel_time(possiblyBest);
					shortestTimeTaken = nextTimeTaken;
					bestPath = possiblyBest;
				}
			}
		}

		delete singlePOITree;
		return bestPath;
	}
	// if there are many points, use multiple-point Dijkstra's algorithm
	else {
		// generate set of end intersections
		std::set<unsigned> endIntersectionIDs;
		for (unsigned i=0; i < numMatches; i++) {
			unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[possiblyBetterPOIIndices[i]]]->getCoordinates()[0]);
			endIntersectionIDs.insert(nearestIntersection);
		}

		// find best path to any of these intersections
		bestPath = findShortestPathDijkstraMultipleTarget(intersect_id_start, endIntersectionIDs);

		// we have now found the best path
		delete singlePOITree;
		return bestPath;
	}
}

// older version of find_path_to_point_of_interest, the naive solution that checks all matching POIs
std::vector<unsigned> find_path_to_point_of_interest_basic (unsigned
                   intersect_id_start, std::string point_of_interest_name) {
	// the MapData instance
	MapData* map = MapData::instance();
	// the POI objects of the map
	PoiXY** poiObjects = map->getPointsOfInterest();

	// the path to return
	std::vector<unsigned> bestPath;

	// first we get the IDs of the POI's and put them into the vector poiSubsetIDs

	// unordered_multimap of street names => street ids
	nameIDMap* poiNameMap = MapData::instance()->getPOINameMap();
	// iterator for street IDs that match the street name
	auto range = poiNameMap->equal_range(point_of_interest_name);

	// empty the existing vector
	poiSubsetIDs.clear();

	// push these names onto the ID vector
	for (auto iter = range.first; iter != range.second; ++iter) {
		poiSubsetIDs.push_back(iter->second);
	}

	// check that any POIs matched
	if (poiSubsetIDs.size() == 0) {
		return bestPath;
	}

	unsigned bestPOIID = poiSubsetIDs[0];

	// find the intersection that's closest to this POI
	unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[bestPOIID]->getCoordinates()[0]);

	// find the path to this closest intersection
	std::cout << "About to find first path" << std::endl;
	bestPath = find_path_between_intersections(intersect_id_start, nearestIntersection);
	double shortestTimeTaken;
	// check if a path exists
	if (bestPath.size() != 0) {
		shortestTimeTaken = compute_path_travel_time(bestPath); // in minutes
	}
	else {
		shortestTimeTaken = FLT_MAX;
	}

	// check all possible POI's

	// holds a reference to the current best path, so that we only have to do a full-copy of the array of street segments once at the end
	for (unsigned i=1; i < poiSubsetIDs.size(); i++) {
		// find closest intersection to this target POI
		unsigned trialIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[i]]->getCoordinates()[0]);

		// then find the fastest path to it
		std::vector<unsigned> trialPath = find_path_between_intersections(intersect_id_start, trialIntersection);
		// check if a path exists
		if (trialPath.size() != 0) {
			double trialTimeTaken = compute_path_travel_time(trialPath);

			// if this was better, substitute this as the new bestPOI
			if (trialTimeTaken < shortestTimeTaken) {
				shortestTimeTaken = trialTimeTaken;
				bestPOIID = poiSubsetIDs[i];
				bestPath = trialPath;
				nearestIntersection = trialIntersection;
			}
		}
	}

	// we have now found the best path
	return bestPath;
}

// Older version of find_path_to_point_of_interest, uses only ANN and not multipath Dijkstra
std::vector<unsigned> find_path_to_point_of_interest_ANN (unsigned
                   intersect_id_start, std::string point_of_interest_name) {
	// the MapData instance
	MapData* map = MapData::instance();
	// the POI objects of the map
	PoiXY** poiObjects = map->getPointsOfInterest();
	// the intersection objects of the map (small intersections contains all intersections)
	IntersectionXY** intersectionObjects = map->getSmallIntersections();

	// the path to return
	std::vector<unsigned> bestPath;

	// first we get the IDs of the POI's and put them into the vector poiSubsetIDs

	// unordered_multimap of street names => street ids
	nameIDMap* poiNameMap = MapData::instance()->getPOINameMap();
	// iterator for street IDs that match the street name
	auto range = poiNameMap->equal_range(point_of_interest_name);

	// empty the existing vector
	poiSubsetIDs.clear();

	// push these names onto the ID vector
	for (auto iter = range.first; iter != range.second; ++iter) {
		poiSubsetIDs.push_back(iter->second);
	}

	// check that any POIs matched
	if (poiSubsetIDs.size() == 0) {
		return bestPath;
	}
	// if there's only 1 of this POI, return the path to it
	else if (poiSubsetIDs.size() == 1) {
		// find the intersection that's closest to this POI
		unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[0]]->getCoordinates()[0]);
		return find_path_between_intersections(intersect_id_start, nearestIntersection);
	}
	// else we have more than one POI of this type

	// Now we initialize an ANN kd tree for this POI name, allowing us to quickly find nearby POIs with this name
	// ANNInterface requires a function pointer that, given the index of an element, returns its PointXY location.
	ANNInterface *singlePOITree = new ANNInterface (poiSubsetIDs.size(), &getPOIPositionFromSubset);

	// use this ANNInterface to find the matching POI that's closest geographically to the start intersection
	PointXY startPosition = intersectionObjects[intersect_id_start]->getCoordinates()[0];
	unsigned bestPOIIndex = (unsigned) singlePOITree->findClosestPoint(startPosition);
	unsigned bestPOIID = poiSubsetIDs[bestPOIIndex];

	// find the intersection that's closest to this POI
	unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[bestPOIID]->getCoordinates()[0]);

	// find the path to this closest intersection
	// TODO: MAKE THIS FASTER BY ONLY DELETING RESULTS FROM GRAPH SEARCH WHEN THE STARTINTERSECTION CHANGES. THIS ALSO MAKES TRAVELLING SALESMAN MUCH FASTER
	bestPath = find_path_between_intersections(intersect_id_start, nearestIntersection);
	double shortestTimeTaken;
	// check if a path exists
	if (bestPath.size() != 0) {
		shortestTimeTaken = compute_path_travel_time(bestPath); // in minutes
	}
	else {
		shortestTimeTaken = FLT_MAX;
	}

	// now do a radius search to find all matching POIs that are capable of being better than this one
	int numMatches; // ANN modifies this to hold the correct value
	// largest possible distance that a POI could be within and be faster (ideal is one straight street segment at maximum speed limit)
	double radius = shortestTimeTaken * 1000 * SPEED_LIMIT / 60; // shortestTimeTaken is minutes, SPEED_LIMIT is km/h, looking for answer in meters

	int* possiblyBetterPOIIndices = singlePOITree->findPointsWithinRadius(startPosition, radius*radius, numMatches);

	// check all possible POI's

	// holds a reference to the current best path, so that we only have to do a full-copy of the array of street segments once at the end
	for (int i=0; i < numMatches; i++) {
		// don't re-check the starting index
		if (possiblyBetterPOIIndices[i] == bestPOIIndex) {
			continue;
		}

		// otherwise, find closest intersection to this target POI
		unsigned trialIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[possiblyBetterPOIIndices[i]]]->getCoordinates()[0]);

		// then find the fastest path to it
		std::vector<unsigned> trialPath = find_path_between_intersections(intersect_id_start, trialIntersection);
		// check if a path exists
		if (trialPath.size() != 0) {
			double trialTimeTaken = compute_path_travel_time(trialPath);

			// if this was better, substitute this as the new bestPOI
			if (trialTimeTaken < shortestTimeTaken) {
				shortestTimeTaken = trialTimeTaken;
				bestPOIID = poiSubsetIDs[possiblyBetterPOIIndices[i]];
				bestPath = trialPath;
				nearestIntersection = trialIntersection;
			}
		}
	}

	// we have now found the best path
	delete singlePOITree;
	return bestPath;
}

// older version of find_path_to_point_of_interest, uses multipath Dijkstra but not ANN
std::vector<unsigned> find_path_to_point_of_interest_MULTIPATH (unsigned
                   intersect_id_start, std::string point_of_interest_name) {
	// the MapData instance
	MapData* map = MapData::instance();
	// the POI objects of the map
	PoiXY** poiObjects = map->getPointsOfInterest();

	// the path to return
	std::vector<unsigned> bestPath;

	// first we get the IDs of the POI's and put them into the vector poiSubsetIDs

	// unordered_multimap of street names => street ids
	nameIDMap* poiNameMap = MapData::instance()->getPOINameMap();
	// iterator for street IDs that match the street name
	auto range = poiNameMap->equal_range(point_of_interest_name);

	// empty the existing vector
	poiSubsetIDs.clear();

	// push these names onto the ID vector
	for (auto iter = range.first; iter != range.second; ++iter) {
		poiSubsetIDs.push_back(iter->second);
	}

	// check that any POIs matched
	if (poiSubsetIDs.size() == 0) {
		return bestPath;
	}

	// generate set of end intersections
	std::set<unsigned> endIntersectionIDs;
	for (unsigned i=0; i < poiSubsetIDs.size(); i++) {
		unsigned nearestIntersection = (unsigned) find_closest_point(SmallIntersections, poiObjects[poiSubsetIDs[i]]->getCoordinates()[0]);
		endIntersectionIDs.insert(nearestIntersection);
	}

	// find best path to any of these intersections
	return findShortestPathDijkstraMultipleTarget(intersect_id_start, endIntersectionIDs);
}

/**
 * Given an index, returns the location of the corresponding POI from the poiSubsetIDs vector
 * @param i The index from the subset array
 * @return PointXY location
 */
PointXY getPOIPositionFromSubset(unsigned i) {
	PoiXY** poiObjects = MapData::instance()->getPointsOfInterest();
	PointXY position = poiObjects[poiSubsetIDs[i]]->getCoordinates()[0];
	return position;
}

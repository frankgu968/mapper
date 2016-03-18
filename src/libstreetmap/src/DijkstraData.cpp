/*
 * DijkstraData.cpp
 *
 *  Created on: 10/03/2016
 *      Author: william
 */

#include "DijkstraData.h"

DijkstraData* DijkstraData::member = NULL;

/**
 * Get the single DijkstraData instance
 * @return
 */
DijkstraData* DijkstraData::instance() {
	if (!member) {
		member = new DijkstraData();
	}

	return DijkstraData::member;
}

/**
 * Initialize all values required for a Djikstra search. Use this before the first search and every time a subsequent search
 * involves a new start intersection
 * @param startIntersectionID
 */
void DijkstraData::initialize(unsigned startIntersectionID, unsigned numTotalIntersections) {
	// update startID
	dijkstraPrevStartIntersectionID = startIntersectionID;

	// add the starting intersection to the queue
	dijkstraOpen.push(std::make_pair(startIntersectionID, 0.0));

	// set of nodes that have been expanded, so that we don't visit them again
	dijkstraVisited = new bool[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		dijkstraVisited[i] = false;
	}

	// contains time to reach from start and first step of fastest path back for each intersection that has been visited
	dijkstraTimeToIntersection = new double[numTotalIntersections];
	dijkstraPrevIntersection = new unsigned[numTotalIntersections];
	dijkstraPrevStreetSeg = new unsigned[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		dijkstraTimeToIntersection[i] = PRACTICALLY_INFINITE;
		dijkstraPrevIntersection[i] = INT_MAX;
		dijkstraPrevStreetSeg[i] = INT_MAX;
	}
	dijkstraTimeToIntersection[startIntersectionID] = 0.0;
}

/**
 * Get the current start intersection ID (used to check if we can reuse old data)
 * @return
 */
unsigned DijkstraData::getPrevStartIntersectionID() {
	return dijkstraPrevStartIntersectionID;
}

/**
 * Whether or not an intersection has been visited by Dijkstra in this run
 * @param i The intersection ID
 * @return
 */
bool DijkstraData::hasBeenVisited(unsigned i) {
	return dijkstraVisited[i];
}

/**
 * Set that an intersection has been visited by Dijkstra in this run
 * @param i The intersection ID
 * @return
 */
void DijkstraData::setVisited(unsigned i) {
	dijkstraVisited[i] = true;
}

/**
 * Return the intersectionID of the lowest time-to-reach element of the open queue, remove it from the open queue
 */
unsigned DijkstraData::popOpen() {
	unsigned toReturn = dijkstraOpen.top().first;
	dijkstraOpen.pop();
	return toReturn;
}

/**
 * Add the given element to the open queue
 * @param intersectionID The ID of the intersection to add to the queue
 * @param timeToReach The best guess at total time to reach this intersection from the start intersection
 */
void DijkstraData::pushOpen(unsigned intersectionID, double timeToReach) {
	dijkstraOpen.push(std::make_pair(intersectionID, timeToReach));
}

/**
 * Check whether the open queue is empty or not
 * @return
 */
bool DijkstraData::openIsEmpty() {
	return dijkstraOpen.empty();
}

/**
 * Set the values corresponding to the path back from an intersection
 * @param i The ID of the intersection to set values for
 * @param returnIntersection The intersection immediately preceding this one in the fastest path
 * @param returnStreetSegment The street segment used to get to this one in the fastest path
 * @param timeToReach The fastest time to reach this intersection
 */
void DijkstraData::setReturnPath(unsigned i, unsigned returnIntersection, unsigned returnStreetSegment, double timeToReach) {
	dijkstraTimeToIntersection[i] = timeToReach;
	dijkstraPrevIntersection[i] = returnIntersection;
	dijkstraPrevStreetSeg[i] = returnStreetSegment;
}

/**
 * Get the intersection immediately preceding this one in the fastest path
 * @param i The ID of the intersection to get back from
 * @return Intersection ID
 */
unsigned DijkstraData::getReturnIntersection(unsigned i) {
	assert(dijkstraPrevIntersection[i] != INT_MAX);
	return dijkstraPrevIntersection[i];
}

/**
 * Get the street segment immediately preceding this one in the fastest path
 * @param i The ID of the intersection to get back from
 * @return Street segment ID
 */
unsigned DijkstraData::getReturnStreetSeg(unsigned i) {
	assert(dijkstraPrevStreetSeg[i] != INT_MAX);
	return dijkstraPrevStreetSeg[i];
}

/**
 * Get the fastest known time to reach this intersection
 * @param i The ID of the intersection to find time for
 * @return
 */
double DijkstraData::getTimeToReach(unsigned i) {
	return dijkstraTimeToIntersection[i];
}

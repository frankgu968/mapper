/*
 * DijkstraData.h
 *
 *  Created on: 10/03/2016
 *      Author: william
 */

#ifndef DIJKSTRADATA_H
#define DIJKSTRADATA_H

#include <queue>
#include <cstdlib>
#include <climits>
#include <assert.h>

#define PRACTICALLY_INFINITE 1e+30

/**
 * This file contains data for Dijkstra search, and corresponding getters, so that the data from each search can be
 * re-used for the last if they have the same start intersection.
 */

// used by Dijkstra and A* to allow our priority queue to put the pair with the lowest time at the top
class CompareHeuristic {
public:
	bool operator()(std::pair<unsigned, double> a, std::pair<unsigned,double> b) {
		return a.second > b.second;
	}
};

/**
 * Singleton containing data for Dijkstra graph search
 */
class DijkstraData {
public:
	/**
	 * Get the single DijkstraData instance
	 * @return
	 */
	static DijkstraData* instance();

	/**
	 * Initialize all values required for a Djikstra search. Use this before the first search and every time a subsequent search
	 * involves a new start intersection
	 * @param startIntersectionID
	 * @param numTotalIntersections
	 */
	void initialize(unsigned startIntersectionID, unsigned numTotalIntersections);

	/**
	 * Get the current start intersection ID (used to check if we can reuse old data)
	 * @return
	 */
	unsigned getPrevStartIntersectionID();

	/**
	 * Whether or not an intersection has been visited by Dijkstra in this run
	 * @param i The intersection ID
	 * @return
	 */
	bool hasBeenVisited(unsigned i);

	/**
	 * Set that an intersection has been visited by Dijkstra in this run
	 * @param i The intersection ID
	 * @return
	 */
	void setVisited(unsigned i);

	/**
	 * Return the intersectionID of the lowest time-to-reach element of the open queue, remove it from the open queue
	 */
	unsigned popOpen();

	/**
	 * Add the given element to the open queue
	 * @param intersectionID The ID of the intersection to add to the queue
	 * @param timeToReach The best guess at total time to reach this intersection from the start intersection
	 */
	void pushOpen(unsigned intersectionID, double timeToReach);

	/**
	 * Check whether the open queue is empty or not
	 * @return
	 */
	bool openIsEmpty();

	/**
	 * Set the values corresponding to the path back from an intersection
	 * @param i The ID of the intersection to set values for
	 * @param returnIntersection The intersection immediately preceding this one in the fastest path
	 * @param returnStreetSegment The street segment used to get to this one in the fastest path
	 * @param timeToReach The fastest time to reach this intersection
	 */
	void setReturnPath(unsigned i, unsigned returnIntersection, unsigned returnStreetSegment, double timeToReach);

	/**
	 * Get the intersection immediately preceding this one in the fastest path
	 * @param i The ID of the intersection to get back from
	 * @return Intersection ID
	 */
	unsigned getReturnIntersection(unsigned i);

	/**
	 * Get the street segment immediately preceding this one in the fastest path
	 * @param i The ID of the intersection to get back from
	 * @return Street segment ID
	 */
	unsigned getReturnStreetSeg(unsigned i);

	/**
	 * Get the fastest known time to reach this intersection
	 * @param i The ID of the intersection to find time for
	 * @return
	 */
	double getTimeToReach(unsigned i);

	/**
	 * Destructor, free all memory
	 */
	~DijkstraData() {
		delete[] dijkstraVisited;
		delete[] dijkstraTimeToIntersection;
		delete[] dijkstraPrevIntersection;
		delete[] dijkstraPrevStreetSeg;
	}
private:
	/**
	 * Private default constructor
	 */
	DijkstraData() {
		dijkstraPrevStartIntersectionID = INT_MAX;
		dijkstraVisited = NULL;
		dijkstraTimeToIntersection = NULL;
		dijkstraPrevIntersection = NULL;
		dijkstraPrevStreetSeg = NULL;
	}

	/**
	 * The single instance of DijkstraData
	 */
	static DijkstraData* member;

	/**
	 * Data members
	 */
	// previous starting intersection
	unsigned dijkstraPrevStartIntersectionID;
	// priority queue for nodes that have not yet been visited
	std::priority_queue<std::pair<unsigned, double>, std::vector<std::pair<unsigned, double>>, CompareHeuristic> dijkstraOpen;
	// array of size = total number of intersections, tracks whether each intersection has already been visited by dijkstra
	bool* dijkstraVisited;
	// to keep track of the path back to the start intersection
	double *dijkstraTimeToIntersection;
	unsigned *dijkstraPrevIntersection;
	unsigned *dijkstraPrevStreetSeg;
};

#endif /* DIJKSTRADATA_H */

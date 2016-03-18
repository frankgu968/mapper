/*
 * ShortestPath.h
 *
 *  Created on: 7/03/2016
 *      Author: william
 */

#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <stdlib.h>
#include <queue>
#include <set>
#include <map>
#include <limits> // for DBL_MAX for one-way street travel times
#include "MapData.h"
#include "DijkstraData.h"

/*
 * ====== BEST METHODS ======
 */

/**
 * Finds shortest path between one intersection and multiple points using Dijkstra's algorithm and returns the path to the closest one.
 * This is significantly faster than separately using A* or Dijkstra on each one and comparing the results
 * @param startIntersectionID
 * @param endIntersectionIDs A set of potential end intersections
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathDijkstraMultipleTarget(unsigned startIntersectionID, std::set<unsigned> endIntersectionIDs);

/**
 * Finds shortest path between two intersections using A* algorithm (best for when we only care about the path to the one destination)
 * @param startIntersectionID
 * @param endIntersectionID
 * @param bestTime RETURN PARAMETER: This will contain the shortest time to get to the destination
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathAstar(unsigned startIntersectionID, unsigned endIntersectionID, double& bestTime);

/**
 * Calculates the time taken to travel from one intersection to another, as the crow flies, at the maximum speed limit
 * @param cur Start intersection
 * @param dest End intersection
 * @return Time in seconds
 */
double _timeBetweenTwoIntersections(IntersectionXY* cur, IntersectionXY* dest);

/*
 * ====== FUTURE METHODS ======
 * (retains set of dijkstra data between iterations with the same startIntersectionID, aiming to have this working for m4
 * to make travelling salesman more efficient)
 */

/**
 * TODO: WORK ON SINGLETON DIJKSTRA! THIS WILL PROBABLY BE VALUABLE FOR M4
 * @param startIntersectionID
 * @param endIntersectionID
 * @return
 */
std::vector<unsigned> findShortestPathDijkstraSingleton(unsigned startIntersectionID, unsigned endIntersectionID);

/**
 * When a path between two intersections has been found, this function can be used to return it as a vector of street segment IDs
 * @param startIntersectionID
 * @param endIntersectionID
 * @return
 */
std::vector<unsigned> dijkstraReturnPathBetweenIntersections(unsigned startIntersectionID, unsigned endIntersectionID);

/**
 * When a path between two intersections has been found, this function can be used to return the time taken to reach the end intersection
 * @param endIntersectionID The intersectionID of an intersection that has been visited by findShortestPathDijkstra
 * @return The time (in minutes) taken to reach the intersection
 */
double dijkstaReturnTimeToIntersection(unsigned endIntersectionID);

/*
 * ====== PAST VERSIONS ======
 * (worse than current functions, but retained just in case)
 */

/**
 * Finds shortest path between two intersections using Dijkstra's algorithm. IN PRACTICE DON'T USE THIS, EITHER USE findShortestPathAstar
 * FOR SINGLE DESINATIONS OR findShortestPathDijkstraMultipleTarget FOR MULTIPLE POSSIBLE DESTINATIONS
 * @param startIntersectionID
 * @param endIntersectionID
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathDijkstra(unsigned startIntersectionID, unsigned endIntersectionID);


/**
 * Finds shortest path between two intersections using A* algorithm (best for when we only care about the time to the one destination)
 * This version was an attempt at optimizing by storing the heuristic associated with each intersection so we don't have to recalculate
 * it each time, but ended up being slower. Retained for posterity.
 * @param startIntersectionID
 * @param endIntersectionID
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathAstar_storedHeuristic(unsigned startIntersectionID, unsigned endIntersectionID);

#endif /* SHORTESTPATH_H */

/*
 * ShortestPath.cpp
 *
 *  Created on: 7/03/2016
 *      Author: william
 */

#include "ShortestPath.h"

/**
 * Finds shortest path between two intersections using Dijkstra's algorithm. IN PRACTICE DON'T USE THIS, EITHER USE findShortestPathAstar
 * FOR SINGLE DESINATIONS OR findShortestPathDijkstraMultipleTarget FOR MULTIPLE POSSIBLE DESTINATIONS
 * @param startIntersectionID
 * @param endIntersectionID
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathDijkstra(unsigned startIntersectionID, unsigned endIntersectionID) {
	// priority queue for nodes that have not yet been visited
	std::priority_queue<std::pair<unsigned, double>, std::vector<std::pair<unsigned, double>>, CompareHeuristic> open;
	// add the starting intersection to the queue
	open.push(std::make_pair(startIntersectionID, 0.0));

	// set of nodes that have been expanded, so that we don't visit them again
	unsigned numTotalIntersections = MapData::instance()->getNumIntersections();
	bool* visited = new bool[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		visited[i] = false;
	}

	// contains time to reach from start and first step of fastest path back for each intersection that has been visited
	double *timeToIntersection = new double[numTotalIntersections];
	unsigned *prevIntersection = new unsigned[numTotalIntersections];
	unsigned *prevStreetSeg = new unsigned[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		timeToIntersection[i] = FLT_MAX;
		prevIntersection[i] = INT_MAX;
		prevStreetSeg[i] = INT_MAX;
	}
	timeToIntersection[startIntersectionID] = 0.0;

	IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	// loop until there are no more undiscovered nodes
	while (!open.empty()) {
		// pop the intersection with the lowest time to reach + heuristic time to destination
		std::pair<unsigned, double> current = open.top();
		open.pop();

		// since we update the priority of given intersections by pushing the new, faster one, we have to check if the
		// one we just popped has already been processed and this was just the version with the lower priority
		if (visited[current.first]) {
			continue;
		}

		visited[current.first] = true;

		// if this is the end intersection, we must have taken the fastest path to reach it
		if (current.first == endIntersectionID) {
			break;
		}

		// expand this node's neighbors
		unsigned* neighborSegments = intersections[current.first]->getStreetSegs();
		unsigned numNeighbors = intersections[current.first]->getNumStreetSegs();
		std::vector<unsigned> *neighborIntersections = intersections[current.first]->getConnectedIntersections();
		for (unsigned i=0; i<numNeighbors; i++) {
			// check if this intersection has been expanded before. If so, skip it
			if (!visited[(*neighborIntersections)[i]]) {
				// make sure we're not going the wrong way along this street segment
				if (streetSegments[neighborSegments[i]].getInfo().oneWay && streetSegments[neighborSegments[i]].getInfo().from != current.first) {
					continue;
				}

				// calculate current-best distance to this neighbor
				double timeToNextNode = timeToIntersection[current.first] + streetSegments[neighborSegments[i]].getTravelTime();
				// if there's a turn, add that time. There can never be a turn from the first intersection
				if (current.first != startIntersectionID) {
					if (streetSegments[prevStreetSeg[current.first]].getInfo().streetID !=
							streetSegments[neighborSegments[i]].getInfo().streetID) {
						timeToNextNode += 0.25; // 15 seconds
					}
				}

				// update path to this node, if it's better than any existing way there
				if (timeToNextNode < timeToIntersection[(*neighborIntersections)[i]]) {
					// update path back
					timeToIntersection[(*neighborIntersections)[i]] = timeToNextNode;
					prevStreetSeg[(*neighborIntersections)[i]] = neighborSegments[i];
					prevIntersection[(*neighborIntersections)[i]] = current.first;
					// insert new entry into priority queue, priority is set by heuristic time to destination
					open.push(std::make_pair((*neighborIntersections)[i], timeToNextNode));
				}
			}
		}
	}

	// reset all the intersections' heuristics
	for (unsigned i=0; i<numTotalIntersections; i++) {
		intersections[i]->unsetHeuristic();
	}

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	if (timeToIntersection[endIntersectionID] == FLT_MAX) {
		// no path was found
		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = endIntersectionID;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(prevStreetSeg[cur]);

			cur = prevIntersection[cur];

			i++;
		}

		// reverse list so it's in correct order
		toReturn.resize(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
}

/**
 * Finds shortest path between one intersection and multiple points using Dijkstra's algorithm and returns the path to the closest one.
 * This is significantly faster than separately using A* or Dijkstra on each one and comparing the results
 * @param startIntersectionID
 * @param endIntersectionIDs A set of potential end intersections
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathDijkstraMultipleTarget(unsigned startIntersectionID, std::set<unsigned> endIntersectionIDs) {
	// priority queue for nodes that have not yet been visited
	std::priority_queue<std::pair<unsigned, double>, std::vector<std::pair<unsigned, double>>, CompareHeuristic> open;
	// add the starting intersection to the queue
	open.push(std::make_pair(startIntersectionID, 0.0));

	// set of nodes that have been expanded, so that we don't visit them again
	unsigned numTotalIntersections = MapData::instance()->getNumIntersections();
	bool* visited = new bool[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		visited[i] = false;
	}

	// contains time to reach from start and first step of fastest path back for each intersection that has been visited
	double *timeToIntersection = new double[numTotalIntersections];
	unsigned *prevIntersection = new unsigned[numTotalIntersections];
	unsigned *prevStreetSeg = new unsigned[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		timeToIntersection[i] = FLT_MAX;
		prevIntersection[i] = INT_MAX;
		prevStreetSeg[i] = INT_MAX;
	}
	timeToIntersection[startIntersectionID] = 0.0;

	// current best time to a destination and current best destination
	unsigned closestIntersection = INT_MAX;
	double currentBestTime = FLT_MAX;

	IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	// loop until there are no more undiscovered nodes
	while (!open.empty()) {
		// pop the intersection with the lowest time to reach + heuristic time to destination
		std::pair<unsigned, double> current = open.top();
		open.pop();

		// since we update the priority of given intersections by pushing the new, faster one, we have to check if the
		// one we just popped has already been processed and this was just the version with the lower priority
		if (visited[current.first]) {
			continue;
		}

		visited[current.first] = true;

		// if this is the end intersection, we must have taken the fastest path to reach it
		auto it = endIntersectionIDs.find(current.first);
		if (it != endIntersectionIDs.end()) {
			// remove current from targets set
			endIntersectionIDs.erase(it);
			// check if the time to this node was better than the current best
			if (timeToIntersection[current.first] < currentBestTime) {
				currentBestTime = timeToIntersection[current.first];
				closestIntersection = current.first;
			}
			// if there are no more target intersections, we are done
			if (endIntersectionIDs.empty()) {
				break;
			}
		}

		// expand this node's neighbors
		unsigned* neighborSegments = intersections[current.first]->getStreetSegs();
		unsigned numNeighbors = intersections[current.first]->getNumStreetSegs();
		std::vector<unsigned> *neighborIntersections = intersections[current.first]->getConnectedIntersections();
		for (unsigned i=0; i<numNeighbors; i++) {
			// check if this intersection has been expanded before. If so, skip it
			if (!visited[(*neighborIntersections)[i]]) {
				// make sure we're not going the wrong way along this street segment
				if (streetSegments[neighborSegments[i]].getInfo().oneWay && streetSegments[neighborSegments[i]].getInfo().from != current.first) {
					continue;
				}

				// calculate current-best distance to this neighbor
				double timeToNextNode = timeToIntersection[current.first] + streetSegments[neighborSegments[i]].getTravelTime();
				// if there's a turn, add that time. There can never be a turn from the first intersection
				if (current.first != startIntersectionID) {
					if (streetSegments[prevStreetSeg[current.first]].getInfo().streetID !=
							streetSegments[neighborSegments[i]].getInfo().streetID) {
						timeToNextNode += 0.25; // 15 seconds
					}
				}

				// update path to this node, if it's better than any existing way there
				if (timeToNextNode < timeToIntersection[(*neighborIntersections)[i]]) {
					// update path back
					timeToIntersection[(*neighborIntersections)[i]] = timeToNextNode;
					prevStreetSeg[(*neighborIntersections)[i]] = neighborSegments[i];
					prevIntersection[(*neighborIntersections)[i]] = current.first;
					// insert new entry into priority queue, priority is set by heuristic time to destination
					open.push(std::make_pair((*neighborIntersections)[i], timeToNextNode));
				}
			}
		}
	}

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	if (currentBestTime == FLT_MAX) {
		// no path was found
		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = closestIntersection;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(prevStreetSeg[cur]);

			cur = prevIntersection[cur];

			i++;
		}

		// reverse list so it's in correct order
		toReturn.resize(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
}

/**
 * TODO: WORK ON SINGLETON DIJKSTRA! THIS WILL PROBABLY BE VALUABLE FOR M4
 * @param startIntersectionID
 * @param endIntersectionID
 * @return
 */
std::vector<unsigned> findShortestPathDijkstraSingleton(unsigned startIntersectionID, unsigned endIntersectionID) {
	DijkstraData* data = DijkstraData::instance();
	unsigned numTotalIntersections = MapData::instance()->getNumIntersections();

	// check if data already exists, if not then initialize it
	if (startIntersectionID != data->getPrevStartIntersectionID()) {
		//std::cout << "Resetting Dijkstra" << std::endl;
		delete data;
		data = DijkstraData::instance();
		data->initialize(startIntersectionID, numTotalIntersections);
	}
	// data already exists, check if the path we're looking for has already been found
	else if (data->hasBeenVisited(endIntersectionID)) {
		//std::cout << "Returning precalculated value" << std::endl;
		return dijkstraReturnPathBetweenIntersections(startIntersectionID, endIntersectionID);
	}
	//std::cout << "Calculating value" << std::endl;

	// we're either building off old data or using newly instantiated variables, it makes no difference

	IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	// loop until there are no more undiscovered nodes
	while (!data->openIsEmpty()) {
		// pop the intersection with the lowest time to reach + heuristic time to destination
		unsigned current = data->popOpen();

		// since we update the priority of given intersections by pushing the new, faster one, we have to check if the
		// one we just popped has already been processed and this was just the version with the lower priority
		if (data->hasBeenVisited(current)) {
			continue;
		}

		data->setVisited(current);

		// if this is the end intersection, we must have taken the fastest path to reach it
		if (current == endIntersectionID) {
			break;
		}

		// expand this node's neighbors
		unsigned* neighborSegments = intersections[current]->getStreetSegs();
		unsigned numNeighbors = intersections[current]->getNumStreetSegs();
		std::vector<unsigned> *neighborIntersections = intersections[current]->getConnectedIntersections();
		for (unsigned i=0; i<numNeighbors; i++) {
			// check if this intersection has been expanded before. If so, skip it
			if (!data->hasBeenVisited((*neighborIntersections)[i])) {
				// make sure we're not going the wrong way along this street segment
				if (streetSegments[neighborSegments[i]].getInfo().oneWay && streetSegments[neighborSegments[i]].getInfo().from != current) {
					continue;
				}

				// calculate current-best distance to this neighbor
				double timeToNextNode = data->getTimeToReach(current) + streetSegments[neighborSegments[i]].getTravelTime();
				// if there's a turn, add that time. There can never be a turn from the first intersection
				if (current != startIntersectionID) {
					if (streetSegments[data->getReturnStreetSeg(current)].getInfo().streetID !=
							streetSegments[neighborSegments[i]].getInfo().streetID) {
						timeToNextNode += 0.25; // 15 seconds
					}
				}

				// update path to this node, if it's better than any existing way there
				if (timeToNextNode < data->getTimeToReach((*neighborIntersections)[i])) {
					// update path back
					data->setReturnPath((*neighborIntersections)[i], current, neighborSegments[i], timeToNextNode);
					// insert new entry into priority queue, priority is set by heuristic time to destination
					data->pushOpen((*neighborIntersections)[i], timeToNextNode);
				}
			}
		}
	}

	// reset all the intersections' heuristics
	for (unsigned i=0; i<numTotalIntersections; i++) {
		intersections[i]->unsetHeuristic();
	}

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	if (data->getTimeToReach(endIntersectionID) == PRACTICALLY_INFINITE) {
		// no path was found
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = endIntersectionID;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(data->getReturnStreetSeg(cur));

			cur = data->getReturnIntersection(cur);

			i++;
		}

		// reverse list so it's in correct order
		toReturn.resize(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		return toReturn;
	}
}

/**
 * When a path between two intersections has been found, this function can be used to return it as a vector of street segment IDs
 * @param startIntersectionID
 * @param endIntersectionID
 * @return
 */
std::vector<unsigned> dijkstraReturnPathBetweenIntersections(unsigned startIntersectionID, unsigned endIntersectionID) {
	DijkstraData* data = DijkstraData::instance();

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	if (data->getTimeToReach(endIntersectionID) == FLT_MAX) {
		// no path was found
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = endIntersectionID;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(data->getReturnStreetSeg(cur));

			cur = data->getReturnIntersection(cur);

			i++;
		}

		// reverse list so it's in correct order
		toReturn.reserve(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		return toReturn;
	}
}

/**
 * When a path between two intersections has been found, this function can be used to return the time taken to reach the end intersection
 * @param endIntersectionID The intersectionID of an intersection that has been visited by findShortestPathDijkstra
 * @return The time (in minutes) taken to reach the intersection
 */
double dijkstaReturnTimeToIntersection(unsigned endIntersectionID) {
	return DijkstraData::instance()->getTimeToReach(endIntersectionID);
}

/**
 * Finds shortest path between two intersections using A* algorithm (best for when we only care about the path to the one destination)
 * @param startIntersectionID
 * @param endIntersectionID
 * @param bestTime RETURN PARAMETER: This will contain the shortest time to get to the destination
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathAstar(unsigned startIntersectionID, unsigned endIntersectionID, double& bestTime) {
	// priority queue for nodes that have not yet been visited
	std::priority_queue<std::pair<unsigned, double>, std::vector<std::pair<unsigned, double>>, CompareHeuristic> open;
	// add the starting intersection to the queue
	open.push(std::make_pair(startIntersectionID, 0.0));

	// set of nodes that have been expanded, so that we don't visit them again
	unsigned numTotalIntersections = MapData::instance()->getNumIntersections();
	bool* visited = new bool[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		visited[i] = false;
	}

	// contains time to reach from start and first step of fastest path back for each intersection that has been visited
	double *timeToIntersection = new double[numTotalIntersections];
	unsigned *prevIntersection = new unsigned[numTotalIntersections];
	unsigned *prevStreetSeg = new unsigned[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		timeToIntersection[i] = FLT_MAX;
		prevIntersection[i] = INT_MAX;
		prevStreetSeg[i] = INT_MAX;
	}
	timeToIntersection[startIntersectionID] = 0.0;

	IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	// loop until there are no more undiscovered nodes
	while (!open.empty()) {
		// pop the intersection with the lowest time to reach + heuristic time to destination
		std::pair<unsigned, double> current = open.top();
		open.pop();

		// since we update the priority of given intersections by pushing the new, faster one, we have to check if the
		// one we just popped has already been processed and this was just the version with the lower priority
		if (visited[current.first]) {
			continue;
		}

		visited[current.first] = true;

		// if this is the end intersection, we must have taken the fastest path to reach it
		if (current.first == endIntersectionID) {
			break;
		}

		// expand this node's neighbors
		unsigned* neighborSegments = intersections[current.first]->getStreetSegs();
		unsigned numNeighbors = intersections[current.first]->getNumStreetSegs();
		std::vector<unsigned> *neighborIntersections = intersections[current.first]->getConnectedIntersections();
		for (unsigned i=0; i<numNeighbors; i++) {
			// check if this intersection has been expanded before. If so, skip it
			if (!visited[(*neighborIntersections)[i]]) {
				// make sure we're not going the wrong way along this street segment
				if (streetSegments[neighborSegments[i]].getInfo().oneWay && streetSegments[neighborSegments[i]].getInfo().from != current.first) {
					continue;
				}

				// calculate current-best distance to this neighbor
				double timeToNextNode = timeToIntersection[current.first] + streetSegments[neighborSegments[i]].getTravelTime();
				// if there's a turn, add that time. There can never be a turn from the first intersection
				if (current.first != startIntersectionID) {
					if (streetSegments[prevStreetSeg[current.first]].getInfo().streetID !=
							streetSegments[neighborSegments[i]].getInfo().streetID) {
						timeToNextNode += 0.25; // 15 seconds
					}
				}

				// update path to this node, if it's better than any existing way there
				if (timeToNextNode < timeToIntersection[(*neighborIntersections)[i]]) {
					// update path back
					timeToIntersection[(*neighborIntersections)[i]] = timeToNextNode;
					prevStreetSeg[(*neighborIntersections)[i]] = neighborSegments[i];
					prevIntersection[(*neighborIntersections)[i]] = current.first;
					// insert new entry into priority queue, priority is set by heuristic time to destination
					double f = timeToNextNode + _timeBetweenTwoIntersections(intersections[(*neighborIntersections)[i]], intersections[endIntersectionID]);
					//double f = timeToNextNode + intersections[(*neighborIntersections)[i]]->getHeuristic(intersections[endIntersectionID]);
					open.push(std::make_pair((*neighborIntersections)[i], f));
				}
			}
		}
	}

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	// this is a return parameter; gets read back by the caller
	bestTime = timeToIntersection[endIntersectionID];

	if (bestTime == FLT_MAX) {
		// no path was found
		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = endIntersectionID;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(prevStreetSeg[cur]);

			cur = prevIntersection[cur];

			i++;
		}

		// reverse list so it's in correct order
		toReturn.resize(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
}

/**
 * Finds shortest path between two intersections using A* algorithm (best for when we only care about the time to the one destination)
 * This version was an attempt at optimizing by storing the heuristic associated with each intersection so we don't have to recalculate
 * it each time, but ended up being slower. Retained for posterity.
 * @param startIntersectionID
 * @param endIntersectionID
 * @return Vector of street segments, in order, to get to destination
 */
std::vector<unsigned> findShortestPathAstar_storedHeuristic(unsigned startIntersectionID, unsigned endIntersectionID) {
	// priority queue for nodes that have not yet been visited
	std::priority_queue<std::pair<unsigned, double>, std::vector<std::pair<unsigned, double>>, CompareHeuristic> open;
	// add the starting intersection to the queue
	open.push(std::make_pair(startIntersectionID, 0.0));

	// set of nodes that have been expanded, so that we don't visit them again
	unsigned numTotalIntersections = MapData::instance()->getNumIntersections();
	bool* visited = new bool[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		visited[i] = false;
	}

	// contains time to reach from start and first step of fastest path back for each intersection that has been visited
	double *timeToIntersection = new double[numTotalIntersections];
	unsigned *prevIntersection = new unsigned[numTotalIntersections];
	unsigned *prevStreetSeg = new unsigned[numTotalIntersections];
	for (unsigned i=0; i<numTotalIntersections; i++) {
		timeToIntersection[i] = FLT_MAX;
		prevIntersection[i] = INT_MAX;
		prevStreetSeg[i] = INT_MAX;
	}
	timeToIntersection[startIntersectionID] = 0.0;

	IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
	StreetSegment* streetSegments = MapData::instance()->getStreetSegments();

	// loop until there are no more undiscovered nodes
	while (!open.empty()) {
		// pop the intersection with the lowest time to reach + heuristic time to destination
		std::pair<unsigned, double> current = open.top();
		open.pop();

		// since we update the priority of given intersections by pushing the new, faster one, we have to check if the
		// one we just popped has already been processed and this was just the version with the lower priority
		if (visited[current.first]) {
			continue;
		}

		visited[current.first] = true;

		// if this is the end intersection, we must have taken the fastest path to reach it
		if (current.first == endIntersectionID) {
			break;
		}

		// expand this node's neighbors
		unsigned* neighborSegments = intersections[current.first]->getStreetSegs();
		unsigned numNeighbors = intersections[current.first]->getNumStreetSegs();
		std::vector<unsigned> *neighborIntersections = intersections[current.first]->getConnectedIntersections();
		for (unsigned i=0; i<numNeighbors; i++) {
			// check if this intersection has been expanded before. If so, skip it
			if (!visited[(*neighborIntersections)[i]]) {
				// make sure we're not going the wrong way along this street segment
				if (streetSegments[neighborSegments[i]].getInfo().oneWay && streetSegments[neighborSegments[i]].getInfo().from != current.first) {
					continue;
				}

				// calculate current-best distance to this neighbor
				double timeToNextNode = timeToIntersection[current.first] + streetSegments[neighborSegments[i]].getTravelTime();
				// if there's a turn, add that time. There can never be a turn from the first intersection
				if (current.first != startIntersectionID) {
					if (streetSegments[prevStreetSeg[current.first]].getInfo().streetID !=
							streetSegments[neighborSegments[i]].getInfo().streetID) {
						timeToNextNode += 0.25; // 15 seconds
					}
				}

				// update path to this node, if it's better than any existing way there
				if (timeToNextNode < timeToIntersection[(*neighborIntersections)[i]]) {
					// update path back
					timeToIntersection[(*neighborIntersections)[i]] = timeToNextNode;
					prevStreetSeg[(*neighborIntersections)[i]] = neighborSegments[i];
					prevIntersection[(*neighborIntersections)[i]] = current.first;
					// insert new entry into priority queue, priority is set by heuristic time to destination
					double f = timeToNextNode + intersections[(*neighborIntersections)[i]]->getHeuristic(intersections[endIntersectionID]);
					open.push(std::make_pair((*neighborIntersections)[i], f));
				}
			}
		}
	}

	// reset all the intersections' heuristics
	for (unsigned i=0; i<numTotalIntersections; i++) {
		intersections[i]->unsetHeuristic();
	}

	// return path as a vector of street segments in order, starting from start. If no path exists, return empty vector.
	std::vector<unsigned> toReturn;

	if (timeToIntersection[endIntersectionID] == FLT_MAX) {
		// no path was found
		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
	else {
		// first get backwards path
		std::vector<unsigned> reverse;

		unsigned cur = endIntersectionID;
		unsigned i = 0;

		while (cur != startIntersectionID) {
			// add street segment that arrived at the current intersection to the vector, move on to the next one
			reverse.push_back(prevStreetSeg[cur]);

			cur = prevIntersection[cur];

			i++;
		}

		// reverse list so it's in correct order
		toReturn.resize(i); // size i
		for (unsigned j=0; j<i; j++) {
			toReturn[j] = reverse[i - j - 1];
		}

		delete[] visited;
		delete[] timeToIntersection;
		delete[] prevIntersection;
		delete[] prevStreetSeg;
		return toReturn;
	}
}

/**
 * Calculates the time taken to travel from one intersection to another, as the crow flies, at the maximum speed limit
 * @param cur Start intersection
 * @param dest End intersection
 * @return Time in minutes
 */
double _timeBetweenTwoIntersections(IntersectionXY* cur, IntersectionXY* dest) {
	// in km
	double dx = (dest->getCoordinates()[0].x - cur->getCoordinates()[0].x) / 1000;
	double dy = (dest->getCoordinates()[0].y - cur->getCoordinates()[0].y) / 1000;

	// in minutes
	return (sqrt(dx * dx + dy * dy) / SPEED_LIMIT * 60);
}

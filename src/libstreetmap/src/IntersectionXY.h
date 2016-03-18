/*
 * IntersectionXY.h
 *
 *  Created on: 6/03/2016
 *      Author: william
 */

#ifndef INTERSECTIONXY_H
#define INTERSECTIONXY_H

#include "MapElement.h"

class IntersectionXY: public MapElement {
public:
	/**
	 * Constructor for IntersectionXY. No default constructor is provided.
	 * @param _name The name of this intersection
	 * @param _coordinates A size 1 array of coordinates, is an array to match MapElement
	 * @param _drawPriority This intersection should only include street segments of equal or greater DrawPriority to this
	 * @param _numStreetSegs The number of street segments attached to this intersection of equal or greater DrawPriority to this
	 */
	IntersectionXY(std::string _name, PointXY* _coordinates, DrawPriority _drawPriority, unsigned _numStreetSegs)
		: MapElement(_name, _coordinates, 1, _drawPriority) {
		numStreetSegs = _numStreetSegs;
		streetSegs = new unsigned[numStreetSegs];
		connectedIntersections = new std::vector<unsigned>(numStreetSegs, 0);
		heuristicSet = false;
		heuristic = 0.0;
	}

	/**
	 * Destructor
	 */
	virtual ~IntersectionXY() {
		delete[] streetSegs;
		delete connectedIntersections;
	}

	/**
	 * Gets the street segments attached to this intersection
	 * @return Array of ID's of street segments
	 */
	unsigned* getStreetSegs() const {
		return streetSegs;
	}

	/**
	 * Gets the number of street segments attached to this intersection
	 * @return
	 */
	unsigned getNumStreetSegs() const {
		return numStreetSegs;
	}

	/**
	 * Gets the street segments attached to this intersection, excluding those already part of another intersection's
	 * drawingStreetSegs.
	 * @return Array of ID's of street segments
	 */
	std::vector<unsigned> getDrawingStreetSegs() const {
		return drawingStreetSegs;
	}

	/**
	 * Adds a street segment to this intersection
	 * @param streetSegmentID The ID of the street segment
	 * @param i The index to add it
	 * @param newOnMap Whether or not this street segment has already been added to an intersection on the map,
	 *  true means it will be added to drawingStreetSegs, false means it won't (since it already exists elsewhere)
	 */
	void setStreetSeg(unsigned streetSegmentID, unsigned i, bool newOnMap) {
		assert(i < numStreetSegs);
		streetSegs[i] = streetSegmentID;

		// if unique, add it to drawingStreetSegs (avoids double-ups)
		if (newOnMap) {
			drawingStreetSegs.push_back(streetSegmentID);
		}
	}

	/**
	 * Adds a street segment to this intersection, including the ID of the destination intersection for pathfinding
	 * @param streetSegmentID The ID of the street segment
	 * @param i The index to add it
	 * @param destination The ID of the intersection the street segment ends at
	 * @param newOnMap Whether or not this street segment has already been added to an intersection on the map,
	 *  true means it will be added to drawingStreetSegs, false means it won't (since it already exists elsewhere)
	 */
	void setStreetSeg(unsigned streetSegmentID, unsigned i, unsigned destination, bool newOnMap) {
		assert(i < numStreetSegs);
		streetSegs[i] = streetSegmentID;

		// add connected intersection
		(*connectedIntersections)[i] = destination;

		// if unique, add it to drawingStreetSegs (avoids double-ups)
		if (newOnMap) {
			drawingStreetSegs.push_back(streetSegmentID);
		}
	}

	/**
	 * Get IDs of the intersections that can be reached from this intersection in one step
	 * @return A pointer to a vector of intersection IDs
	 */
	std::vector<unsigned> *getConnectedIntersections() const {
		return connectedIntersections;
	}

	/**
	 * For use after a run of A*, this means the old heuristic won't be reused for a different target
	 */
	void unsetHeuristic() {
		heuristicSet = false;
	}

	/**
	 * Finds the heuristic distance to an intersection, for use by A*. If it has already been calculated, return the one we already have.
	 * @param goal The intersection to find the heuristic distance to
	 * @return
	 */
	double getHeuristic(IntersectionXY* goal) {
		if (heuristicSet) {
			return heuristic;
		}
		else {
			// calculate heuristic distance
			// in km
			double dx = (this->coordinates[0].x - goal->coordinates[0].x) / 1000;
			double dy = (this->coordinates[0].y - goal->coordinates[0].y) / 1000;

			// in minutes
			heuristic = (sqrt(dx * dx + dy * dy) / SPEED_LIMIT * 60);
			heuristicSet = true;
			return heuristic;
		}
	}

private:
	// Array of IDs of street segments attached to this intersection
	unsigned* streetSegs;
	// Number of street segments attached to this intersection
	unsigned numStreetSegs;
	// vector of street segments that are unique (not included on any other intersection) to avoid double-ups when drawing
	std::vector<unsigned> drawingStreetSegs;
	// IDs of the intersections that can be reached in one step from this one
	std::vector<unsigned> *connectedIntersections;
	// for A*
	bool heuristicSet;
	double heuristic;
};

#endif /* INTERSECTIONXY_H */

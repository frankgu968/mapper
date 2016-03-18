/*
 * PoiXY.h
 *
 *  Created on: 22/02/2016
 *      Author: william
 */

#ifndef POIXY_H
#define POIXY_H

#include <string>
#include "MapElement.h"
#include "utility.h"

class PoiXY : public MapElement {
public:
	/**
	 * Constructor, requires the basic information to be given. No default constructor is provided.
	 * @param _name Name of the element, "" if unnamed
	 * @param _coordinates A size 1 array of coordinates, is an array to match MapElement
	 */
	PoiXY(std::string _name, PointXY* _coordinates)
		: MapElement(_name, _coordinates, 1, low) // POI only have 1 coordinate and only appear at highest zoom level
	{}

	/**
	 * Default destructor (does nothing)
	 */
	virtual ~PoiXY() {}
};

#endif /* POIXY_H */

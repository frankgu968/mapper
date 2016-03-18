/*
 * FeatureXY.h
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

#ifndef FEATUREXY_H
#define FEATUREXY_H

#include "utility.h"
#include "StreetsDatabaseAPI.h"
#include "MapElement.h"

class FeatureXY : public MapElement {
public:
	/**
	 * Constructor, requires the basic information to be given. No default constructor is provided.
	 * @param _name Name of the element, "" if unnamed
	 * @param _coordinates An array of points defining the shape of this feature
	 * @param _numCoordinates The number of points, i.e. size of the coordinates array.
	 * @param _drawPriority What drawing cutoff this element fits into, used for deciding what to show at different zoom levels.
	 * @param _featureID The OSM id of this feature
	 * @param _featureType The type of this feature (lake, building etc.)
	 * @param _featureHeight The height of this feature in m (currently only used for buildings)
	 */
	FeatureXY(std::string _name, PointXY* _coordinates, unsigned _numCoordinates, DrawPriority _drawPriority,
			unsigned _featureID, FeatureType _featureType, double _featureHeight)
		: MapElement(_name, _coordinates, _numCoordinates, _drawPriority) {
		featureType = _featureType;
		id = _featureID;
		featureHeight = _featureHeight;
	}

	/**
	 * Destructor
	 */
	virtual ~FeatureXY() {}

	/**
	 * Gets the type of this feature (e.g. lake, building etc.)
	 * @return type of the feature
	 */
	FeatureType getType() {
		return featureType;
	}

	/**
	 * Gets the OSMID of this feature
	 * @return OSMID of the feature
	 */
	OSMID getOSMID() {
		return id;
	}

	/**
	 * Gets the height of the feature in m (will be 0 for everything that's not a building)
	 * @return
	 */
	double getHeight() {
		return featureHeight;
	}

private:
	/* Type of the feature (e.g. lake, building etc.) */
	FeatureType featureType;
	/* OSM id of the feature */
	OSMID id;
	/* Height of this feature in m (currently only used for buildings) */
	double featureHeight;
};

#endif /* FEATUREXY_H */

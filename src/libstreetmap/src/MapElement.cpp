/*
 * MapElement.cpp
 *
 *  Created on: 5/03/2016
 *      Author: william
 */

#include "MapElement.h"

/**
 * Constructor, requires the basic information to be given. No default constructor is provided.
 * @param _name Name of the element, "" if unnamed
 * @param _coordinates An array of PointXY coordinates that makes up the element.
 * @param _numCoordinates The number of points, i.e. size of the coordinates array.
 * @param _drawPriority What drawing cutoff this element fits into, used for deciding what to show at different zoom levels.
 */
MapElement::MapElement(std::string _name, PointXY* _coordinates, unsigned _numCoordinates, DrawPriority _drawPriority) {
	name = _name;
	coordinates = _coordinates;
	drawCoordinates = new PointXY[_numCoordinates];
	numCoordinates = _numCoordinates;
	drawPriority = _drawPriority;
}

MapElement::~MapElement() {
	// delete all coordinates
	delete[] coordinates;
	delete[] drawCoordinates;
}

/**
 * Gets name of this element
 * @return string with name of the element, "" if unnamed
 */
std::string MapElement::getName() const {
	return name;
}

/**
 * Gets all coordinates of the element (in m)
 * @return array of PointXY coordinates
 */
PointXY* MapElement::getCoordinates() const {
	return coordinates;
}

/**
 * Gets all drawing coordinates of the element (translated and scaled to be centered on origin
 * and have X from -1000 to 1000)
 * @return array of PointXY coordinates
 */
PointXY* MapElement::getDrawCoordinates() const {
	return drawCoordinates;
}

/**
 * Gets number of coordinates associated with this element.
 * @return The size of the first dimension of the coordinates 2D array.
 */
unsigned MapElement::getNumCoordinates() const {
	return numCoordinates;
}

/**
 * Fills drawCoordinates with the right values based on the existing XY coordinates and the map scale factors
 * @param offset The center point of the map
 * @param scale The amount to multiply the shifted coordinates by to get a -1000 to 1000 range for X values
 */
void MapElement::setDrawCoordinates(PointXY drawingMapOffset, double drawingMapScale) {
	for (unsigned i=0; i<numCoordinates; i++) {
		drawCoordinates[i].x = (coordinates[i].x - drawingMapOffset.x) * drawingMapScale;
		drawCoordinates[i].y = (coordinates[i].y - drawingMapOffset.y) * drawingMapScale;
	}
}

/**
 * Gets drawing priority of this element.
 * @return An enum defining which zoom threshold group this element falls under.
 */
DrawPriority MapElement::getDrawPriority() const {
	return drawPriority;
}

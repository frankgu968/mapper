/*
 * MapElement.h
 *
 *  Created on: 5/03/2016
 *      Author: william
 */

#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <string>
#include <assert.h>

#include "utility.h"

enum DrawPriority { always, high, medium, low };

class MapElement {
public:
	/**
	 * Constructor, requires the basic information to be given. No default constructor is provided.
	 * @param _name Name of the element, "" if unnamed
	 * @param _coordinates An array of PointXY coordinates that makes up the element.
	 * @param _numCoordinates The number of points, i.e. size of the coordinates array.
	 * @param _drawPriority What drawing cutoff this element fits into, used for deciding what to show at different zoom levels.
	 */
	MapElement(std::string _name, PointXY* _coordinates, unsigned _numCoordinates, DrawPriority _drawPriority);

	/**
	 * Destructor, frees all memory
	 */
	virtual ~MapElement();

	/**
	 * Gets name of this element
	 * @return string with name of the element, "" if unnamed
	 */
	std::string getName() const;

	/**
	 * Gets all map coordinates of the element (in m)
	 * @return array of PointXY coordinates
	 */
	PointXY* getCoordinates() const;

	/**
	 * Gets all drawing coordinates of the element (translated and scaled to be centered on origin
	 * and have X from -1000 to 1000)
	 * @return array of PointXY coordinates
	 */
	PointXY* getDrawCoordinates() const;

	/**
	 * Gets number of coordinates associated with this element.
	 * @return The size of the first dimension of the coordinates 2D array.
	 */
	unsigned getNumCoordinates() const;

	/**
	 * Fills drawCoordinates with the right values based on the existing XY coordinates and the map scale factors
	 * @param offset The center point of the map
	 * @param scale The amount to multiply the shifted coordinates by to get a -1000 to 1000 range for X values
	 */
	void setDrawCoordinates(PointXY drawingMapOffset, double drawingMapScale);

	/**
	 * Gets drawing priority of this element.
	 * @return An enum defining which zoom threshold group this element falls under.
	 */
	DrawPriority getDrawPriority() const;
protected:
	// name of the element
	std::string name;
	// array of X,Y locations (size of 1 for a single point)
	// these correspond to map coordinates (in m)
	PointXY *coordinates;
	// these correspond to drawing coordinates (translated and scaled to be centered on origin and
	// have X from -1000 to 1000)
	PointXY *drawCoordinates;
	// number of points (size of coordinate arrays)
	unsigned numCoordinates;
	// the category for how zoomed in the user must be to see this object
	DrawPriority drawPriority;
};

#endif /* MAPELEMENT_H */

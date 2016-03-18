/*
 * StreetSegment.h
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

#ifndef STREETSEGMENT_H
#define STREETSEGMENT_H

#include "StreetsDatabaseAPI.h"
#include "utility.h"

/* Type of street this is part of (for drawing purposes) */
enum StreetSegmentType { motorway, primary, secondary, tertiary, unclassified };

// contains the four corners of a rectangle defining the street segment between two curve points
class StreetRectangle {
public:
	/**
	 * Constructor, takes as arguments the start and end curve points and initializes the four corners
	 * of the rectangle
	 * @param start
	 * @param end
	 * @param thickness Distance from middle of street to edge in draw coordinates, use MapData's getDrawingMapScale
	 * to convert to this from meters
	 */
	StreetRectangle(PointXY start, PointXY end, double thickness) {
		double theta = atan((end.y - start.y) / (end.x - start.x));

		// adjust for angles > 90 or < -90 degrees
		if (end.x < start.x) {
			theta += M_PI;
		}

		a.x = start.x - thickness * sin(theta);
		a.y = start.y + thickness * cos(theta);

		b.x = start.x + thickness * sin(theta);
		b.y = start.y - thickness * cos(theta);

		c.x = end.x + thickness * sin(theta);
		c.y = end.y - thickness * cos(theta);

		d.x = end.x - thickness * sin(theta);
		d.y = end.y + thickness * cos(theta);
	}


	// the corners
	PointXY a, b, c, d;
};

class StreetSegment {
public:
	StreetSegment();
	StreetSegment(const StreetSegment &rhs);
	void operator=(const StreetSegment &rhs);
	virtual ~StreetSegment();

	/**
	 * Initializes values with data from StreetsDatabaseAPI
	 * @param streetSegmentID the ID of this street segment
	 */
	void initialize(unsigned streetSegmentID);

	/**
	 * Sets the length of the street, determines the travel time
	 * @param _length length of the street
	 */
	void setLength(double _length);

	/**
	 * Adds a curve point to the street segment
	 * @param X the x coordinate
	 * @param Y the y coordinate
	 */
	void addCurvePoint(PointXY coord);

	/**
	 * Sets the type of the street (motorway, secondary etc.)
	 * @param _type the type
	 */
	void setStreetType(StreetSegmentType _type);

	/**
	 * Gets the type of the street (motorway, secondary etc.)
	 * @return the type
	 */
	StreetSegmentType getStreetType();

	/**
	 * Gets struct of street segment info, e.g. start/end intersections
	 * @return struct of street segment info, e.g. start/end intersections
	 */
	StreetSegmentInfo getInfo();

	/**
	 * Gets length of this street segment (in m)
	 * @return
	 */
	double getLength();

	/**
	 * Gets curve points of the street segment (as X,Y, in metres)
	 * @return curve points of this street segment as X,Y. For each pair, first is X, second is Y
	 */
	std::vector<PointXY> getCurvePoints();

	/**
	 * Gets curve points of the street segment (as X,Y, in adjusted drawing coordinates)
	 * @return curve points of this street segment as X,Y. For each pair, first is X, second is Y
	 */
	std::vector<PointXY> getDrawCurvePoints();

	/**
	 * Whether this street segment is straight enough to draw a name on
	 * @return true if it is suitable to draw a name
	 */
	bool canDrawName() {
		return suitableForName;
	}

	/**
	 * The time to travel along this street segment, in seconds
	 * @return
	 */
	double getTravelTime() const {
		return travelTime;
	}

	/**
	 * Check if this street segment is straight enough to be suitable draw the street name on, store this value
	 * internally.
	 */
	void determineSuitableForName();

	/**
	 * Get the rectangles that define the street segment for drawing
	 * @return Array of pointers to StreetRectangles, each of which contain 4 PointXY's that define the rectangle
	 */
	StreetRectangle** getRectangles() const;

	/**
	 * Get the number of rectangles
	 */
	unsigned getNumRectangles() const {
		return curvePoints.size() - 1;
	}

	/**
	 * Fills drawCurvePoints with the right values based on the existing XY coordinates and the map scale factors.
	 * Generates the rectangles for drawing
	 * @param offset The center point of the map
	 * @param scale The amount to multiply the shifted coordinates by to get a -1000 to 1000 range for X values
	 */
	void setDrawCoordinates(PointXY drawingMapOffset, double drawingMapScale);

private:
	/* struct of street segment info, e.g. start/end intersections */
	StreetSegmentInfo info;
	/* length of this street segment (in m) */
	double length;
	/* curve points of this street segment as X,Y.  */
	// Map coordinates (in m)
	std::vector<PointXY> curvePoints;
	// Draw coordinates (translated and scaled to be centered at (0,0) and X values ranging from -1000 to 1000)
	std::vector<PointXY> drawCurvePoints;
	/* street type (motorway, secondary etc.) */
	StreetSegmentType type;
	/* whether this street segment is straight enough to draw the street name on */
	bool suitableForName;
	/* The time (in minutes) it takes to travel from one end of this street to the other (length / speedlimit) */
	double travelTime;
	/* Array of pointers to the rectangles that define the street segment for drawing. Array is of size curvePoints.size() - 1 */
	StreetRectangle** rectangles;
};

#endif /* STREETSEGMENT_H */

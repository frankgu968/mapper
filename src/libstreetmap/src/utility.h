/*
 * utility.h
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

/*
 * General utility functions
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include "StreetsDatabaseAPI.h"

//use these defines if you need earth radius or conversion from degrees to radians
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define DEG_TO_RAD 0.017453292519943295769236907684886
// TODO: extract this from the map, assuming 100km/h for now
#define SPEED_LIMIT 100.000

/**
 * Simple struct for holding X and Y coordinates
 */
struct PointXY{
	double x, y;

	PointXY(){
		x = y = 0;
	}

	explicit PointXY(double _x, double _y){
		x = _x;
		y = _y;
	}

	friend std::ostream& operator<<(std::ostream& os, const PointXY point) {
	       os << "(" << point.x << "," << point.y <<")";
	       return os;
	}

	PointXY& operator=(PointXY inPoint){
		x = inPoint.x;
		y = inPoint.y;

		return *this;
	}
};

/**
 * Calculates scaling factor to account for X distance varying with latitude
 * Runs in O(n)
 * @param getPosition function pointer, either &getPointOfIntersetPosition or &getIntersectionPosition
 * @param getNumPoints function pointer, either &getNumberOfPointsOfInterest or &getNumberOfIntersections
 * @return double scaling factor to calculate X coordinates
 */
double _calculateXScalingFactor(LatLon (*getPosition)(unsigned), unsigned (*getNumPoints)(void));

/** Converts a given latitude/longitude (in degrees) to X/Y coordinates
 * When converting a set of points, ensure xScaleFactor is calculated for and used for all points in the set
 * (using _calculateXScalingFactor)
 *
 * @param point The LatLon object to convert
 * @param xScaleFactor scaling to account for X distance chancing with latitudes - defaults to map wide scaling factor
 * @return X/Y point
 */
PointXY convertLatLonToXY (LatLon point, double xScaleFactor );

/**
 * Function to find index of vector element with kth smallest value
 * @param array Vector to find index of element from
 * @param k Find kth smallest value
 * @param startIndex beginning index of array to search within
 * @param endIndex end index of array to search within
 */
unsigned findkth(const std::vector<double> &array, unsigned k);

/**
 * Removes all duplicate points in an array
 * Does not shrink the original array, only modifies values
 * @param data Array of points
 * @param numPts Number of points in array
 * @return hew number of points, after removal of duplicates
 */
int removeDuplicatesInArray(int* data, int numPts);

/**
 * Checks if a data point already exists in a dataset
 * @param testPt the value to check
 * @param data Array of data
 * @param numPts Number of points to check
 * @return true if point is duplicate
 */
bool pointExistsInArray(int testPt, int* data, int numPts);

/** Calculates distance between two LatLot points
 * Uses formula M1 hand out
 *
 * @param point1 		The first point
 * @param point2 		The second point
 * @return double 		Distance between both points
 */
double find_distance_between_two_points(LatLon point1, LatLon point2);

/**
 * Capitalizes all separate words in a string
 * @param input the string to process
 * @return capitalized string
 */
std::string capitalizeAllWords(std::string input);

/**
 * Calculates angle between two vectors
 * @param v1
 * @param v2
 * @return angle principal arctangent in degrees [180, 180]
 */
double getAngleTwoVectors(std::pair<double,double> v1, std::pair<double, double> v2);
#endif /* UTILITY_H */

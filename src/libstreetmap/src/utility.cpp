/*
 * utility.cpp
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

#include "utility.h"

/**
 * Calculates scaling factor to account for X distance varying with latitude
 * Runs in O(n)
 * @param getPosition function pointer, either &getPointOfIntersetPosition or &getIntersectionPosition
 * @param getNumPoints function pointer, either &getNumberOfPointsOfInterest or &getNumberOfIntersections
 * @return double scaling factor to calculate X coordinates
 */
double _calculateXScalingFactor(LatLon (*getPosition)(unsigned), unsigned (*getNumPoints)(void)){
	double scalingFact, minLat, maxLat, latAvgDeg;

	//Seed an initial reference latitude
	LatLon curPoint = getPosition(0);
	int numPoints = getNumPoints();

	minLat = curPoint.lat;
	maxLat = curPoint.lat;

	//Iterate through all intersections
	for (int i = 1; i < numPoints; i++) {
		curPoint = getPosition(i);
		if (curPoint.lat < minLat)
			minLat = curPoint.lat;

		if (curPoint.lat > maxLat)
			maxLat = curPoint.lat;
	}

	latAvgDeg = (minLat + maxLat) / 2;

	scalingFact = cos(DEG_TO_RAD*latAvgDeg);
	return scalingFact;
}


/** Converts a given latitude/longitude (in degrees) to X/Y coordinates
 * When converting a set of points, ensure avgOfLatExtrema is calculated for and used for all points in the set
 *
 * @param point The LatLon object to convert
 * @param sScaleFactor Average of the largest and smallest latitudes in the set of points being converted
 * @return  X/Y point
 */
PointXY convertLatLonToXY (LatLon point, double xScaleFactor){
	PointXY coord;


	//Must convert to radians
	coord.x = EARTH_RADIUS_IN_METERS*DEG_TO_RAD * xScaleFactor * point.lon;
	coord.y = EARTH_RADIUS_IN_METERS*DEG_TO_RAD * point.lat;

	return coord;
}


/**
 * Function to find index of vector element with kth smallest value
 * @param array Vector to find index of element from
 * @param k Find kth smallest
 * @param startIndex beginning index of array to search within
 * @param endIndex end index of array to search within
 */
unsigned findkth(const std::vector<double> &array, unsigned k) {
	std::vector<double> tmp = array;
	std::sort(tmp.begin(), tmp.end());
	// return index of an element in array that has the same value as the kth smallest element
	for (unsigned i=0; i<array.size(); i++) {
		if (array[i] == tmp[k]) {
			return i;
		}
	}
	return -1;
}


/**
 * Removes all duplicate points in an array
 * Does not shrink the original array, only modifies values
 * @param data Array of points
 * @param numPts Number of points in array
 * @return hew number of points, after removal of duplicates
 */
int removeDuplicatesInArray(int* data, int numPts){
	int numUniques = 0;

	//Iterate through all points
	for (int i = 0; i < numPts; i++){
		//Check if point is unique
		if(!pointExistsInArray(data[i], data, numUniques)){
			//Point is unique - store it
			data[numUniques] = data[i];
			numUniques++;
		}
	}

	return numUniques;
}

/**
 * Checks if a data point already exists in a dataset
 * @param testPt the value to check
 * @param data Array of data
 * @param numPts Number of points to check
 * @return true if point is duplicate
 */
bool pointExistsInArray(int testPt, int* data, int numPts){
	//Iterates through all points
	for(int i = 0; i< numPts; i++){
		if (data[i] == testPt)
			return true;
	}
	return false;
}

/**
 *  Calculates distance between two LatLon points
 * Uses formula M1 hand out
 *
 * @param point1 		The first point
 * @param point2 		The second point
 * @return double 		Distance between both points
 */
double find_distance_between_two_points(LatLon point1, LatLon point2){
	double latAvgDeg, xScaleFactor, dX, dY, distance;

	//Calculate scaling factor (accounts for X distance varying with latitude)
	latAvgDeg = (point1.lat + point2.lat)/2;
	xScaleFactor = cos(DEG_TO_RAD*latAvgDeg);

	//Convert LatLons to XY points
	PointXY p1 = convertLatLonToXY(point1, xScaleFactor);
	PointXY p2 = convertLatLonToXY(point2, xScaleFactor);

	//Calculate delta X and Y between points
	dX = p1.x - p2.x;
	dY = p1.y - p2.y;

	//Use Pythagoras' Theorem to find magnitude of distance
	//Dont' multiply by radius of Earth, as LatLonToXY already does so
	distance = sqrt(dX*dX + dY*dY);

	return distance;
}

/**
 * Capitalizes all separate words in a string
 * @param input the string to process
 * @return capitalized string
 */
std::string capitalizeAllWords(std::string input){
	bool lastCharWasSpace = true;
	std::string newString;

	//Iterate through entire string
	for (unsigned i = 0; i<input.size(); i++){
		char curChar = input[i];

		//Char is a lower case letter preceded by a space
		if(lastCharWasSpace && curChar >= 'a' && curChar <= 'z'){
			//Add capitalized char to string
			char upperCase = curChar + 'A' - 'a';
			newString += upperCase;
		}

		//Add char as is
		else
			newString += curChar;

		//Check if char was a space
		lastCharWasSpace = (curChar == ' ');
	}

	return newString;
}

/**
 * Calculates angle between two vectors
 * Positive angle means v2 is clockwise ("right") compared to v1
 * @param v1
 * @param v2
 * @return angle principal arctangent in degrees [180, 180]
 */
double getAngleTwoVectors(std::pair<double,double> v1, std::pair<double, double> v2){
	double x1 = v1.first;
	double y1 = v1.second;
	double x2 = v2.first;
	double y2 = v2.second;

	double dot = x1*x2 + y1*y2;      // dot product
	double det = x1*y2 - y1*x2;      // determinant
	double angle = atan2(det, dot);   // atan2(y, x);

	return -1 * angle * 180 / M_PI;
}


/*
 * ANNInterface.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: eddd
 */

#include "ANNInterface.h"


/**
 * Destructor
 */
ANNInterface::~ANNInterface() {
	if (nnIdx != NULL){
		delete[] nnIdx;
		nnIdx = NULL;
	}

	if(radiusIdx != NULL){
		delete[] radiusIdx;
		radiusIdx = NULL;
	}

	if(dists != NULL){
		delete[] dists;
		dists = NULL;
	}

	if (queryPt != NULL){
		delete[] queryPt;
		queryPt = NULL;
	}

	if (kdTree != NULL) {
		delete kdTree;
		kdTree = NULL;
	}


	//This deletion is correct - dataPts is not a typical 2D array
	if(dataPts != NULL) {
		delete[] dataPts[0];
		delete[] dataPts;
		dataPts = NULL;
	}
	annClose();
}

/**
 * Constructor - loads a data set into ANN for Nearest Neighbour searching
 * @param _nPts Number of points in data set
 * @param getLatLonFromIndex Function pointer - gets LatLon coordinates for all points in set
 */
ANNInterface::ANNInterface(int _nPts, PointXY (*getPointFromIndex)(unsigned)) {
	//Parameters describing tree
	nPts = _nPts;

	//Create a kdTree if input is valid
	//Parse all data
	dataPts = parseAllPoints(getPointFromIndex, nPts, scalingFact);
	if (nPts > 0 && dataPts != NULL) {
		//Create tree
		kdTree = new ANNkd_tree(dataPts, nPts,
		DIMENSION_OF_MAP);

		//Allocate space to hold results (preallocated to max size for speed over memory efficiency)
		queryPt = annAllocPt(DIMENSION_OF_MAP);					// allocate query point
		nnIdx = new ANNidx[NUMBER_NEIGHBOURS_TO_FIND];// allocate near neigh indices
		radiusIdx = new ANNidx[_nPts];// allocate radius result indices

		dists = new ANNdist[_nPts];	// allocate near neighbor dists
	}


	//Invalid dataset
	else {
		std::cout << "ERROR: Invalid ANN Dataset" << std::endl;
		nPts = 0;
		queryPt = NULL;
		kdTree = NULL;
		nnIdx = NULL;
		radiusIdx = NULL;
		dists = NULL;
	}

}

/**
 * Searches for the nearest point to a location
 * @param location query location
 * @return index of closest neighbour (-1 if not valid)
 */
int ANNInterface::findClosestPoint(PointXY location) {
	//Dataset not valid
	if(nPts == 0)
		return -1;

	//Load point
	queryPt[0] = location.x;
	queryPt[1] = location.y;

	//Searches for nearest point to chosen location
	kdTree->annkSearch(						// search
			queryPt,						// query point
			NUMBER_NEIGHBOURS_TO_FIND,				// number of near neighbors
			nnIdx,							// nearest neighbors (returned)
			dists,							// distance (returned)
			ERROR_BOUND);					// error bound

	return nnIdx[0];
}

/**
 * Returns all points within a radius of central point
 * @param centrePoint Origin, around which to search
 * @param radiusSquared Square of search radius
 * @param numPts RETURN BY PARAM - number of points found
 * @return int* - an array, containing indices of found points
 */
int* ANNInterface::findPointsWithinRadius(PointXY centrePoint, ANNdist radiusSquared, int &numPts){
	//Invalid dataset
	if (nPts == 0){
		numPts = 0;
		return NULL;
	}

	//Load point
	queryPt[0] = centrePoint.x;
	queryPt[1] = centrePoint.y;

	numPts = kdTree->annkFRSearch(
			queryPt,		//Point around which to search
			radiusSquared,	//Square of the radius
			nPts,			//Max points to find - want all possible, so specify total number of points
			radiusIdx,			//Array to store index of stored values
			dists,			//Array to store distances of each point
			ERROR_BOUND);	//Acceptable error bound (zero)

	return radiusIdx;
}



/**
 * Creates an array of points, containing the XY coords of a set of LatLon positions
 * Is used for loading data from StreetsDatabaseAPI (using function pointers)
 * Data is intended for creation of an ANNInterface object
 * @param getPosition function pointer, either &getIntersectionPosition or &getPointofInterestPosition
 * @param getNumPoints function pointer, either &getNumberOfIntersections or &getNumberOfPointsOfInterest
 * @param scalingFactor the longitude->X scaling factor used for the entire data set
 * @return ANNpointArray containing all points, scaled into XY coords
 */
ANNpointArray ANNInterface::parseAllPoints(PointXY (*getPosition)(unsigned),
		unsigned numPoints, double scalingFactor) {

	//Allocate space to hold all data points
	ANNpointArray dataPts = annAllocPts(numPoints, DIMENSION_OF_MAP);

	//Iterate through each intersection; add to storage
	for (unsigned i = 0; i < numPoints; i++) {
		//Fetch point
		PointXY curPoint = getPosition(i);

		//Load XY data into array
		dataPts[i][0] = curPoint.x;
		dataPts[i][1] = curPoint.y;
	}

	return dataPts;
}




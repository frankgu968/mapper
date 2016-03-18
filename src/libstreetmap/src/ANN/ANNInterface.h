/*
 * ANNData.h
 *
 *  Created on: Jan 30, 2016
 *      Author: eddd
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_ANN_ANNINTERFACE_H_
#define MAPPER_LIBSTREETMAP_SRC_ANN_ANNINTERFACE_H_
#include "ANN.h"
#include "LatLon.h"
#include "utility.h"

#define DIMENSION_OF_MAP 2 //2D map
#define ERROR_BOUND 0 //want exact answers
#define NUMBER_NEIGHBOURS_TO_FIND 1 //only want the single nearest one

/**
 * Encapsulates the most basic functionality of ANN (Approximate Nearest Neighbour library)
 * Defines parameters and provides interface for ANN
 * Functionality limited to loading data set during construction, and searching with "findClosestPoint"
 * After creation, data is nonpermutable because ANN relies on underlying data remaining constant
 */

class ANNInterface {

public:
	/**
	 * Constructor - loads a data set into ANN for Nearest Neighbour searching
	 * @param _nPts Number of points in data set
	 * @param getPositionFromIndex Function pointer to getter for all points in the dataset
	 */
	ANNInterface(int _nPts, PointXY (*getPointFromIndex)(unsigned));


	/**
	 * Searches for the nearest point to a location
	 * @param location query location
	 * @return unsigned index of the closest point wihtin  the data set
	*/
	int findClosestPoint(PointXY location);

	/**
	 * Returns all points within a radius of central point
	 * @param centrePoint Origin, around which to search
	 * @param radiusSquared Square of search radius
	 * @param numPts RETURN BY PARAM - number of points found
	 * @return int* - an array, containing indices of found points
	 */
	int* findPointsWithinRadius(PointXY centrePoint, ANNdist radiusSquared, int &numPts);


	//***************************************Getters**************************************
	ANNkd_tree* getKdTree()  { return kdTree; }

	ANNpointArray getDataPts() const { return dataPts; }

	int getNumPts() const { return nPts; }

	ANNidxArray getResults() const { return nnIdx; }

	double getDistance(unsigned i) const { return dists[i]; }

	~ANNInterface();

private:

	//**********************************************Member Variables********************************

	//Details tree parameters
	int nPts;						// actual number of data points

	// Input data set
	//ANNpointArray = double**
	ANNpointArray dataPts;

	ANNkd_tree* kdTree;				// pointer to the actual search structure

	// query point (i.e. point to search from)
	//ANNpoint is typedef = double* by ANN
	ANNpoint queryPt;

	//Scaling factor for LatLon/XY conversions
	double scalingFact;

	//**********************************************Returned values***************************************

	// holds indices of found points
	//ANNidxArray is typedef = int* by ANN
	ANNidxArray nnIdx;
	ANNidxArray radiusIdx;

	//ANNdistArray is typedef to double* by ANN
	ANNdistArray dists;  // near neighbor distances (results from search)


	//***********************************************Helper Functions*************************************

	/**
	 * Creates an array of points
	 * Is used for loading data from StreetsDatabaseAPI (using function pointers)
	 * Data is intended for creation of an ANNInterface object
	 * @param getPosition function pointer, either &getIntersectionPosition or &getPointofInterestPosition
	 * @param getNumPoints function pointer, either &getNumberOfIntersections or &getNumberOfPointsOfInterest
	 * @param scalingFactor the longitude->X scaling factor used for the entire data set (cosine of avg lat)
	 * @return ANNpointArray containing all points, scaled into XY coords
	 */
	ANNpointArray parseAllPoints(PointXY (*getPosition)(unsigned),
			unsigned numPoints, double scalingFactor);

	/**
	 * Private default constructor
	 * Sanity check - guarantee that creation without parameters is impossible
	 */
	ANNInterface() {
		assert(0);
	};

};


//Used to differentiate between all types of data stored using ANN
enum DataType {
	SmallIntersections,
	MediumSmallIntersections,
	MediumLargeIntersections,
	LargeIntersections,
	POI,
	SmallFeatures,
	MediumSmallFeatures,
	MediumLargeFeatures,
	LargeFeatures,
	Events,
	Invalid_Type
};

#endif /* MAPPER_LIBSTREETMAP_SRC_ANN_ANNINTERFACE_H_ */

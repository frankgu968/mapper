/* 
 * File:   MapData.cpp
 * Author: William Kingsford
 * 
 * Created on January 21, 2016, 12:07 PM
 */
#include "MapData.h"

// initialize mapData to NULL
MapData* MapData::mapData = NULL;

/**
* Gets the MapData instance
* @return the MapData instance (by reference)
*/
MapData* MapData::instance() {
    // If mapData does not already exist, call constructor.
	if (!mapData) {
		mapData = new MapData();
	}

    return MapData::mapData;
}



/**
* Gets the array of Street objects, where the array index is the streetID
* @return pointer to first object in street array
*/
Street* MapData::getStreets() {
    return streets;
}

/**
 * Gets the array of StreetSegment objects, where the array index is the streetSegmentID
 * @return pointer to first object in streetsegment array
 */
StreetSegment* MapData::getStreetSegments() {
	return streetSegments;
}

/**
 * Gets the array of point of interest objects, where the array index is the point of interest ID
 * @return Array of pointers to PoiXY objects
 */
PoiXY** MapData::getPointsOfInterest() {
	return pointsOfInterest;
}

/**
 * Gets the vector of Event objects
 * @return pointer to vector of Event objects
 */
std::vector<Event>* MapData::getEvents() {
	return events;
}

/**
* Gets the map of street names => streetID's
* @return the map of street names => streetID's
*/
nameIDMap* MapData::getStreetNameMap() {
    return streetNameMap;
}

/**
 * Gives the index of the large feature the large feature index is part of
 * @param largeFeaturePtIdx The index of the large feature point
 * @return The index of the large feature
 */
unsigned MapData::convertLargeFeaturePtIndexToIndex(unsigned largeFeaturePtIdx) {
	return largeFeaturePointToIndex[largeFeaturePtIdx];
}

/**
 * Add a large feature point for ANN purposes, keep track of what large feature this is part of.
 * @param point The XY location of the feature point
 * @param featureIdx The index of the feature this point is a part of
 */
void MapData::addLargeFeaturePt(PointXY point, unsigned featureIdx) {
	// add feature point
	largeFeaturePoints.push_back(point);
	// keep track of what feature this point is a part of
	largeFeaturePointToIndex.push_back(featureIdx);
}

/**
 * Determines whether a point is inside a given feature (used to still draw a large feature when screen is zoomed
 * far enough in on the center of the feature that none of its edges are within drawing radius)
 * @param featureIdx The index of the feature
 * @param X The x location to check
 * @param Y The y location to check
 * @param drawPriority Used to determine which array of feature the index refers to
 * @return True if the point is inside the feature, false otherwise
 */
bool MapData::checkPointInsideFeature(unsigned featureIdx, double X, double Y, DrawPriority drawPriority) {
	// method is to check how many times a line from -infinity to the XY point, traveling parallel to the X axis,
	// intersects the feature polygon's edges. If it is an odd number of times, the point is inside the polygon.

	PointXY* coordinates;
	unsigned numCoordinates;

	// get coordinates from index, depending on what size feature it is
	if (drawPriority == always) {
		coordinates = largeFeatures[featureIdx]->getCoordinates();
		numCoordinates = largeFeatures[featureIdx]->getNumCoordinates();
	}
	else if (drawPriority == high) {
		coordinates = mediumLargeFeatures[featureIdx]->getCoordinates();
		numCoordinates = mediumLargeFeatures[featureIdx]->getNumCoordinates();
	}
	else if (drawPriority == medium) {
		coordinates = mediumSmallFeatures[featureIdx]->getCoordinates();
		numCoordinates = mediumSmallFeatures[featureIdx]->getNumCoordinates();
	}
	else {
		coordinates = smallFeatures[featureIdx]->getCoordinates();
		numCoordinates = smallFeatures[featureIdx]->getNumCoordinates();
	}

	// the vector representing the edge from the current point
	double xEdge, yEdge;
	// vertical offset of XY point relative to vertex
	double yDistance;
	// ratio of yDistance to Y component of edge vector
	double yFraction;
	// index of next vertex
	int iEnd;
	// counter for how many sides have been intersected by the horizontal line
	int sidesIntersected = 0;

	for (unsigned istart = 0; istart < numCoordinates; istart+=10) {
		// find vector representing next edge
		iEnd = (istart + 1) % numCoordinates; // next vertex
		xEdge = coordinates[iEnd].x - coordinates[istart].x;
		yEdge = coordinates[iEnd].y - coordinates[istart].y;

		// check for y-intersection: if the ratio of the yDistance between the click and first vertex and the
		// y component of the vector between the two vertices is between 0 and 1, the click was in a band of the
		// y-axis which could cause intersection
		yDistance = (Y - coordinates[istart].y);
		if (yEdge != 0) {
		  yFraction = yDistance / yEdge;
		}
		else if (yDistance == 0) {
		  yFraction = 0;
		}
		else {
		  yFraction = 1e10; // approximately infinite for our purposes
		}

		if (yFraction >= 0 && yFraction < 1) {
		 // we know the click has a y-value which could cause collision, now we need to see if the click was
		 // to the right of the vector
		 float xIntersection = coordinates[istart].x + xEdge * yFraction;
		 if (xIntersection <= X )
			 sidesIntersected++;
		}
	}
	// if there were an odd number of intersections, the point must be inside the polygon
	return (sidesIntersected % 2);
}

/**
 * Returns pointer to desired ANN interface
 * @param dataType The data set required (e.g. Intersections, POI)
 * @return requested pointer to ANNInterface
 */
ANNInterface* MapData::getAnnInterface(DataType dataType){
	//Select data type
	switch (dataType){

	//Small Intersections
	case SmallIntersections:
		return smallIntersectionKd;
		break;

	//Medium-Small Intersections
	case MediumSmallIntersections:
		return mediumSmallIntersectionKd;
		break;

	//Medium-Large Intersections
	case MediumLargeIntersections:
		return mediumLargeIntersectionKd;
		break;

	//Large Intersections
	case LargeIntersections:
		return largeIntersectionKd;
		break;

	//Points of interest
	case POI:
		return poiKd;
		break;

	//Small Features
	case SmallFeatures:
		return smallFeatureKd;
		break;

	//Medium Small Features
	case MediumSmallFeatures:
		return mediumSmallFeatureKd;
		break;

	//Medium large features
	case MediumLargeFeatures:
		return mediumLargeFeatureKd;
		break;


	//Large features
	case LargeFeatures:
		return largeFeatureKd;
		break;

	//Events
	case Events:
		return eventKd;
		break;

	//Unrecognized data type
	default: return NULL;

	}
}

/**
 * Initializes and loads data for all KD trees
 * NOTE: Scaling factor must be calculated before calling this function
 */
void MapData::generateKdTrees(){
	//Exit if the kds are already created
	if(smallIntersectionKd != NULL || mediumSmallIntersectionKd != NULL || mediumLargeIntersectionKd != NULL ||
			smallIntersectionKd != NULL || poiKd != NULL || smallFeatureKd != NULL ||
			mediumSmallFeatureKd != NULL || mediumLargeFeatureKd != NULL ||
			largeFeatureKd != NULL) {
		return;
	}

	//Get number of data points for events kd tree
	//unsigned numEvent = events->size();

	//Initialize intersection trees
	if (numIntersections > 0) {
		smallIntersectionKd = new ANNInterface (numIntersections, &getSmallIntersectionPositionXY);
	}
	if (numMediumSmallIntersections > 0) {
		mediumSmallIntersectionKd = new ANNInterface (numMediumSmallIntersections, &getMediumSmallIntersectionPositionXY);
	}
	if (numMediumLargeIntersections > 0) {
		mediumLargeIntersectionKd = new ANNInterface (numMediumLargeIntersections, &getMediumLargeIntersectionPositionXY);
	}
	if (numLargeIntersections > 0) {
		largeIntersectionKd = new ANNInterface (numLargeIntersections, &getLargeIntersectionPositionXY);
	}

	//Initialize poi tree
	if (numPOI > 0) {
		poiKd = new ANNInterface(numPOI, &getPOIPositionXY);
	}

	//Initialize feature trees
	if (numSmallFeatures > 0) {
		smallFeatureKd = new ANNInterface(numSmallFeatures, &getSmallFeatureLocation);
	}
	if (numMediumSmallFeatures > 0) {
		mediumSmallFeatureKd = new ANNInterface(numMediumSmallFeatures, &getMediumSmallFeatureLocation);
	}
	if (numMediumLargeFeatures > 0) {
		mediumLargeFeatureKd = new ANNInterface(numMediumLargeFeatures, &getMediumLargeFeatureLocation);
	}
	if (numLargeFeatures > 0) {
		largeFeatureKd = new ANNInterface(numLargeFeatures, &getLargeFeatureLocation);
	}

	//Initialize event tree
	//eventKd = new ANNInterface(numEvent, &getEventLocation);
}

/**
 * Get the vector of street segments at a given intersection
 * @param intersectionID The intersection to get the street segments for
 */
std::vector<unsigned> MapData::getIntersectionStreetSegments(unsigned intersectionID) {
	assert(intersectionID < numIntersections);
	// create vector to return
	std::vector<unsigned> retVal;
	unsigned* streetSegs = smallIntersections[intersectionID]->getStreetSegs();
	unsigned numStreetSegs = smallIntersections[intersectionID]->getNumStreetSegs();
	retVal.resize(numStreetSegs);
	for (unsigned i=0; i<numStreetSegs; i++) {
		retVal[i] = streetSegs[i];
	}
	return retVal;
}



/**
 * Deletes all data structures, resets all counts to 0.
 * Can be used before loading a new map, or when it is time to end the program.
 */
void MapData::emptyData() {
	delete mapData;
	mapData = NULL;
}

/**
* Destructor, deletes all data structures and frees all associated memory
*/
MapData::~MapData() {
	delete[] streets;
	streets = NULL;

	// delete all feature objects
	for (unsigned i=0; i<numSmallFeatures; i++) {
		delete smallFeatures[i];
	}
	delete[] smallFeatures;

	for (unsigned i=0; i<numMediumSmallFeatures; i++) {
		delete mediumSmallFeatures[i];
	}
	delete[] mediumSmallFeatures;

	for (unsigned i=0; i<numMediumLargeFeatures; i++) {
		delete mediumLargeFeatures[i];
	}
	delete[] mediumLargeFeatures;

	for (unsigned i=0; i<numLargeFeatures; i++) {
		delete largeFeatures[i];
	}
	delete[] largeFeatures;

	delete[] streetSegments;
	streetSegments = NULL;

	// delete points of interest
	for (unsigned i=0; i<numPOI; i++) {
		delete pointsOfInterest[i];
	}
	delete[] pointsOfInterest;

	pointsOfInterest = NULL;

	delete events;
	events = NULL;

	delete streetNameMap;
	streetNameMap = NULL;

	delete poiNameMap;
	poiNameMap = NULL;

	// delete all intersection objects
	for (unsigned i=0; i<numIntersections; i++) {
		delete smallIntersections[i];
	}
	delete[] smallIntersections;
	smallIntersections = NULL;

	for (unsigned i=0; i<numMediumSmallIntersections; i++) {
		delete mediumSmallIntersections[i];
	}
	delete[] mediumSmallIntersections;
	mediumSmallIntersections = NULL;

	for (unsigned i=0; i<numMediumLargeIntersections; i++) {
		delete mediumLargeIntersections[i];
	}
	delete[] mediumLargeIntersections;
	mediumLargeIntersections = NULL;

	for (unsigned i=0; i<numLargeIntersections; i++) {
		delete largeIntersections[i];
	}
	delete[] largeIntersections;
	largeIntersections = NULL;
	
	if(smallIntersectionKd != NULL){
		delete smallIntersectionKd;
		smallIntersectionKd = NULL;
	}

	if(mediumSmallIntersectionKd != NULL){
		delete mediumSmallIntersectionKd;
		mediumSmallIntersectionKd = NULL;
	}

	if(mediumLargeIntersectionKd != NULL){
		delete mediumLargeIntersectionKd;
		mediumLargeIntersectionKd = NULL;
	}

	if(largeIntersectionKd != NULL){
		delete largeIntersectionKd;
		largeIntersectionKd = NULL;
	}

	if(poiKd != NULL){
		delete poiKd;
		poiKd = NULL;
	}

	if(smallFeatureKd != NULL){
		delete smallFeatureKd;
		smallFeatureKd = NULL;
	}

	if(mediumSmallFeatureKd != NULL){
		delete mediumSmallFeatureKd;
		mediumSmallFeatureKd = NULL;
	}

	if(mediumLargeFeatureKd != NULL){
		delete mediumLargeFeatureKd;
		mediumLargeFeatureKd = NULL;
	}

	if(largeFeatureKd != NULL){
		delete largeFeatureKd;
		largeFeatureKd = NULL;
	}

	if(eventKd != NULL){
		delete eventKd;
		eventKd = NULL;
	}

	delete mapTrie;
	mapTrie = NULL;

	annClose(); //handles small memory leak in ANN
}

/**
 * Private default constructor for singleton, initializes array of street objects
 * and map of street names to street id's
 * Initializes everything to eliminate warnings
*/
MapData::MapData() {
	numStreets = getNumberOfStreets();
	numStreetSegments = getNumberOfStreetSegments();
	numIntersections = getNumberOfIntersections();
	numPOI = getNumberOfPointsOfInterest();
	numFeatures = getNumberOfFeatures();
	streets = new Street[numStreets];

	// TODO: Make these sized based on actual number of each size of feature/intersection. Currently like this for purpose of generating datafile
	numMediumSmallIntersections = numIntersections;
	numMediumLargeIntersections = numIntersections;
	numLargeIntersections = numIntersections;
	smallIntersections = new IntersectionXY*[numIntersections];
	mediumSmallIntersections = new IntersectionXY*[numMediumSmallIntersections];
	mediumLargeIntersections = new IntersectionXY*[numMediumLargeIntersections];
	largeIntersections = new IntersectionXY*[numLargeIntersections];

	numSmallFeatures = 0;
	numMediumSmallFeatures = 0;
	numMediumLargeFeatures = 0;
	numLargeFeatures = 0;
	smallFeatures = new FeatureXY*[numFeatures];
    mediumSmallFeatures = new FeatureXY*[numFeatures];
    mediumLargeFeatures = new FeatureXY*[numFeatures];
    largeFeatures = new FeatureXY*[numFeatures];

	streetNameMap = new nameIDMap();
	poiNameMap = new nameIDMap();
    streetSegments = new StreetSegment[numStreetSegments];

    pointsOfInterest = new PoiXY*[numPOI];

    events = new std::vector<Event>;

	smallIntersectionKd = NULL;
	mediumSmallIntersectionKd = NULL;
	mediumLargeIntersectionKd = NULL;
	largeIntersectionKd = NULL;

	poiKd = NULL;
	smallFeatureKd = NULL;
	mediumSmallFeatureKd = NULL;
	mediumLargeFeatureKd = NULL;
	largeFeatureKd = NULL;
	eventKd = NULL;

	mapTrie = new Trie();

	//Calculate scaling factor (needed for all conversions of LatLon -> XY)
	scalingFactor =_calculateXScalingFactor(&getIntersectionPosition, &getNumberOfIntersections);

}


/**
 * Returns location of small feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Small feature index
 * @return XY location of feature at index
 */
PointXY getSmallFeatureLocation(unsigned i) {
	MapData* map = MapData::instance();
	double x = map->getSmallFeatures()[i]->getCoordinates()[0].x;
	double y = map->getSmallFeatures()[i]->getCoordinates()[0].y;
	PointXY retVal(x,y);
	return retVal;
}

/**
 * Returns location of medium-small feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Medium-small feature index
 * @return XY location of feature at index
 */
PointXY getMediumSmallFeatureLocation(unsigned i) {
	MapData* map = MapData::instance();
	double x = map->getMediumSmallFeatures()[i]->getCoordinates()[0].x;
	double y = map->getMediumSmallFeatures()[i]->getCoordinates()[0].y;
	PointXY retVal(x,y);
	return retVal;
}

/**
 * Returns location of medium-large feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Medium-large feature index
 * @return XY location of feature at index
 */
PointXY getMediumLargeFeatureLocation(unsigned i) {
	MapData* map = MapData::instance();
	double x = map->getMediumLargeFeatures()[i]->getCoordinates()[0].x;
	double y = map->getMediumLargeFeatures()[i]->getCoordinates()[0].y;
	PointXY retVal(x,y);
	return retVal;
}

/**
 * Returns location of large feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Large feature index
 * @return XY location of feature at index
 */
PointXY getLargeFeatureLocation(unsigned i) {
	MapData* map = MapData::instance();
	double x = map->getLargeFeatures()[i]->getCoordinates()[0].x;
	double y = map->getLargeFeatures()[i]->getCoordinates()[0].y;
	PointXY retVal(x,y);
	return retVal;
}

/**
 * Get location of given large feature point (several of these may correspond to one feature)
 * It is not a MapData member so that it can be used as a callback function for ANNInterface constructor
 * @param i Index corresponding to the largeFeaturePoints array
 * @return XY location of large feature point
 */
PointXY getLargeFeaturePointLocation(unsigned i) {
	MapData* map = MapData::instance();
	return map->getLargeFeaturePoints()[i];
}

/**
 * Gets the location of the event with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @return XY location of the event
 */
PointXY getEventLocation(unsigned i) {
	MapData* map = MapData::instance();
	return (*(map->getEvents()))[i].getCoordinates()[0];
}

/**
 * Gets the location of a small intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getSmallIntersectionPositionXY(unsigned i) {
	MapData* map = MapData::instance();
	assert(i < map->getNumIntersections());
	return map->getSmallIntersections()[i]->getCoordinates()[0];
}

/**
 * Gets the location of a medium-small intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getMediumSmallIntersectionPositionXY(unsigned i) {
	MapData* map = MapData::instance();
	assert(i < map->getNumMediumSmallIntersections());
	return map->getMediumSmallIntersections()[i]->getCoordinates()[0];
}

/**
 * Gets the location of a medium-large intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getMediumLargeIntersectionPositionXY(unsigned i) {
	MapData* map = MapData::instance();
	assert(i < map->getNumMediumLargeIntersections());
	return map->getMediumLargeIntersections()[i]->getCoordinates()[0];
}

/**
 * Gets the location of a large intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getLargeIntersectionPositionXY(unsigned i) {
	MapData* map = MapData::instance();
	assert(i < map->getNumLargeIntersections());
	return map->getLargeIntersections()[i]->getCoordinates()[0];
}

/**
 * Gets the location of a POI, used for ANN to build a kd tree
 * @param i The poi ID
 * @return XY location of the poi
 */
PointXY getPOIPositionXY(unsigned i) {
	MapData* map = MapData::instance();
	assert(i<map->getNumPOI());
	return map->getPointsOfInterest()[i]->getCoordinates()[0];
}

#include "mapProcessor.h"
#include "DisplayProcessor.h"

#include "ANN/ANN.h"
#include "ANN/ANNInterface.h"

//load the map
bool load_map(std::string map_name) {
	// initialize map data, check whether it has loaded successfully
	bool load_success = loadStreetsDatabaseBIN(map_name);

	if (!load_success) {
		std::cout << "ERROR: Streets database failed to load!" << std::endl;
		return false;
	}
	else {
		// load OSM data
		std::string osm_name = map_name.erase(map_name.length() - 11) + "osm.bin";
		load_success = loadOSMDatabaseBIN(osm_name);
		if (!load_success) {
			std::cout << "ERROR: OSM database failed to load!" << std::endl;
			return false;
		}

		// MapData object is a singleton, so it is automatically initialized the
		// first time it is used. We store it here for efficiency.
		MapData *map = MapData::instance();

		/* Create street names => street id's hash table */
		_generateStreetNameHashTable(map);


		/* Generate hash table inside each street object with keys being id's of streets
		 * it connects to and values being id's of the intersection formed by
		 * those two streets. */
		_generateStreetIntersectionHashTable(map);


		/* Generate Feature objects, containing their basic data (name, OSMID etc.) and coordinates in LatLon and XY.
		 * Store them in three arrays, based on how large the feature is.
		 */
		_generateFeatureObjects(map);

		/* Compute and store all street segment curve points as X,Y values, calculate length as we do this,
		 * and add list of street segments that form a street to each Street object. */
		_generateStreetSegmentObjects(map);

		/* Generate two arrays inside each intersection object, one containing ids of all street segments
		 * attached to the intersection and the other containing number of street segments connected to the
		 * intersection.
		 */
		_generateIntersections(map);

		// add points of interest
		_generatePOIObjects(map);

		//Load strings of the map for autocomplete
		//Must happen after street name and POI maps are completed
		_loadNames(map);

		// load events
		// TODO: Make events work
		//_loadEvents("toronto.events.xml", "toronto", map);

		// calculate values for conversion from map coordinates -> drawing coordinates (which have an average of (0,0)
		// and a range of X values from -1000 to 1000
		_drawingScaleConversion(map);

		//Load kd trees - this call should happen last, as it relies on all data sets being loaded
		map->generateKdTrees();

		std::cout << "Loaded map: " << map_name << std::endl;

		// return true if mapData successfully loaded
		return true;
	}
}



//close the map
void close_map() {
	closeStreetDatabase();
	// empty data structures, so that it's free for the next map (if any)
	MapData::instance()->emptyData();
}

/**
 * function to return street id(s) for a street name
 * return a 0-length vector if no street with this name exists.
 * @param street_name
 * @return vector of ids of street with the given name
 */
std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
	// IDs of streets that have the given name
	std::vector<unsigned> IDs;
	// unordered_multimap of street names => street ids
	nameIDMap* nameMap = MapData::instance()->getStreetNameMap();
	// iterator for street IDs that match the street name
	auto range = nameMap->equal_range(street_name);

	// push these names onto the return vector
	for (auto iter = range.first; iter != range.second; ++iter) {
		IDs.push_back(iter->second);
	}

	return IDs;
}



/**
 * function to return the street segments for a given intersection
 * @param intersection_id the intersection to find street segments for
 * @return a vector of street segment id's connected to this intersection
 */
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
	MapData* map = MapData::instance();

	return map->getIntersectionStreetSegments(intersection_id);
}


/**
 * function to return street names at an intersection (including duplicate street names in returned vector)
 * @param intersection_id the intersection to find street names for
 * @return a vector of street names connected to this intersection
 */
std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
	// street name vector to return
	std::vector<std::string> streetNames;

	std::vector<unsigned> streetSegmentIDs = find_intersection_street_segments(intersection_id);
	unsigned numSegments = streetSegmentIDs.size();
	for (unsigned i=0; i<numSegments; i++) {
		// get name of street that the street segment is part of, add it to the vector
		streetNames.push_back(getStreetName(getStreetSegmentInfo(streetSegmentIDs[i]).streetID));
	}
	return streetNames;
}


/**
 * can you get from intersection1 to intersection2 using a single street segment (taking into account 1-way streets)
 * corner case: an intersection is considered to be connected to itself
 * @param intersection_id1 start intersection
 * @param intersection_id2 end intersection
 * @return true if you can get from intersection1 to intersection2 in one street segment
 */
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
	// check for intersection ids within the valid range
	MapData *map = MapData::instance();

	// corner case: a street is connected to itself
	if (intersection_id1 == intersection_id2) {
		return true;
	}

	// iterate through street segments connected to intersection 1, if they lead to intersection 2 then return true
	std::vector<unsigned> streetSegments = map->getIntersectionStreetSegments(intersection_id1);
	unsigned numSegments = streetSegments.size();
	StreetSegmentInfo curSegment;
	for (unsigned i=0; i<numSegments; i++) {
		curSegment = getStreetSegmentInfo(streetSegments[i]);

		// this segment must be connected to intersection_id1 on at least one end
		// if it ends at intersection_id2, there is a direct path
		if (curSegment.to == intersection_id2) {
			return true;
		}
		// otherwise if it begins at intersection_id2 and is not a one way street, they are connected
		else if (curSegment.from == intersection_id2 && !curSegment.oneWay) {
			return true;
		}
	}

	// if control reaches this point, no street segment directly connected the two points
	return false;
}


/**
 * find all intersections reachable by traveling down one street segment
 * from given intersection (can't travel the wrong way on a 1-way street)
 * the returned vector does not contain duplicate intersections
 * returns empty vector if the intersection id is invalid
 * @param intersection_id the intersection id to start from
 * @return a vector of intersection id's that can be reached along one street from this intersection
 */
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
	MapData* map = MapData::instance();

	// vector of intersection ids to return
	std::vector<unsigned> adjacentIntersections;
	// for duplicate-checking
	bool exists;

	// loop through all street segments connected to intersection, find the intersection that can be reached
	// via each segment
	std::vector<unsigned> streetSegments = map->getIntersectionStreetSegments(intersection_id);
	StreetSegmentInfo curSegment;
	unsigned numSegments = streetSegments.size();
	for (unsigned i=0; i<numSegments; i++) {
		curSegment = getStreetSegmentInfo(streetSegments[i]);

		// this segment must be connected to intersection_id on at least one end
		// if it begins at intersection_id, there is a direct path regardless of whether street is one-way
		if (curSegment.from == intersection_id) {
			// check if intersection is already in vector, if so don't add it
			exists = false;
			for (unsigned j=0; j<adjacentIntersections.size(); j++) {
				if (adjacentIntersections[j] == curSegment.to) {
					exists = true;
				}
			}
			if (!exists) {
				adjacentIntersections.push_back(curSegment.to);
			}
		}
		// otherwise if it is not a one way street, it is connected to the starting intersection
		else if (curSegment.to == intersection_id && !curSegment.oneWay) {
			// check if intersection is already in vector, if so don't add it
			exists = false;
			for (unsigned j=0; j<adjacentIntersections.size(); j++) {
				if (adjacentIntersections[j] == curSegment.from) {
					exists = true;
				}
			}
			if (!exists) {
				adjacentIntersections.push_back(curSegment.from);
			}
		}
	}

	return adjacentIntersections;
}


/**
 * for a given street, return all the street segments
 * @param street_id the street to find segments for
 * @return a vector of street segment IDs
 */
std::vector<unsigned> find_street_street_segments(unsigned street_id) {
	MapData* map = MapData::instance();
	assert(street_id < map->getNumStreets());
	return (map->getStreets()[street_id]).getStreetSegmentIDs();
}


/**
 * for a given street, find all the intersections
 * @param street_id The street to find intersections for
 * @return a vector of intersection IDs
 */
std::vector<unsigned> find_all_street_intersections(unsigned street_id) {
	// find all street intersections by finding all intersections the street has with itself
	MapData* map = MapData::instance();

	assert(street_id < map->getNumStreets());

	return (map->getStreets()[street_id]).getIntersection(street_id);
}


/**
 * function to return all intersection ids for two intersecting streets
 * this function will typically return one intersection id between two street names
 * but duplicate street names are allowed, so more than 1 intersection id may exist for 2 street names
 * interchanging first and second street names doesn't change the output, but if the first street name has
 * fewer intersections then the function will be faster
 * @param street_name1 first street name
 * @param street_name2 second street name
 * @return vector of intersection IDs
 */
std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2) {
	std::vector<unsigned> streetID1;
	std::vector<unsigned> streetID2;
	std::vector<unsigned> intersectionIDs;
	// array of street objects, used to find intersections
	Street* streets = MapData::instance()->getStreets();
	// vector to hold intersections between two given streets
	std::vector<unsigned> intersections;

	// get street IDs for the first and second name
	streetID1 = find_street_ids_from_name(street_name1);
	streetID2 = find_street_ids_from_name(street_name2);

	// iterate through all combinations of street IDs
	for (unsigned i=0; i<streetID1.size(); i++) {
		for (unsigned j=0; j<streetID2.size(); j++) {
			// append intersection(s) of streets to return vector
			intersections = streets[streetID1[i]].getIntersection(streetID2[j]);
			intersectionIDs.insert(intersectionIDs.end(), intersections.begin(), intersections.end());
		}
	}
	return intersectionIDs;
}


/**Returns length of a street segment from the MapData object
 *
 * @param street_segment_id ID of segment to get length of
 * @return double Length of the street
 */
double find_street_segment_length(unsigned street_segment_id) {
	return MapData::instance()->getStreetSegments()[street_segment_id].getLength();
}


/**
 *  Calculates length of street
 *
 * @param street_id Street to measure
 * @return double Length of street
 */
double find_street_length(unsigned street_id){
	//Retrieve all segments that make up the street
	MapData* map = MapData::instance();
	Street myStreet = map->getStreets()[street_id];
	std::vector <unsigned> containedSegmentIds = myStreet.getStreetSegmentIDs();

	int numSeg = containedSegmentIds.size();
	double streetLength = 0;

	//Iterate through and add lengths of each segment
	for (int i = 0; i<numSeg; i++){
		streetLength += map->getStreetSegments()[containedSegmentIds[i]].getLength();
	}

	return streetLength;
}


/**
 *  Calculates travel time (minutes) of a street segment
 * Returns 0.0 if no segment found
 *
 * @param street_segment_id segment ID to process
 * @return double Time (minutes) to traverse segment
 */
double find_street_segment_travel_time(unsigned street_segment_id){
	StreetSegmentInfo streetInfo = getStreetSegmentInfo(street_segment_id);
	double speedLimit = streetInfo.speedLimit;
	double segDistance = find_street_segment_length(street_segment_id)/1000; //In km

	if (speedLimit == 0 && segDistance == 0)
	{	return 0.0;	}

	double time =  segDistance/speedLimit* 60;

	return time;
}

/**
 * Loads street data - helper function for _loadNames
 * @param map reference to mapdata
 */
void _loadStreetNames(MapData* map){
	Trie* mapTrie = map->getMapTrie();
	nameIDMap* streetMap = map->getStreetNameMap();
	//Iterate through all streets
	for (auto it = streetMap->begin(); it != streetMap->end(); it++){
		std::string curString = it->first;
		unsigned streetId = it->second;

		//Calculate weighting - based on street length
		int importance = (int)find_street_length(streetId);
		//Add the string
		mapTrie->insertWord(curString, importance);
	}
}

/**
 * Loads poi names - helper function for _loadNames
 * @param map reference to mapdata
 */
void _loadPoiNames(MapData* map){
	//Get reference to Trie
	Trie* mapTrie = map->getMapTrie();
	nameIDMap* poiMap = map->getPOINameMap();

	//Set to keep track of which strings have been inserted
	std::unordered_set<std::string> insertedStrings;

	//Load references to all POI objects
	PoiXY** POIs = map->getPointsOfInterest();
	int numPoi = map->getNumPOI();

	//Iterate through all POI
	for(int i = 0; i < numPoi; i++){
		//Get current string name
		PoiXY* curPoi = POIs[i];
		std::string curName = curPoi->getName();
		int numCopies = 1;
		//Check if name has already been seen - try and insert
		auto insertResult = insertedStrings.insert(curName);

		//POI is unique if insertion to set was successful
		if(insertResult.second == true){
			auto range = poiMap->equal_range(curName);

			//Determine priority - count the number of repeats this POI has
			for(auto it = range.first; it!=range.second; it++){
				numCopies++;
			}

			//Insert poi name
			mapTrie->insertWord(curName, numCopies);
		}

	}
}

/**
* Helper function for load_map
* Loads the names of all streets and POI's
* @param map MapData object to add data to
*/
void _loadNames(MapData* map){
	//Load streets
	_loadStreetNames(map);

	//Load POI
	_loadPoiNames(map);
}


/**Overloaded function
 *
 * Finds closest point to a LatLon coordinate
 * Returns -1 if data set is invalid
 *
 * Simply converts LatLon -> XY, then calls XY closest point finder
 * @param dataType Identifier for data type
 * @param my_position LatLon position around which to search
 * @return index of nearest point
 */
int find_closest_point(DataType dataType, LatLon my_position){
	//Retrieve reference to MapData
	MapData* map = MapData::instance();
	double scalingFact = map->getScalingFactor();

	//Convert coord into XY point, with same scale as stored intersection data
	PointXY coord = convertLatLonToXY(my_position, scalingFact);

	//Call XY version of function for actual search
	int closestIndex = find_closest_point(dataType, coord);

	return closestIndex;
}

/**
 * Helper function to query the distance of a point in an ANN tree
 * @param type the type of tree to query
 * @param idx index to query
 * @return distance
 */
double getAnnDistance(DataType type, unsigned idx){
	MapData* map = MapData::instance();
	ANNInterface* interface = map->getAnnInterface(type);

	//Error checking
	if(interface == NULL)
		return -1;

	else return interface->getDistance(idx);
}

/**
 * Returns the closest type and index, given a vector of types and a location to query
 *
 * @param types
 * @param my_position
 * @return pair of closest type & index
 */
std::pair<DataType, int> find_closest_point_multiple_trees(std::vector<DataType> types, LatLon my_position){
	std::pair<DataType, int> closest;
	closest.first = Invalid_Type;
	closest.second = -1;

	//Values to hold current iteration's values
	int tempIdx;
	double curClosestDist, tempDist;

	//Error checking
	if(types.size() <= 0)
		return closest;

	//Seed initial value
	tempIdx = find_closest_point(types[0], my_position);
	curClosestDist = getAnnDistance(types[0], 0);

	//Error checking
	if(curClosestDist == -1)
		return closest;

	//Store initial value to return
	closest.first = types[0];
	closest.second = tempIdx;

	//Query every type
	for(int i = 0; i < types.size(); i++){
		//Query this data type
		tempIdx = find_closest_point(types[i], my_position);
		tempDist = getAnnDistance(types[i], 0);

		//Store data if valid and closer than current closest point
		if(tempDist >=0 && tempDist < curClosestDist){
			curClosestDist = tempDist;
			closest.first = types[i];
			closest.second = tempIdx;
		}

	}

	return closest;
}

/**Overloaded function
 *
 * Finds closest point to an XY point
 * Returns -1 if data set is invalid
 *
 * @param dataType Identifier for data type
 * @param my_position Position from which to search
 * @return index of nearest point
 */

int find_closest_point(DataType dataType, PointXY my_position){
	//Retrieve reference to MapData
	MapData* map = MapData::instance();

	//Retrieve reference to ANN
	ANNInterface* kdTree = map->getAnnInterface(dataType);

	//Queried for an invalid tree
	if(kdTree == NULL)
		return -1;

	//Query for nearest index
	int closestIndex = kdTree->findClosestPoint(my_position);

	//Special handling for large features
	//This is because large features have multiple points for one feature around their perimeter
	//So we need to go back to the true index of the feature
	if(dataType == LargeFeatures)
		closestIndex = map->convertLargeFeaturePtIndexToIndex(closestIndex);

	return closestIndex;
}

/**
 * Autocompletes a string, using known street and POI names
 * Returns "" if nothing found
 * @param query the incomplete query
 * @return Best guess of string
 */
std::string autocompleteStringTopPriority(std::string query){
	//Get reference to trie
	MapData* map = MapData::instance();
	Trie* myTree = map->getMapTrie();

	//Query for string
	return myTree->queryTopWord(capitalizeAllWords(query));
}

/**
 * Autocompletes a string
 * Returns all possible words that can be made from the node, in decreasing priority
 * @param query word to autocomplete
 * @return vector of strings, sorted by importance
 */
std::vector<std::string> autocompleteAllResults(std::string query){
	//Get reference to trie
	MapData* map = MapData::instance();
	Trie* myTree = map->getMapTrie();

	//Query for vector
	return myTree->queryAllWords(capitalizeAllWords(query));
}


/**
 * Converts an array of largeFeaturePoint indices to largeFeature indices
 * Removes duplicates of same index
 * @param data The array largeFeaturePoints - modifies to contain largeFeature indices instead
 * @param numPts Number of largeFeaturePoints
 * @return Number of largeFeatures
 */
int convertAllToLargePointIndices (int* data, int numPts){
	//Get instance of map
	MapData* map = MapData::instance();

	//Iterate through all points
	for(int i = 0; i < numPts; i++)
		data[i] = map->convertLargeFeaturePtIndexToIndex(data[i]);

	int numUniquePoints = removeDuplicatesInArray(data, numPts);

	return numUniquePoints;
}



/**Overloaded function
 *
 * Searches for all points within a radius of a certain XY point
 * returns NULL if data set is invalid
 *
 * @param dataType Specify which data set to query (Intersection,
 * @param numPts RETURN (param) Number of points found
 * @param my_position Centre position around which to search
 * @param radius Distance to search
 * @return Array containing indices of found points
 */
int* find_points_within_radius(DataType dataType, int &numPts, PointXY my_position, double radius){
	int* dataPtr = NULL;
	double radiusSquared = radius*radius;

	//Retrieve reference to MapData
	MapData* map = MapData::instance();

	//Retrieve interface to ANN
	ANNInterface* kdTree= map->getAnnInterface(dataType);

	//Queried for invalid tree
	if (kdTree == NULL){
		numPts = 0;
		return NULL;
	}

	//Perform search
	dataPtr = kdTree->findPointsWithinRadius(my_position, radiusSquared, numPts);

	//Must correct values if data is of type large features
	//This is because large features have multiple points, to account for their size
	if (dataPtr != NULL && dataType == LargeFeatures)
		numPts = convertAllToLargePointIndices(dataPtr, numPts);

	return dataPtr;
}


/**Overloaded function
 *
 * Searches for all points within a radius of a certain LatLon point
 * returns NULL if data set is invalid
 *
 * @param dataType Specify which data set to query (Intersection,
 * @param numPts RETURN (param) Number of points found
 * @param my_position Centre position around which to search
 * @param radius Distance to search
 * @return Array containing indices of found points
 */
int* find_points_within_radius(DataType dataType, int &numPts, LatLon my_position, double radius){
	//Convert LatLon point to XY
	PointXY point;
	double scaleFactor = MapData::instance()->getScalingFactor();
	point = convertLatLonToXY(my_position, scaleFactor);

	//Call XY version of function for actual calculation
	return find_points_within_radius(dataType, numPts, point, radius);
}


/**	@DEPRECATED - LEFT ONLY FOR MILESTONE 1 FUNCTIONALITY
 * Use find_closest_point(...) instead
 *
 * Finds nearest point of interest to a given position
 *	Returns NULL if no POI's found
 *
 * @param my_position Current position
 * @return unsigned ID of nearest POI.
 */
int find_closest_point_of_interest(LatLon my_position){
	return find_closest_point(POI, my_position);
}

/**	@DEPRECATED - LEFT ONLY FOR MILESTONE 1 FUNCTIONALITY
 * Use find_closest_point(...) instead
 *
 *  find the nearest intersection (by ID) to a given position
 *
 * @param my_position Current position
 * @return unsigned ID of closest intersection
 */
int find_closest_intersection(LatLon my_position){
	return find_closest_point(SmallIntersections, my_position);
}

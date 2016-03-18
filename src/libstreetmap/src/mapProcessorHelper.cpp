/*
 * mapProcessorHelper.cpp
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

#include "mapProcessorHelper.h"

using boost::property_tree::ptree;

/**
 * Helper function for load_map.
 * Generate hash table from street names -> street IDs
 * @param map The MapData object to add the data to
 */
void _generateStreetNameHashTable(MapData* map) {
	nameIDMap *streetNameMap = map->getStreetNameMap();
	unsigned numStreets = map->getNumStreets();
	for (unsigned i=0; i<numStreets; i++) {
		streetNameMap->emplace(getStreetName(i), i);
	}
}

/**
 * Helper function for load_map.
 * Generate hash table inside each street object with keys being id's of streets
 * it connects to and values being id's of the intersection formed by
 * those two streets.
 * @param map The MapData object to add the data to
 */
void _generateStreetIntersectionHashTable(MapData* map) {
	unsigned numIntersections = map->getNumIntersections();
	for (unsigned i=0; i<numIntersections; i++) {
		// stores all streets at this intersection
		std::vector<unsigned> streetVec;
		// id of current street to (potentially) add to vector
		unsigned curStreet;
		// for avoiding repetitions when adding street ids to vector
		bool alreadyExists;

		// add all streets at the intersection into the vector
		unsigned segmentCount = getIntersectionStreetSegmentCount(i);
		for (unsigned j=0; j<segmentCount; j++) {
			// street to potentially be added
			curStreet = getStreetSegmentInfo(getIntersectionStreetSegment(i, j)).streetID;

			// check if street is already in the vector
			alreadyExists = false;
			for (unsigned k=0; k<streetVec.size(); k++) {
				if (streetVec[k] == curStreet) {
					alreadyExists = true;
				}
			}

			// only add to vector if not already present
			if (!alreadyExists) {
				streetVec.push_back(curStreet);
			}
		}

		// for each street, add an intersection with each other street (including itself) into its Street object
		for (unsigned j=0; j<streetVec.size(); j++) {
			for (unsigned k=0; k<streetVec.size(); k++) {
				// i is the ID of the intersection currently being processed
				map->getStreets()[streetVec[j]].addIntersection(streetVec[k], i);
			}
		}
	}
}

/**
 * Helper function for load_map.
 * Generate 4 arrays of intersection objects corresponding to different zoom levels, each containing an array of their connected
 * street segments that are large enough to be displayed at their given size.
 * @param map The MapData object to add the data to
 */
void _generateIntersections(MapData* map) {
	double scalingFactor = map->getScalingFactor();

	// to keep track of how many of each type of intersection there are
	unsigned numIntersections = map->getNumIntersections();
	unsigned numMediumSmallIntersections = 0;
	unsigned numMediumLargeIntersections = 0;
	unsigned numLargeIntersections = 0;

	// arrays of IntersectionXY objects inside MapData
	IntersectionXY** smallIntersections = map->getSmallIntersections();
	IntersectionXY** mediumSmallIntersections = map->getMediumSmallIntersections();
	IntersectionXY** mediumLargeIntersections = map->getMediumLargeIntersections();
	IntersectionXY** largeIntersections = map->getLargeIntersections();

	// basic info of each intersection
	std::string name;
	PointXY* coordinates;
	unsigned numStreetSegs;
	unsigned numLargeEnough;

	// to check if a street segment has already been added to another intersection, so that we can have a set
	// of intersections that only has unique street segments to improve drawing efficiency
	unsigned totalNumStreetSegs = map->getNumStreetSegments();
	bool* notYetProcessed = new bool[totalNumStreetSegs];

	// initialize notYetProcessed array
	for (unsigned k=0; k<totalNumStreetSegs; k++) {
		notYetProcessed[k] = true;
	}

	// iterate through all intersections
	for (unsigned i=0; i<numIntersections; i++) {
		// fill in basic information
		name = getIntersectionName(i);
		// array of size 1 to comply with MapElement specification
		coordinates = new PointXY[1];
		coordinates[0] = convertLatLonToXY(getIntersectionPosition(i), scalingFactor);
		numStreetSegs = getIntersectionStreetSegmentCount(i);
		numLargeEnough = 0;

		// every intersection gets added to smallIntersections, along with every attached street segment
		smallIntersections[i] = new IntersectionXY(name, coordinates, low, numStreetSegs);
		for (unsigned j=0; j<numStreetSegs; j++) {
			unsigned nextSegment = getIntersectionStreetSegment(i,j);

			// get next intersection ID - whether it's the "to" or "from" member needs to be determined
			StreetSegmentInfo nextSegInfo = map->getStreetSegments()[nextSegment].getInfo();
			unsigned destinationIntersection;
			if (nextSegInfo.from == i) {
				destinationIntersection = nextSegInfo.to;
			}
			else {
				destinationIntersection = nextSegInfo.from;
			}

			smallIntersections[i]->setStreetSeg(nextSegment, j, destinationIntersection, notYetProcessed[nextSegment]);
		}

		// intersections that have at least one street segment of size secondary or larger get added to
		// mediumSmall intersection

		// check if this intersection has a large enough street segment
		for (unsigned j=0; j<numStreetSegs; j++) {
			// smallIntersections contains all street segments
			unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];
			StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
			if (type != tertiary && type != unclassified) {
				numLargeEnough++;
			}
		}
		if (numLargeEnough > 0) {
			// allocate memory for new coordinates for this intersection
			PointXY* mediumSmallCoordinates = new PointXY[1];
			mediumSmallCoordinates[0] = convertLatLonToXY(getIntersectionPosition(i), scalingFactor);
			// at least one street segment is large enough, so add an intersection object and the street segments that
			// meet the threshold
			mediumSmallIntersections[numMediumSmallIntersections] = new IntersectionXY(name, mediumSmallCoordinates, medium, numLargeEnough);
			unsigned currentStreetSegIdx = 0;
			for (unsigned j=0; j<numStreetSegs; j++) {
				unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];

				StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
				if (type != tertiary && type != unclassified) {
					mediumSmallIntersections[numMediumSmallIntersections]->setStreetSeg(streetSegID, currentStreetSegIdx, notYetProcessed[streetSegID]);
					currentStreetSegIdx++;
				}
			}
			numMediumSmallIntersections++;
		}
		// reset counter for number of large enough street segments
		numLargeEnough = 0;

		// intersections that have at least one street segment of size primary or larger get added to
		// mediumLarge intersections

		// check if this intersection has a large enough street segment
		for (unsigned j=0; j<numStreetSegs; j++) {
			// smallIntersections contains all street segments
			unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];
			StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
			if (type == primary || type == motorway) {
				numLargeEnough++;
			}
		}
		if (numLargeEnough > 0) {
			// allocate memory for new coordinates for this intersection
			PointXY* mediumLargeCoordinates = new PointXY[1];
			mediumLargeCoordinates[0] = convertLatLonToXY(getIntersectionPosition(i), scalingFactor);
			// at least one street segment is large enough, so add an intersection object and the street segments that
			// meet the threshold
			mediumLargeIntersections[numMediumLargeIntersections] = new IntersectionXY(name, mediumLargeCoordinates, high, numLargeEnough);
			unsigned currentStreetSegIdx = 0;
			for (unsigned j=0; j<numStreetSegs; j++) {
				unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];

				StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
				if (type == primary || type == motorway) {
					mediumLargeIntersections[numMediumLargeIntersections]->setStreetSeg(streetSegID, currentStreetSegIdx, notYetProcessed[streetSegID]);
					currentStreetSegIdx++;
				}
			}
			numMediumLargeIntersections++;
		}
		// reset counter for number of large enough street segments
		numLargeEnough = 0;

		// intersections that have at least one street segment of size motorway get added to
		// large intersections

		// check if this intersection has a large enough street segment
		for (unsigned j=0; j<numStreetSegs; j++) {
			// smallIntersections contains all street segments
			unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];
			StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
			if (type == motorway) {
				numLargeEnough++;
			}
		}
		if (numLargeEnough > 0) {
			// allocate memory for new coordinates for this intersection
			PointXY* largeCoordinates = new PointXY[1];
			largeCoordinates[0] = convertLatLonToXY(getIntersectionPosition(i), scalingFactor);
			// at least one street segment is large enough, so add an intersection object and the street segments that
			// meet the threshold
			largeIntersections[numLargeIntersections] = new IntersectionXY(name, largeCoordinates, always, numLargeEnough);
			unsigned currentStreetSegIdx = 0;
			for (unsigned j=0; j<numStreetSegs; j++) {
				unsigned streetSegID = smallIntersections[i]->getStreetSegs()[j];

				StreetSegmentType type = map->getStreetSegments()[streetSegID].getStreetType();
				if (type == motorway) {
					largeIntersections[numLargeIntersections]->setStreetSeg(streetSegID, currentStreetSegIdx, notYetProcessed[streetSegID]);
					currentStreetSegIdx++;
				}
			}
			numLargeIntersections++;
		}

		// keep track of the street segments we've processed for this intersection
		for (unsigned j=0; j<numStreetSegs; j++) {
			notYetProcessed[smallIntersections[i]->getStreetSegs()[j]] = false;
		}
	}

	delete[] notYetProcessed;

	// set totals in MapData
	map->setNumMediumSmallIntersections(numMediumSmallIntersections);
	map->setNumMediumLargeIntersections(numMediumLargeIntersections);
	map->setNumLargeIntersections(numLargeIntersections);
}


/**
 * Helper function for load_map.
 * Generate all FeatureXY objects from the streets database and put them in MapData.
 * @param map The MapData object to add the data to.
 */
void _generateFeatureObjects(MapData* map) {
	// tmp values used for creating each feature
	unsigned numFeatures = getNumberOfFeatures();
	double scalingFactor = map->getScalingFactor();
	FeatureType featureType;

	// generate mapping from feature wayOSMID to OSMDatabaseAPI index: key is wayOSMID, value is OSMDatabaseAPI index
	// this is used for getting building height data from OSM database
	unsigned numWays = getNumberOfWays();
	std::map<unsigned, unsigned> wayOSMIDtoIndex;
	for (unsigned i=0; i<numWays; i++) {
		// way's id member is the wayOSMID, i is the index for OSMDatabaseAPI
		const OSMWay* way = getWayByIndex(i);
		wayOSMIDtoIndex.emplace(way->id(), i);
	}

	// fill each element with data from StreetsDatabaseAPI
	for (unsigned i=0; i<numFeatures; i++) {
		// get data for this feature
		featureType = getFeatureType(i);
		DrawPriority drawPriority;

		// the height of the feature (currently only used for buildings)
		double featureHeight = 0;

		// calculate and store x,y coordinates in a 2D array, allocated here and freed in the feature's destructor
		unsigned numPts = getFeaturePointCount(i);
		PointXY* coordinates;
		coordinates = new PointXY[numPts];
		for (unsigned j=0; j<numPts; j++) {
			coordinates[j] = convertLatLonToXY(
						getFeaturePoint(i, j), // the LatLon coordinates of this point
						scalingFactor
						);
		}

		// determine drawPriority from feature type
		// large features (require special calculations since they may be larger than viewing window)
		if (featureType == Beach || featureType == Lake || featureType == Shoreline) {
			drawPriority = always;

			// add every LARGE_FEATURE_POINT_DIVISION'th feature points to vector, for the purposes of determining if this feature
			// is onscreen when drawing
			for (unsigned j=0; j<numPts; j+= LARGE_FEATURE_POINT_DIVISION) {
				map->addLargeFeaturePt(coordinates[j], i);
			}
		}
		// large features (shown at most zoom levels)
		else if (featureType == Park || featureType == Greenspace || featureType == Golfcourse || featureType == Island) {
			drawPriority = high;
		}
		// medium features (shown at most zoom levels)
		else if (featureType == River || featureType == Stream) {
			drawPriority = medium;
		}
		// small features (buildings & unknown) (only shown on high zoom)
		else if (featureType == Building) {
			// extract building height from OSM Database
			OSMID featureOSMID = getFeatureOSMID(i);
			unsigned wayIndex = wayOSMIDtoIndex.at(featureOSMID);
			const OSMWay* featureOSMWay = getWayByIndex(wayIndex);

			// find height key - value gives which type of street it is
			unsigned numTags = getTagCount(featureOSMWay);
			// used to read each tag
			std::pair<std::string, std::string> tag;

			// iterate through all tags, looking for the height of the building
			for (unsigned j=0; j<numTags; j++) {
				tag = getTagPair(featureOSMWay, j);
				if (tag.first == "height") {
					// parse height into readable format
					// TODO: SUPPORT BUILDING HEIGHTS IN IMPERIAL UNITS
					std::string heightStr = tag.second;
					double usable = true;
					// check that the height is in metric units, with form either "12.3" or "12.3 m"
					for (unsigned k=0; k< (heightStr.length() - 2); k++) {
						if (!isdigit(heightStr[k]) && heightStr[k] != 'm') {
							usable = false;
							break;
						}
					}
					// convert the height into a double we can store
					if (usable) {
						featureHeight = atof(heightStr.c_str());
					}

					// since we've found the height tag, no need to keep looking
					break;
				}
				else if (tag.first == "building:levels") {
					std::string levelStr = tag.second;
					int featureLevels = 0;
					if (levelStr.length() != 0) {
						featureLevels = std::strtol(levelStr.c_str(), NULL, 10);
					}
					// approximation of each level being 3m tall
					featureHeight = (double) featureLevels * 3.0;

					// since we've found the levels tag, no need to keep looking
					break;
				}
			}

			// if the building height and levels were not specified, we will assume 2 stories/6 metres tall
			if (featureHeight == 0.0) {
				featureHeight = 6;
			}

			drawPriority = low;
		}
		else {
			drawPriority = low;
		}

		// the next feature to add to the array
		FeatureXY *toAdd = new FeatureXY(
			getFeatureName(i), // name of the feature
			coordinates, // the xy points that define the feature
			numPts, // the number of points
			drawPriority,
			getFeatureOSMID(i),
			featureType,
			featureHeight
			);

		// add feature to MapData
		if (drawPriority == always) {
			map->getLargeFeatures()[map->getNumLargeFeatures()] = toAdd;
			map->incrementLargeFeaturesCount();
		}
		else if (drawPriority == high) {
			map->getMediumLargeFeatures()[map->getNumMediumLargeFeatures()] = toAdd;
			map->incrementMediumLargeFeaturesCount();
		}
		else if (drawPriority == medium) {
			map->getMediumSmallFeatures()[map->getNumMediumSmallFeatures()] = toAdd;
			map->incrementMediumSmallFeaturesCount();
		}
		else {
			map->getSmallFeatures()[map->getNumSmallFeatures()] = toAdd;
			map->incrementSmallFeaturesCount();
		}
	}
}


/**
 * Helper function for load_map.
 * Generate all StreetSegment objects from the streets database and put them in MapData
 * and their associated Street objects.
 * @param map The MapData object to add the data to.
 */
void _generateStreetSegmentObjects(MapData* map) {
	StreetSegment* streetSegments = map->getStreetSegments();
	unsigned numStreetSegs = map->getNumStreetSegments();
	double scalingFactor = map->getScalingFactor();
	StreetSegmentInfo streetSeg;
	PointXY curPoint;

	// generate mapping from street segment wayOSMID to OSMDatabaseAPI index: key is wayOSMID
	// value is index
	unsigned numWays = getNumberOfWays();
	std::map<unsigned, unsigned> wayOSMIDtoIndex;
	for (unsigned i=0; i<numWays; i++) {
		// way's id member is the wayOSMID, i is the index for OSMDatabaseAPI
		const OSMWay* way = getWayByIndex(i);
		wayOSMIDtoIndex.emplace(way->id(), i);
	}

	// iterate through all street segments
	for (unsigned i=0; i<numStreetSegs; i++) {
		// initialize street segment with StreetsDatabaseAPI data
		streetSegments[i].initialize(i);
		streetSeg = streetSegments[i].getInfo();

		// add starting intersection as first curve point
		curPoint = convertLatLonToXY(getIntersectionPosition(streetSeg.from), scalingFactor);

		streetSegments[i].addCurvePoint(curPoint);

		// iterate through all curve points
		for (unsigned j=0; j<= streetSeg.curvePointCount; j++) {
			// check if reached end, then next point is the destination intersection
			if (j == streetSeg.curvePointCount) {
				curPoint = convertLatLonToXY(getIntersectionPosition(streetSeg.to), scalingFactor);
			}
			// else, use next curve point
			else {
				curPoint = convertLatLonToXY(getStreetSegmentCurvePoint(i,j), scalingFactor);
			}

			// add curve point to StreetSegment object, add to length
			streetSegments[i].addCurvePoint(curPoint);
		}

		// determine whether street segment is suitable to draw a name on
		streetSegments[i].determineSuitableForName();

		// add length of street segment to streetSegmentInfo object
		// TODO: calculate this alongside finding X,Y coords of curve points to save processing time
		streetSegments[i].setLength(_calculate_street_segment_length(i));

		// add street type to streetsegment object
		OSMID segmentOSMID = getStreetSegmentInfo(i).wayOSMID;
		unsigned wayIndex = wayOSMIDtoIndex.at(segmentOSMID);
		const OSMWay* segmentOSMWay = getWayByIndex(wayIndex);

		// find highway key - value gives which type of street it is
		unsigned numTags = getTagCount(segmentOSMWay);
		// used to read each tag
		std::pair<std::string, std::string> tag;

		for (unsigned j=0; j<numTags; j++) {
			tag = getTagPair(segmentOSMWay, j);
			if (tag.first == "highway") {
				// convert from string to enum for efficiency when used later
				if (tag.second == "motorway") {
					streetSegments[i].setStreetType(motorway);
				}
				else if (tag.second == "trunk" || tag.second == "primary") {
					streetSegments[i].setStreetType(primary);
				}
				else if (tag.second == "secondary" || tag.second == "residential") {
					streetSegments[i].setStreetType(secondary);
				}
				else if (tag.second == "tertiary" || tag.second == "service") {
					streetSegments[i].setStreetType(tertiary);
				}
				else {
					streetSegments[i].setStreetType(unclassified);
				}

				// no need to continue, since we've found the street type
				break;
			}
		}

		// For each street segment, add its id to the corresponding street object
		map->getStreets()[streetSeg.streetID].addStreetSegment(i);
	}
}

/**
 * Helper function for load_map.
 * Generate all PoiXY objects from the streets database and put them in MapData
 * @param map The MapData object to add the data to.
 */
void _generatePOIObjects(MapData* map) {
	double scalingFactor = map->getScalingFactor();
	unsigned numPOI = map->getNumPOI();
	PoiXY** pointsOfInterest = map->getPointsOfInterest();
	// 1-point coordinates array, used to construct each PoiXY object
	PointXY* coordinates;
	// map from POI name to POI ID
	nameIDMap *poiNameMap = map->getPOINameMap();

	for (unsigned i=0; i<numPOI; i++) {
		// get XY coordinates of this POI
		coordinates = new PointXY[1]; // array of size 1 is to meet specification of MapElement
		coordinates[0] = convertLatLonToXY(
				getPointOfInterestPosition(i),
				scalingFactor);
		// generate PoiXY object, add it to MapData
		std::string poiName = getPointOfInterestName(i);
		pointsOfInterest[i] = new PoiXY(poiName, coordinates);

		// add this to the POI name map
		poiNameMap->emplace(std::make_pair(poiName, i));
	}
}

/**
 * Loads events from specified XML file into Event objects in MapData.
 * @param eventFile The name of the event XML file (assumes location of mapper/Events/<eventFile>)
 * @param city The name of the city to get events for (e.g. toronto)
 * @param map The MapData instance
 */
void _loadEvents(std::string eventFile, std::string city, MapData* map) {
	// property tree for parsing XML file
	ptree pt;
	read_xml("mapper/Events/" + eventFile, pt);

	// iterate through all events
	//const char* cityEvent = (city + ".event").c_str();
	BOOST_FOREACH(const ptree::value_type &v, pt.get_child("toronto")) {
		// create new event object
		Event toAdd(
				v.second.get<std::string>("name"),
				v.second.get<std::string>("address"),
				v.second.get<double>("latitude"),
				v.second.get<double>("longitude"),
				v.second.get<std::string>("description")
				);
		// add categories to it
		ptree categories = v.second.get_child("category");
		//for (const auto& categoryPair : categories) {
		BOOST_FOREACH(const ptree::value_type &v2, v.second.get_child("category")) {
			EventType category;
			std::string categoryText = v2.second.data();
			if (categoryText == "concert") {
				category = concert;
			}
			else if (categoryText == "festival") {
				category = festival;
			}
			else if (categoryText == "comedy") {
				category = comedy;
			}
			else if (categoryText == "family") {
				category = family;
			}
			else if (categoryText == "nightlife") {
				category = nightlife;
			}
			else if (categoryText == "sports") {
				category = sports;
			}
			else {
				category = unknown;
			}
			toAdd.addCategory(category);
		}
		// add event to MapData
		map->getEvents()->push_back(toAdd);
	}
}

/**
 * Calculates drawing offset and drawing scale factor to convert map coordinates (in units of m,
 * centered relative to the location on the globe) to drawing coordinates (with a range of X values
 * from -1000 to 1000 and the center of the map at 0,0). Updates all FeatureXY, PoiXY, IntersectionXY
 * to have a copy of these coordinates in addition to their existing "real" coordinates;
 * @param map The MapData instance
 */
void _drawingScaleConversion(MapData* map) {
	// find extremes and average values of XY coordinates on the map
	double minX = 999999999;
	double maxX = -999999999;
	double minY = 999999999;
	double maxY = -999999999;

	// small intersections contains all intersections
	unsigned numIntersections = map->getNumIntersections();
	IntersectionXY** intersections = map->getSmallIntersections();
	PointXY coordinates;

	double averageX = 0;
	double averageY = 0;

	// iterate over all intersections and find extreme and average values
	for (unsigned i=0; i<numIntersections; i++) {
		coordinates = intersections[i]->getCoordinates()[0];
		averageX += coordinates.x;
		averageY += coordinates.y;
		if (coordinates.x < minX) {
			minX = coordinates.x;
		}
		else if (coordinates.x > maxX) {
			maxX = coordinates.x;
		}
		if (coordinates.y < minY) {
			minY = coordinates.y;
		}
		else if (coordinates.y > maxY) {
			maxY = coordinates.y;
		}
	}
	// divide by number of features to get average
	averageX /= (double) numIntersections;
	averageY /= (double) numIntersections;

	PointXY offset(averageX, averageY);

	// scaling factor is the number we need to divide the now-centered coordinates by to make the
	// X values range from -1000 to 1000
	double drawScaleFactor = 2000.0 / (maxX - minX);

	// upper and lower bounds of the map
	PointXY bottomLeft(minX, minY);
	PointXY topRight(maxX, maxY);

	map->setDrawConversion(offset, drawScaleFactor, bottomLeft, topRight);

	// convert all points of all types of objects: setDrawCoordinates creates the draw coordinates based
	// on the existing XY coordinates and the given offset and scaleFactor
	for (unsigned i=0; i<numIntersections; i++) {
		intersections[i]->setDrawCoordinates(offset, drawScaleFactor);
	}
	FeatureXY** smallFeatures = map->getSmallFeatures();
	unsigned numSmallFeatures = map->getNumSmallFeatures();
	for (unsigned i=0; i<numSmallFeatures; i++) {
		smallFeatures[i]->setDrawCoordinates(offset, drawScaleFactor);
	}

	FeatureXY** mediumSmallFeatures = map->getMediumSmallFeatures();
	unsigned numMediumSmallFeatures = map->getNumMediumSmallFeatures();
	for (unsigned i=0; i<numMediumSmallFeatures; i++) {
		mediumSmallFeatures[i]->setDrawCoordinates(offset, drawScaleFactor);
	}

	FeatureXY** mediumLargeFeatures = map->getMediumLargeFeatures();
	unsigned numMediumLargeFeatures = map->getNumMediumLargeFeatures();
	for (unsigned i=0; i<numMediumLargeFeatures; i++) {
		mediumLargeFeatures[i]->setDrawCoordinates(offset, drawScaleFactor);
	}

	FeatureXY** largeFeatures = map->getLargeFeatures();
	unsigned numLargeFeatures = map->getNumLargeFeatures();
	for (unsigned i=0; i<numLargeFeatures; i++) {
		largeFeatures[i]->setDrawCoordinates(offset, drawScaleFactor);
	}

	PoiXY** POIs = map->getPointsOfInterest();
	unsigned numPOI = map->getNumPOI();
	for (unsigned i=0; i<numPOI; i++) {
		POIs[i]->setDrawCoordinates(offset, drawScaleFactor);
	}

	StreetSegment* streetSegments = map->getStreetSegments();
	unsigned numStreetSegs = map->getNumStreetSegments();
	for (unsigned i=0; i<numStreetSegs; i++) {
		streetSegments[i].setDrawCoordinates(offset, drawScaleFactor);
	}
}

/**
 * Calculates length of a street segment
 *
 * @param street_segment_id ID of segment to calculate
 * @return double Length of the street
 */
double _calculate_street_segment_length(unsigned street_segment_id){
	StreetSegmentInfo streetInfo = getStreetSegmentInfo(street_segment_id);

	double segmentDistance = 0;
	int numCurves = streetInfo.curvePointCount;

	//Initialize previous point as starting point
	LatLon prePoint = getIntersectionPosition(streetInfo.from);
	LatLon curPoint;

	//Sum distances between curve points
	for (int x = 0; x<= numCurves; x++){
		//End reached - connect to street's endpoint
		if (x == numCurves) {
			curPoint = getIntersectionPosition(streetInfo.to);
		}
		//More curve points - load next curve point
		else {
			curPoint = getStreetSegmentCurvePoint(street_segment_id,x);
		}
		//Add curves' distance, prepare for next segment
		segmentDistance += find_distance_between_two_points(curPoint, prePoint);
		prePoint = curPoint;
	}

	return segmentDistance;
}

/* 
 * File:   MapData.h
 * Author: William Kingsford
 *
 * Created on January 21, 2016, 12:07 PM
 */

#ifndef MAPDATA_H
#define MAPDATA_H

#include "Street.h"
#include "FeatureXY.h"
#include "StreetSegment.h"
#include "StreetsDatabaseAPI.h"
#include "ANN/ANN.h"
#include "ANN/ANNInterface.h"
#include "Trie.h"
#include "Event.h"
#include "PoiXY.h"
#include "IntersectionXY.h"

#include<assert.h>

#define LARGE_FEATURE_POINT_DIVISION 50

/**
 * nameIDMap is a map from string->unsigned, used for mapping street names to streetID's
 */
typedef std::unordered_multimap<std::string, unsigned> nameIDMap;

/**
 * Singleton object to hold all data structures containing pre-processed map data.
 */
class MapData {
public:
    /**
     * Gets the MapData instance
     * @return pointer to the MapData instance
     */
    static MapData* instance();

    /**
     * Gets the array of Street objects, where the array index is the streetID
     * @return pointer to first object in street array
     */
    Street* getStreets();

    /**
     * Gets the array of small FeatureXY objects
     * @return array of pointers to features.
     */
    FeatureXY** getSmallFeatures() const {
    	return smallFeatures;
    }

    /**
	 * Gets the array of medium-small FeatureXY objects
	 * @return array of pointers to features.
	 */
	FeatureXY** getMediumSmallFeatures() const {
		return mediumSmallFeatures;
	}

	/**
	 * Gets the array of medium-large FeatureXY objects
	 * @return array of pointers to features.
	 */
	FeatureXY** getMediumLargeFeatures() const {
		return mediumLargeFeatures;
	}

	/**
	 * Gets the array of large FeatureXY objects
	 * @return array of pointers to features.
	 */
	FeatureXY** getLargeFeatures() const {
		return largeFeatures;
	}

	/**
	 * Gets the vector of large feature point locations, used by ANN to tell if a feature that
	 * is larger than the screen should be drawn by checking more than one point along its boundary
	 * @return
	 */
    std::vector<PointXY> getLargeFeaturePoints() {
    	return largeFeaturePoints;
    }

    /**
     * Gets the array of small intersections (includes all intersections, containing all street segments)
     * @return Array of pointers to intersectionXY objects for small intersections
     */
    IntersectionXY** getSmallIntersections() {
    	return smallIntersections;
    }

    /**
	 * Gets the array of medium-small intersections (includes all intersections that have at least one street segment
	 * of type secondary or larger, containing all attached street segments that meet this size requirement)
	 * @return Array of pointers to intersectionXY objects for medium-small intersections
	 */
	IntersectionXY** getMediumSmallIntersections() {
		return mediumSmallIntersections;
	}

	/**
	 * Gets the array of medium-large intersections (includes all intersections that have at least one street segment
	 * of type primary or larger, containing all attached street segments that meet this size requirement)
	 * @return Array of pointers to intersectionXY objects for medium-large intersections
	 */
	IntersectionXY** getMediumLargeIntersections() {
		return mediumLargeIntersections;
	}

	/**
	 * Gets the array of large intersections (includes all intersections that have at least one street segment
	 * of type motorway, containing all attached street segments that meet this size requirement)
	 * @return Array of pointers to intersectionXY objects for large intersections
	 */
	IntersectionXY** getLargeIntersections() {
		return largeIntersections;
	}

    /**
     * Gets the array of StreetSegment objects, where the array index is the streetSegmentID
     * @return pointer to first object in streetsegment array
     */
    StreetSegment* getStreetSegments();

    /**
     * Gets the array of point of interest objects, where the array index is the point of interest ID
     * @return Array of pointers to PoiXY objects
     */
    PoiXY** getPointsOfInterest();

    /**
     * Gets the vector of Event objects
     * @return pointer to vector of Event objects
     */
    std::vector<Event> *getEvents();

    /**
     * Gets the map of street names => streetID's
     * @return the map of street names => streetID's
     */
    nameIDMap* getStreetNameMap();

    /**
     * Gets the map of POI names => POI ID's
     * @return the map of POI names => POI ID's
     */
    nameIDMap* getPOINameMap() {
    	return poiNameMap;
    }

	/**
	 * Returns pointer to trie, used for auto-correction
	 * @return pointer to trie
	 */
    Trie* getMapTrie(){
    	return mapTrie;
    }


    /**
     * Get total number of streets in map
     * @return total number of streets
     */
    unsigned getNumStreets() {
        return numStreets;
    }

    /**
     * Get total number of street segments in map
     * @return total number of street segments
     */
    unsigned getNumStreetSegments() {
        return numStreetSegments;
    }

    /**
     * Get total number of intersections in map
     * @return total number of intersections
     */
    unsigned getNumIntersections() {
        return numIntersections;
    }

    /**
     * Get total number of medium-small or larger intersections in the map
     * @return
     */
    unsigned getNumMediumSmallIntersections() {
    	return numMediumSmallIntersections;
    }

    /**
	 * Get total number of medium-large or larger intersections in the map
	 * @return
	 */
	unsigned getNumMediumLargeIntersections() {
		return numMediumLargeIntersections;
	}

	/**
	 * Get total number of large intersections in the map
	 * @return
	 */
	unsigned getNumLargeIntersections() {
		return numLargeIntersections;
	}

	/**
	 * Sets the total number of medium-small intersections on the map. Only used when pre-loaded map isn't used
	 * @param num
	 */
	void setNumMediumSmallIntersections(unsigned num) {
		numMediumSmallIntersections = num;
	}

	/**
	 * Sets the total number of medium-large intersections on the map. Only used when pre-loaded map isn't used
	 * @param num
	 */
	void setNumMediumLargeIntersections(unsigned num) {
		numMediumLargeIntersections = num;
	}

	/**
	 * Sets the total number of large intersections on the map. Only used when pre-loaded map isn't used
	 * @param num
	 */
	void setNumLargeIntersections(unsigned num) {
		numLargeIntersections = num;
	}

    /**
     * Get total number of points of interest in map
     * @return total number of points of interest
     */
    unsigned getNumPOI() {
        return numPOI;
    }

    /**
	 * Get total number of small features in map
	 * @return total number of small features
	 */
	unsigned getNumSmallFeatures() {
		return numSmallFeatures;
	}

	/**
	 * Get total number of medium-small features in map
	 * @return total number of medium-small features
	 */
	unsigned getNumMediumSmallFeatures() {
		return numMediumSmallFeatures;
	}

	/**
	 * Get total number of medium-large features in map
	 * @return total number of medium-large features
	 */
	unsigned getNumMediumLargeFeatures() {
		return numMediumLargeFeatures;
	}

	/**
	 * Get total number of large features in map
	 * @return total number of large features
	 */
	unsigned getNumLargeFeatures() {
		return numLargeFeatures;
	}

	/**
	 * Get total number of large feature points in map
	 * @return total number of large feature points
	 */
	unsigned getNumLargeFeaturePoints() {
		return largeFeaturePoints.size();
	}

	/**
	 * Adds one to the total count of small features
	 */
	void incrementSmallFeaturesCount() {
		numSmallFeatures++;
	}

	/**
	 * Adds one to the total count of medium-small features
	 */
	void incrementMediumSmallFeaturesCount() {
		numMediumSmallFeatures++;
	}

	/**
	 * Adds one to the total count of medium-large features
	 */
	void incrementMediumLargeFeaturesCount() {
		numMediumLargeFeatures++;
	}

	/**
	 * Adds one to the total count of large features
	 */
	void incrementLargeFeaturesCount() {
		numLargeFeatures++;
	}

	/**
	 * Gives the index of the large feature the large feature index is part of
	 * @param largeFeaturePtIdx The index of the large feature point
	 * @return The index of the large feature
	 */
	unsigned convertLargeFeaturePtIndexToIndex(unsigned largeFeaturePtIdx);

	/**
	 * Add a large feature point for ANN purposes, keep track of what large feature this is part of.
	 * @param point The XY location of the feature point
	 * @param featureIdx The index of the feature this point is a part of
	 */
	void addLargeFeaturePt(PointXY point, unsigned featureIdx);

	/**
	 * Determines whether a point is inside a given feature (used to still draw a large feature when screen is zoomed
	 * far enough in on the center of the feature that none of its edges are within drawing radius)
	 * @param featureIdx The index of the feature
	 * @param X The x location to check
	 * @param Y The y location to check
	 * @param drawPriority Used to determine which array of feature the index refers to
	 * @return True if the point is inside the feature, false otherwise
	 */
	bool checkPointInsideFeature(unsigned featureIdx, double X, double Y, DrawPriority drawPriority);

    /**
     * Returns pointer to desired ANN interface
     * @param dataType The data set required (e.g. Intersections, POI)
     * @return requested pointer to ANNInterface
     */
    ANNInterface * getAnnInterface(DataType dataType);



    /**
	 * Gets the scaling factor for LatLon -> XY conversions
	 * @return the scaling factor for the map's LatLon -> XY conversions
	 */
	double getScalingFactor(){
		return scalingFactor;
	}


	/**
	 * Sets the scaling factor for LatLon -> XY conversions
	 * @param _scalingFactor the scaling factor for the map's LatLon -> XY conversions
	 */
	void setScalingFactor(double _scalingFactor) {
		scalingFactor = _scalingFactor;
	}

	/**
	 * Converts map coordinates (in units of km, centered relative to the location on the globe)
	 * to drawing coordinates (with a range of X values from -1000 to 1000 and the center of the map at 0,0)
	 * @param original Point in map coordinates
	 * @return Point in drawing coordinates
	 */
	PointXY convertToDrawCoordinates(PointXY original) {
		PointXY converted;
		converted.x = (original.x - drawingMapOffset.x) * drawingMapScale;
		converted.y = (original.y - drawingMapOffset.y) * drawingMapScale;
		return converted;
	}

	/**
	 * Converts drawing  coordinates (with a range of X values from -1000 to 1000 and the center of the map at 0,0)
	 * to drawing coordinates (in units of km, centered relative to the location on the globe)
	 * @param original Point in drawing coordinates
	 * @return Point in map coordinates
	 */
	PointXY convertDrawToMapCoordinates(PointXY drawCoordinates) {
		PointXY converted;
		converted.x = (drawCoordinates.x / drawingMapScale) + drawingMapOffset.x;
		converted.y = (drawCoordinates.y / drawingMapScale) + drawingMapOffset.y;
		return converted;
	}

	/**
	 * Sets all the values needed to convert from map coordinates to drawing coordinates
	 * @param _drawingMapOffset A pointXY representing the center of the map in map coordinates
	 * @param _drawingMapScale A number to multiply the coordinates by to scale the X values into a range of -1000 to 1000
	 * @param _bottomLeft A pointXY containing the lowest X and Y values of intersections on the map
	 * @param _topRight A pointXY containing the highest X and Y values of intersections on the map
	 */
	void setDrawConversion(PointXY _drawingMapOffset, double _drawingMapScale, PointXY _bottomLeft, PointXY _topRight) {
		drawingMapOffset = _drawingMapOffset;
		drawingMapScale = _drawingMapScale;
		bottomLeft = _bottomLeft;
		topRight = _topRight;
	}

	/**
	 * Get a pointXY containing the lowest X and Y values of intersections on the map
	 * @return
	 */
	PointXY getLowerBounds() {
		return bottomLeft;
	}

	/**
	 * Get a pointXY containing the highest X and Y values of intersections on the map
	 * @return
	 */
	PointXY getUpperBounds() {
		return topRight;
	}

	/**
	 * Get the scaling factor that you multiply with map coordinates to scale to drawing coordinates after doing an offset
	 * @return
	 */
	double getDrawingMapScale() const {
		return drawingMapScale;
	}

    /**
     * Get the vector of street segments at a given intersection
     * @param intersectionID The intersection to get the street segments for
     * @return the vector of street segments at a given intersection
     */
    std::vector<unsigned> getIntersectionStreetSegments(unsigned intersectionID);

    /**
     * Deletes streets and streetnameMap, resets all counts to 0.
     * Can be used before loading a new map, or when it is time to end the program.
     */
    void emptyData();

    /**
     * Initializes and loads data for all KD trees
     * Should be called once, in load_map
     * NOTE: Scaling factor must be calculated before calling this function
     */
    void generateKdTrees();

    /**
     * Destructor, deletes all data structures and frees all associated memory
     */
    virtual ~MapData();

private:
    /**
     * Private default constructor for singleton, initializes array of street objects
     */
    MapData();

    /**
     * The single MapData instance
     */
    static MapData* mapData;


    /**
     * Array of Street objects, size is total number of streets and index is street ID
     */
    Street* streets;

    /**
     * Arrays of FeatureXY objects, array size is total number of features of that size
     */
    FeatureXY **smallFeatures;
    FeatureXY **mediumSmallFeatures;
    FeatureXY **mediumLargeFeatures;
    FeatureXY **largeFeatures;

    /**
     * Stores every LARGE_FEATURE_POINT_DIVISION'th LatLon point of every largeFeature in one array.
     * Can use convertLargeFeaturePtIndexToIndex to convert the indices from this array back to the corresponding
     * index in the features array.
     */
    std::vector<PointXY> largeFeaturePoints;

    /**
     * Index is the index of a large feature point, value is index of the large feature the point is part of
     */
    std::vector<unsigned> largeFeaturePointToIndex;

    /**
	 * Array of street segment objects, size is total number of street segments and index is street segment ID
	 */
	StreetSegment* streetSegments;

	/**
	 * Array of pointers to PoiXY objects, size is total number of points of interest and index is POI ID
	 */
	PoiXY** pointsOfInterest;

	/**
	 * Vector of event objects
	 */
	std::vector<Event>* events;

	/**
	 * Arrays of pointers to IntersectionXY objects. Each category fully contains those below it, e.g. mediumLargeIntersections
	 * contains every largeIntersection, but they are different objects as smaller sizes also include smaller street segments.
	 */
	IntersectionXY** smallIntersections;
	IntersectionXY** mediumSmallIntersections;
	IntersectionXY** mediumLargeIntersections;
	IntersectionXY** largeIntersections;

    /**
     * Map from street names to streetID's
     */
    nameIDMap* streetNameMap;

    /**
     * Map from point of interest names to point of interest IDs
     */
    nameIDMap* poiNameMap;

    /**
     * Offset and scale factor for converting from map coordinates (in units of km,
     * centered relative to the location on the globe) to drawing coordinates (with
     * a range of X values from -1000 to 1000 and the center of the map at 0,0)
     */
    PointXY drawingMapOffset;
    double drawingMapScale;

    /**
     * Upper and lower bounds of the map coordinates
     */
    PointXY bottomLeft;
    PointXY topRight;

    /**
     * Total numbers of various elements to save having to compute them each time
     */
    unsigned numStreets;
    unsigned numStreetSegments;
    unsigned numPOI;

    unsigned numIntersections;
    unsigned numMediumSmallIntersections;
    unsigned numMediumLargeIntersections;
    unsigned numLargeIntersections;

    unsigned numFeatures;
    unsigned numSmallFeatures;
    unsigned numMediumSmallFeatures;
    unsigned numMediumLargeFeatures;
    unsigned numLargeFeatures;

    /**
     * Contains k-d tree containing location data, for nearest neighbour/radius searching
     */
    ANNInterface* smallIntersectionKd;
    ANNInterface* mediumSmallIntersectionKd;
    ANNInterface* mediumLargeIntersectionKd;
    ANNInterface* largeIntersectionKd;
    ANNInterface* poiKd;
    ANNInterface* smallFeatureKd;
    ANNInterface* mediumSmallFeatureKd;
    ANNInterface* mediumLargeFeatureKd;
    ANNInterface* largeFeatureKd;
    ANNInterface* eventKd;

    /**
     * Trie containing all strings for auto completion
     */

    Trie* mapTrie;

    /**
     * Scaling factor conversions from longitude to X
     */
    double scalingFactor;
};

/**
 * Returns location of small feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Small feature index
 * @return XY location of feature at index
 */
PointXY getSmallFeatureLocation(unsigned i);

/**
 * Returns location of medium-small feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Medium-small feature index
 * @return XY location of feature at index
 */
PointXY getMediumSmallFeatureLocation(unsigned i);

/**
 * Returns location of medium-large feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Medium-large feature index
 * @return XY location of feature at index
 */
PointXY getMediumLargeFeatureLocation(unsigned i);

/**
 * Returns location of large feature with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @param i Large feature index
 * @return XY location of feature at index
 */
PointXY getLargeFeatureLocation(unsigned i);

/**
 * Get location of given large feature point (several of these may correspond to one feature)
 * It is not a MapData member so that it can be used as a callback function for ANNInterface constructor
 * @param i Index corresponding to the largeFeaturePoints array
 * @return XY location of large feature point
 */
PointXY getLargeFeaturePointLocation(unsigned i);

/**
 * Gets the location of the event with index i. It is not a MapData member so that it can be used as a callback
 * function for ANNInterface constructor
 * @return XY location of the event
 */
PointXY getEventLocation(unsigned i);

/**
 * Gets the location of a small intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getSmallIntersectionPositionXY(unsigned i);

/**
 * Gets the location of a medium-small intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getMediumSmallIntersectionPositionXY(unsigned i);

/**
 * Gets the location of a medium-large intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getMediumLargeIntersectionPositionXY(unsigned i);

/**
 * Gets the location of a large intersection, used for ANN to build a kd tree
 * @param i The intersection ID
 * @return XY location of the intersection
 */
PointXY getLargeIntersectionPositionXY(unsigned i);

/**
 * Gets the location of a POI, used for ANN to build a kd tree
 * @param i The poi ID
 * @return XY location of the poi
 */
PointXY getPOIPositionXY(unsigned i);

#endif /* MAPDATA_H */


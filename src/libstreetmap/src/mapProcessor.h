 #pragma once //protects against multiple inclusions of this header file

#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include "mapProcessorHelper.h"
#include "utility.h"
#include "StreetsDatabaseAPI.h"
#include "MapData.h"
#include "DijkstraData.h"
#include "Trie.h"


// for OSM Layer 1 API
#include "OSMDatabaseAPI.h"

//function to load map streets.bin file
bool load_map(std::string map_name);

//close the loaded map
void close_map();



/**
 * function to return street id(s) for a street name
 * return a 0-length vector if no street with this name exists.
 * @param street_name
 * @return vector of ids of street with the given name
 */
std::vector<unsigned> find_street_ids_from_name(std::string street_name);

/**
 * function to return the street segments for a given intersection
 * @param intersection_id the intersection to find street segments for
 * @return a vector of street segment id's connected to this intersection
 */
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection (include duplicate street names in returned vector)
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);

/**
 * can you get from intersection1 to intersection2 using a single street segment (taking into account 1-way streets)
 * corner case: an intersection is considered to be connected to itself
 * @param intersection_id1 start intersection
 * @param intersection_id2 end intersection
 * @return true if you can get from intersection1 to intersection2 in one street segment
 */
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2);

/**
 * find all intersections reachable by traveling down one street segment
 * from given intersection (can't travel the wrong way on a 1-way street)
 * the returned vector does not contain duplicate intersections
 * returns empty vector if the intersection id is invalid
 * @param intersection_id the intersection id to start from
 * @return a vector of intersection id's that can be reached along one street from this intersection
 */
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id);

/**
 * for a given street, return all the street segments
 * @param street_id the street to find segments for
 * @return a vector of street segment ids
 */
std::vector<unsigned> find_street_street_segments(unsigned street_id);

/**
 * for a given street, find all the intersections
 * @param street_id The street to find intersections for
 * @return a vector of intersection ids
 */
std::vector<unsigned> find_all_street_intersections(unsigned street_id);

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
std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2);

/** Calculates distance between two LatLot points
 * Uses formula M1 hand out
 *
 * @param point1 		The first point
 * @param point2 		The second point
 * @return double 		Distance between both points
 */
double find_distance_between_two_points(LatLon point1, LatLon point2);

/** Calculates length of a street segment
 *
 * @param street_segment_id ID of segment to calculate
 * @return double Length of the street
 */
double calculate_street_segment_length(unsigned street_segment_id);

/**Returns length of a street segment from the MapData object
 *
 * @param street_segment_id ID of segment to get length of
 * @return double Length of the street
 */
double find_street_segment_length(unsigned street_segment_id);

/**Returns length of street
 *
 * @param street_id Street to measure
 * @return double Length of street
 */
double find_street_length(unsigned street_id);

/** Calculates travel time (minutes) of a street segment
 * Returns NULL if no segment found
 *
 * @param street_segment_id segment ID to process
 * @return double Time (minutes) to traverse segment
 */
double find_street_segment_travel_time(unsigned street_segment_id);

/**
 * Returns the closest type and index, given a vector of types and a location to query
 *
 * @param types
 * @param my_position
 * @return
 */
std::pair<DataType, int> find_closest_point_multiple_trees(std::vector<DataType> types, LatLon my_position);

/**Overloaded function
 *
 * Finds closest point to an XY point
 * @param dataType Identifier for data type
 * @param my_position Position from which to search
 * @return index of nearest point
 */

int find_closest_point(DataType dataType, PointXY my_position);

/**Overloaded function
 *
 * Finds closest point to a LatLon coordinate
 * Simply converts LatLon -> XY, then calls XY closest point finder
 * @param dataType Identifier for data type
 * @param my_position LatLon position around which to search
 * @return index of nearest point
 */
int find_closest_point(DataType dataType, LatLon my_position);

/**Overloaded function
 *
 * Searches for all points within a radius of a certain XY point
 * returns NULL if data setis invalid
 *
 * @param dataType Specify which data set to query (Intersection,
 * @param numPts RETURN (param) Number of points found
 * @param my_position Centre position around which to search
 * @param radius Distance to search
 * @return Array containing indices of found points
 */
int* find_points_within_radius(DataType dataType, int &numPts, PointXY my_position, double radius);


/**Overloaded function
 *
 * Searches for all points within a radius of a certain LatLon point
 * returns NULL if data setis invalid
 *
 * @param dataType Specify which data set to query (Intersection,
 * @param numPts RETURN (param) Number of points found
 * @param my_position Centre position around which to search
 * @param radius Distance to search
 * @return Array containing indices of found points
 */
int* find_points_within_radius(DataType dataType, int &numPts, LatLon my_position, double radius);



/**	@DEPRECATED - LEFT ONLY FOR MILESTONE 1 functionality
 * Use find_closest_point(...) instead
 *
 * Finds nearest point of interest to a given position
 *	Returns NULL if no POI's found
 *
 * @param my_position Current position
 * @return unsigned ID of nearest POI.
 */
int find_closest_point_of_interest(LatLon my_position);

/**	@DEPRECATED - LEFT ONLY FOR MILESTONE 1 FUNCTIONALITY
 * Use find_closest_point(...) instead
 *
 *  find the nearest intersection (by ID) to a given position
 *
 * @param my_position Current position
 * @return unsigned ID of closest intersection
 */
int find_closest_intersection(LatLon my_position);

/**
 * Helper function for load_map
 * Must go here rather than in mapProcessor, as it requires mapProcessor functions
 * Loads the names of all streets and POI's
 * @param map MapData object to add data to
 */
void _loadNames(MapData* map);

/**
 * Autocompletes a string, using known street and POI names
 * Returns "" if nothing found
 * @param query the incomplete query
 * @return Best guess of string
 */
std::string autocompleteStringTopPriority(std::string query);


/**
 * Autocompletes a string
 * Returns all possible words that can be made from the node, in decreasing priority
 * @param query word to autocomplete
 * @return vector of strings, sorted by importance
 */
std::vector<std::string> autocompleteAllResults(std::string query);

/**
 * Sets the parameters equal to the highest and lowest x and y values of features on the map
 * @param xLeft
 * @param xRight
 * @param yBottom
 * @param yTop
 */
/*
void findFeatureExtremes(float& xLeft, float& xRight, float& yBottom, float& yTop) {
	double curLowX = 999999999;
	double curHighX = -999999999;
	double curLowY = 999999999;
	double curHighY = -999999999;

	MapData* map = MapData::instance();
	unsigned numFeatures = map->getNumFeatures();
	std::vector<FeatureXY> features = map->getFeatures();
	std::pair<double, double> coordinates;

	double x, y;

	for (unsigned i=0; i<numFeatures; i++) {
		coordinates = features[i].getCoordinates()[0];
		if (coordinates.first < curLowX) {
			curLowX = coordinates.first;
		}
		else if (coordinates.first > curHighX) {
			curHighX = coordinates.first;
		}
		if (coordinates.second < curLowY) {
			curLowY = coordinates.second;
		}
		else if (coordinates.second > curHighY) {
			curHighY = coordinates.second;
		}
	}

	xLeft = (float) curLowX;
	xRight = (float) curHighX;
	yBottom = (float) curLowY;
	yTop = (float) curHighY;
}*/


/*
 * mapProcessorHelper.h
 *
 *  Created on: 11/02/2016
 *      Author: william
 */

/* This file contains helper functions for mapProcessor.cpp */

#ifndef MAPPROCESSORHELPER_H
#define MAPPROCESSORHELPER_H

#include <string>
#include <vector>
#include <set>
#include <boost/property_tree/ptree.hpp> // for events XML
#include <boost/property_tree/xml_parser.hpp> // for events XML
#include <boost/foreach.hpp> // for events XML
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h" // for OSM Layer 1 API
#include "MapData.h"
#include "utility.h"
#include "Trie.h"

/**
 * Helper function for load_map.
 * Generate hash table from street names -> street IDs
 * @param map The MapData object to add the data to
 */
void _generateStreetNameHashTable(MapData* map);

/**
 * Helper function for load_map.
 * Generate hash table inside each street object with keys being id's of streets
 * it connects to and values being id's of the intersection formed by
 * those two streets.
 * @param map The MapData object to add the data to
 */
void _generateStreetIntersectionHashTable(MapData* map);

/**
 * Helper function for load_map.
 * Generate 4 arrays of intersection objects corresponding to different zoom levels, each containing an array of their connected
 * street segments that are large enough to be displayed at their given size.
 * @param map The MapData object to add the data to
 */
void _generateIntersections(MapData* map);

/**
 * Helper function for load_map.
 * Generate all FeatureXY objects from the streets database and put them in MapData.
 * @param map The MapData object to add the data to.
 */
void _generateFeatureObjects(MapData* map);

/**
 * Helper function for load_map.
 * Generate all StreetSegment objects from the streets database and put them in MapData
 * and their associated Street and Intersection objects.
 * @param map The MapData object to add the data to.
 */
void _generateStreetSegmentObjects(MapData* map);



/**
 * Helper function for load_map.
 * Generate all PoiXY objects from the streets database and put them in MapData
 * @param map The MapData object to add the data to.
 */
void _generatePOIObjects(MapData* map);

/**
 * Loads events from specified XML file into Event objects in MapData.
 * @param eventFile The name of the event XML file (assumes location of mapper/Events/<eventFile>)
 * @param city The name of the city to get events for (e.g. toronto)
 * @param map The MapData instance
 */
void _loadEvents(std::string eventFile, std::string city, MapData* map);

/**
 * Calculates drawing offset and drawing scale factor to convert map coordinates (in units of m,
 * centered relative to the location on the globe) to drawing coordinates (with a range of X values
 * from -1000 to 1000 and the center of the map at 0,0). Updates all FeatureXY, PoiXY, IntersectionXY
 * to have a copy of these coordinates in addition to their existing "real" coordinates;
 * @param map The MapData instance
 */
void _drawingScaleConversion(MapData* map);

/** Calculates length of a street segment
 *
 * @param street_segment_id ID of segment to calculate
 * @return double Length of the street
 */
double _calculate_street_segment_length(unsigned street_segment_id);

#endif /* MAPPROCESSORHELPER_H */

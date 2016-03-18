/*
 * Event.cpp
 *
 *  Created on: 20/02/2016
 *      Author: william
 */

#include "Event.h"

/**
 * Main constructor
 * @param _name
 * @param _address
 * @param _latitude
 * @param _longitude
 * @param _description
 */
Event::Event(std::string _name, std::string _address, double _latitude, double _longitude, std::string _description)
	: MapElement(_name, NULL, 1, always)
{
	coordinates = new PointXY[0];
	coordinates[0].x = _latitude;
	coordinates[0].y = _longitude;
	address = _address;
	description = _description;
}

/**
 * Returns the address of the event (human-readable formatted text)
 * @return
 */
std::string Event::getAddress() {
	return address;
}

/**
 * Returns the description of the event (longer text saying what the event is)
 * @return
 */
std::string Event::getDescription() {
	return description;
}

/**
 * Whether or not the event matches at least one category in toShow, meaning it should be displayed
 * @param toShow A set of event categories, where if any one matches then the event should be shown
 * @return true if event should be displayed, false otherwise
 */
bool Event::showAfterFilter(std::set<EventType> toShow) {
	// check for overlap between the enabled categories and categories of this street (order is O(n))
	std::set<EventType> intersection;
	std::set_intersection(
			toShow.begin(), toShow.end(), // set 1
			categories.begin(), categories.end(), // set 2
			std::inserter(intersection, intersection.begin()) // to create a new set from the overlap
			);
	// return true if the sets have any overlap (intersection set not empty)
	return (!intersection.empty());
}

/**
 * Add a category that describes this event
 * @param category
 */
void Event::addCategory(EventType category) {
	categories.insert(category);
}

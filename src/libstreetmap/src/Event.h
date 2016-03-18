/*
 * Event.h
 *
 *  Created on: 20/02/2016
 *      Author: william
 */

#ifndef EVENT_H
#define EVENT_H

#include <set>
#include <algorithm>
#include "LatLon.h"
#include "MapElement.h"

// types of event for categorization/filtering
enum EventType { concert, festival, comedy, family, nightlife, sports, unknown };

class Event : public MapElement {
public:
	/**
	 * Main constructor
	 * @param _name
	 * @param _address
	 * @param _latitude
	 * @param _longitude
	 * @param _description
	 */
	Event(std::string _name, std::string _address, double _latitude, double _longitude, std::string _description);
	virtual ~Event() {}

	/**
	 * Returns the address of the event (human-readable formatted text)
	 * @return
	 */
	std::string getAddress();

	/**
	 * Returns the description of the event (longer text saying what the event is)
	 * @return
	 */
	std::string getDescription();

	/**
	 * Whether or not the event matches at least one category in toShow, meaning it should be displayed
	 * @param toShow A set of event categories, where if any one matches then the event should be shown
	 * @return true if event should be displayed, false otherwise
	 */
	bool showAfterFilter(std::set<EventType> toShow);

	/**
	 * Add a category that describes this event
	 * @param category
	 */
	void addCategory(EventType category);

private:
	// the basic information about this event
	std::string address;
	// some longer text describing what this event is
	std::string description;
	// the types of event this event fits into
	std::set<EventType> categories;
};

#endif /* EVENT_H */

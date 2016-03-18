#include <unittest++/UnitTest++.h>
#include "MapData.h"
#include "Event.h"
#include "mapProcessor.h"

TEST(event_showAfterFilter) {
	Event *test;
	std::set<EventType> filter;
	bool result;

	// tests that should return true

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(concert);
	test->addCategory(family);
	filter = { concert };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(true, result);
	delete test;

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(comedy);
	test->addCategory(festival);
	filter = { comedy, festival, sports };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(true, result);
	delete test;

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(sports);
	test->addCategory(family);
	test->addCategory(nightlife);
	filter = { concert, festival, sports };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(true, result);
	delete test;

	// tests that should fail

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(concert);
	test->addCategory(family);
	filter = { festival };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(false, result);
	delete test;

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(nightlife);
	filter = { concert, festival, comedy, family, sports};
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(false, result);
	delete test;

	// check empty sets

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	filter = { concert };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(false, result);
	delete test;

	test = new Event("", "", 0, 0, ""); // all arguments to constructor don't matter for this test
	test->addCategory(nightlife);
	filter = { };
	result = test->showAfterFilter(filter);
	CHECK_EQUAL(false, result);
	delete test;
}

/*TEST(loadEvents) {
	MapData* map = MapData::instance();
	std::set<EventType> testEventType;
	std::cout << "Loading and printing contents of toronto section of events file:" << std::endl;

	_loadEvents("toronto.events.xml", "toronto", map);

	std::vector<Event> events = *(map->getEvents());
	for (unsigned a=0; a<events.size(); a++) {
		std::cout << "Name: " << events[a].getName() << std::endl;
		std::cout << "Address: " << events[a].getAddress() << std::endl;
		std::cout << "Description: " << events[a].getDescription() << std::endl;
		std::cout << "Location: " << events[a].getCoordinates()[0][0] << ", " << events[a].getCoordinates()[0][1] << std::endl;
		std::cout << "Categories:" << std::endl;
		// check which categories are met:
		testEventType = { concert };
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tConcert" << std::endl;
		}
		testEventType = { festival };
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tFestival" << std::endl;
		}
		testEventType = { comedy};
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tComedy" << std::endl;
		}
		testEventType = { family };
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tFamily" << std::endl;
		}
		testEventType = { nightlife };
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tNightlife" << std::endl;
		}
		testEventType = { sports };
		if (events[a].showAfterFilter(testEventType)) {
			std::cout << "\tSports" << std::endl;
		}
	}
}*/


/*
 * TerminalInput.cpp
 *
 *  Created on: 14/03/2016
 *      Author: william
 */

#include "TerminalInput.h"

/**
 * Defines global variables for communicating between input and graphics
 */
namespace TerminalInterface {
	bool highlightedDirty = false;
	bool intersectionSelected = false;
	PointXY startXY;
	PointXY endXY;
	std::vector<unsigned int> highlightedSegs;
}

/**
 * Reads from terminal, instantiates UserInstructions to process the input
 */
void readFromTerminal() {
	// detect instructions
	std::string task;
	while (task != "4") {
		cout << "What do you want to do?" << endl;
		cout << " (1): Find an intersection" << endl;
		cout << " (2): Find a path from an intersection to either another intersection or a POI" << endl;
		cout << " (3): See help message" << endl;
		cout << " (4): Return to graphics window" << endl;
		cin >> task;
		if (task == "4") {
			cout << "Returning to graphics window..." << endl;
		}
		else if (task == "3") {
			showHelpMessage();
		}
		else if (task == "2") {
			cout << "Enter start intersection name (abbreviations will be autocompleted): ";
			std::string start;
			cin.clear();
			fflush(stdin);
			getline(cin, start);

			cout << "Enter destination intersection or POI name (abbreviations will be autocompleted): ";
			std::string dest;
			cin.clear();
			fflush(stdin);
			getline(cin, dest);

			UserInstruction findPath(start, dest);

			// vector of street segments defining a path
			std::vector<unsigned> path;
			IntersectionXY** intersections = MapData::instance()->getSmallIntersections();
			StreetSegment* streetSegments = MapData::instance()->getStreetSegments();
			PoiXY** pois = MapData::instance()->getPointsOfInterest();
			// draw coordinates of the start and end points of the path
			PointXY startXY = intersections[findPath.getId(0)]->getDrawCoordinates()[0];
			PointXY endXY;
			if (findPath.getInstructionType() == Path_Two_Intersection) {
				path = find_path_between_intersections(findPath.getId(0), findPath.getId(1));
				endXY = MapData::instance()->getSmallIntersections()[findPath.getId(1)]->getDrawCoordinates()[0];
			}
			else if (findPath.getInstructionType() == Path_Intersection_to_POI) {
				path = find_path_to_point_of_interest(findPath.getId(0), findPath.getPOIName());
				// find POI nearest to the end of this path
				StreetSegmentInfo lastStreetSeg = streetSegments[path[path.size() - 1]].getInfo();
				// TODO: This is not perfectly accurate, just a decent approximation
				unsigned poiID = find_closest_point(POI, intersections[lastStreetSeg.to]->getCoordinates()[0]);
				endXY = pois[poiID]->getDrawCoordinates()[0];
			}

			// send this data to graphics side by updating global variables
			TerminalInterface::startXY = startXY;
			TerminalInterface::endXY = endXY;
			TerminalInterface::highlightedSegs = path;
			TerminalInterface::highlightedDirty = true;

			// output directions to terminal
			std::vector<InstructionDisplayElement> instructions = parseStreetSegmentsToDirections(path);

			for (unsigned i=0; i<instructions.size(); i++) {
				cout << instructions[i]<< endl;
			}

			// done now
			break;
		}
		// just looking for one intersection
		else if (task == "1") {
			cout << "Enter intersection name (abbreviations will be autocompleted): ";
			std::string intersection;
			getline(cin, intersection);

			UserInstruction oneIntersection(intersection);

			unsigned intersectionID = oneIntersection.getId(0);

			// get location of this intersection
			PointXY intersectionXY = MapData::instance()->getSmallIntersections()[intersectionID]->getDrawCoordinates()[0];

			// send this data to graphics side by calling a function
			TerminalInterface::startXY = intersectionXY;
			TerminalInterface::intersectionSelected = true;

			// done now
			break;
		}
		else {
			cout << "ERROR: Invalid input! Please enter a number between 1 and 4." << endl;
		}

		//TODO: THIS CODE BELOW IS A GOOD START TOWARDS AUTOCOMPLETING THE STRING AS ITS ENTERED

		// task of 1 or 2 means first step is to read a starting intersection, so we disable console input echoing
		/*struct termios tty;
		tcgetattr(STDIN_FILENO, &tty);
		tty.c_lflag &= ~ECHO;
		(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);

		// read first input
		if (task == 1) {
			cout << "Enter the name of the intersection: ";
		}
		else {
			cout << "Enter the name of the starting intersection: "
		}

		char current = getchar();
		std::string total = "";
		std::string bestGuess;
		// whether or not bestGuess was the last string output, so we know how many characters to erase
		bool bestGuessWritten = false;
		while (current != '\n') {
			total += current;

			// remove previous output
			if (bestGuessWritten) {
				for (unsigned i=0; i<bestGuess.length(); i++) {
					cout << '\b';
				}
			}
			else {
				for (unsigned i=0; i<total.length(); i++) {
					cout << '\b';
				}
			}

			// get best guess of autocompleted string
			bestGuess = autocompleteStringTopPriority(total);

			// if too small to autocorrect, echo input
			if (total.length() <= 2 || bestGuess.length() == 0) {
				cout << total;
			}
			else {
				cout << bestGuess;
			}

			// get next character
			current = getchar();
		}*/

	}
}

/**
 * Give the user instructions on how to use the application
 */
void showHelpMessage() {
	cout << "===== INSTRUCTIONS =====" << endl;
	cout << "To use this map, press ENTER to go from the graphics window to the command line. From there you can issue commands "
			<< "to search for intersections or find paths between intersections on the map. Once you are done, you can return to "
			<< "the graphics window to see the results." << endl;
	cout << "When you're in the graphics window, you can move around by clicking and dragging, zoom with the scroll wheel, and rotate "
			<< "by holding the right mouse button and dragging." << endl;
	cout << endl;
	cout << "Press ENTER to continue..." << endl;
	std::string dummy;
	cin >> dummy;
}

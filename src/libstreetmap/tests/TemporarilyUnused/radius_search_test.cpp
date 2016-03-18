/*
 * radius_search_test.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: eddd
 */

#include <unittest++/UnitTest++.h>
#include "mapProcessor.h"
#include "MapData.h"
#include <string.h>
#include <stdio.h>
#include "unit_test_util.h"

//
struct YongeDavisvilleFixture{
	YongeDavisvilleFixture(){
		//Load LatLon of point near Yonge and Davisville
		testPt.lat = 43.697989;
		testPt.lon = -79.396009;
	}

	int numPts;
	LatLon testPt;
	double radius;
	int* points;

};

//Tests if results make sense
SUITE(intersection_accuracy_tests){

	//Checks for intersections within 10m (should be nothing)
	TEST_FIXTURE(YongeDavisvilleFixture, intersection_10){
		radius = 10;
		points = find_points_within_radius(SmallIntersections,numPts, testPt, radius);
		CHECK_EQUAL(0,numPts);
	}

	//Checks for intersections within 50m (expects) Yonge and Davisville
	TEST_FIXTURE(YongeDavisvilleFixture, intersection_50){
		radius = 50;
		points = find_points_within_radius(SmallIntersections, numPts, testPt, radius);

		CHECK_EQUAL (2, numPts);
		CHECK_EQUAL("Davisville Avenue", getIntersectionName(points[0]));
		CHECK_EQUAL("Davisville Avenue", getIntersectionName(points[1]));
	}

}

//Speed test
SUITE(radius_speed_test){
	TEST_FIXTURE(YongeDavisvilleFixture, intersectionSpeed){
		radius = 1000;
		unsigned numRuns = 500;

		ECE297_TIME_CONSTRAINT(150);
		for(unsigned i = 0; i<numRuns; i++){
			points = find_points_within_radius(SmallIntersections, numPts, testPt, radius);
			CHECK_EQUAL(551,numPts);
		}
	}
}







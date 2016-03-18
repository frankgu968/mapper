/*
 * User_Input_Test.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: eddd
 */

#include <unittest++/UnitTest++.h>
#include <string>
#include "utility.h"
#include "../src/UserInstruction.h"
#include "../src/MapData.h"

//Tests querying just a street
SUITE(Search_Street){
	//User tries to find College Street
	TEST(college){
		UserInstruction testInstr("coll");
		unsigned idx = testInstr.getId(0);

		std::string foundStreet = getStreetName(idx);
		CHECK_EQUAL("College Street", foundStreet);

		instructionTypes instr = testInstr.getInstructionType();
		CHECK_EQUAL(Find_Street, instr);
	}

	//User searches garbage data
	TEST(invalid_street){
		UserInstruction testInstr("asdfasdfasdfasdf");

		instructionTypes instr = testInstr.getInstructionType();
		CHECK_EQUAL(Invalid_Command, instr);
	}

	//User sends blank query
	TEST(blank_query){
		UserInstruction testInstr("");
		instructionTypes instr = testInstr.getInstructionType();
		CHECK_EQUAL(Invalid_Command, instr);
	}
}

//Tests querying just an intersection
SUITE(Search_Intersection){
	TEST(University_College){
		UserInstruction testInstr("Univ and Coll");
		std::string foundInter = getIntersectionName(testInstr.getId(0));
		CHECK_EQUAL("Queen's Park & University Avenue & College Street", foundInter);
		CHECK_EQUAL(Find_Intersection, testInstr.getInstructionType());
	}

	TEST(Yonge_College_Ampersand){
		UserInstruction testInstr("yon & coll");
		std::string foundInter = getIntersectionName(testInstr.getId(0));
		CHECK_EQUAL("Yonge Street & College Street & Carlton Street", foundInter);
		CHECK_EQUAL(Find_Intersection, testInstr.getInstructionType());
	}

	TEST(garbage_input){
		UserInstruction testInstr("abcd and weirrutt and college");
		CHECK_EQUAL(Invalid_Command, testInstr.getInstructionType());
	}
}

//Tests for finding a path between two intersections
SUITE(Intersections_Path){
	TEST(University_College_and_Yonge_Bloor){
		UserInstruction testInstr("uni and coll", "yon & blo");

		CHECK_EQUAL(Path_Two_Intersection, testInstr.getInstructionType());

		std::string inter1 = getIntersectionName(testInstr.getId(0));
		std::string inter2 = getIntersectionName(testInstr.getId(1));

		CHECK_EQUAL("Queen's Park & University Avenue & College Street", inter1);
		CHECK_EQUAL("Yonge Street & Bloor Street East & Bloor Street West", inter2);
	}

	TEST(Finch_Leslie_and_Yonge_Davisville){
		UserInstruction testInstr("Les & Finc", "Yon & Davi");
		CHECK_EQUAL(Path_Two_Intersection, testInstr.getInstructionType());

		std::string inter1 = getIntersectionName(testInstr.getId(0));
		std::string inter2 = getIntersectionName(testInstr.getId(1));

		CHECK_EQUAL("Finch Avenue East & Leslie Street", inter1);
		CHECK_EQUAL("Yonge Street & Davisville Avenue & Chaplin Crescent", inter2);

	}

	TEST(garbage_two_intersections){
		UserInstruction testInstr("asdfwe and dkeiv", "aiernbn & elwa");
		CHECK_EQUAL(Invalid_Command, testInstr.getInstructionType());
	}
}

//Tests for path between intersection and POI
SUITE(Intersection_POI_Path){
	TEST(Bloor_Spadina_Tims){
		UserInstruction testInstr("Blo and Spad", "Tim Hortons");
		std::string inter = getIntersectionName(testInstr.getId(0));
		std::string poi = getPointOfInterestName(testInstr.getId(1));

		//std::cout << "TESTING TIM HORTON'S SEARCH: " << poi << std::endl;

		CHECK_EQUAL(Path_Intersection_to_POI, testInstr.getInstructionType());
		CHECK_EQUAL("loor Street West & Spadina Avenue & Spadina Road", inter);
	}
}

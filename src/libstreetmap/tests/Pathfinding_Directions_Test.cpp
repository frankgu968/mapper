/*
 * Pathfinding_Directions_Test.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: eddd
 */

#include <unittest++/UnitTest++.h>
#include "../src/mapProcessor.h"
#include "../src/DisplayProcessor.h"
#include "../src/m3.h"
#include "../src/InstructionDisplayProcessor.h"
#include "../src/utility.h"

struct College_Spadina_to_Queen_Yonge{
	College_Spadina_to_Queen_Yonge(){
		//Accept user input
		UserInstruction newInstr("College and Spadina", "Queen and Yonge");
		i1 = newInstr.getId(0);
		i2 = newInstr.getId(1);

		//Query for a path
		points = findShortestPathAstar(i1,i2,bestTime);

		//Send path off for processing
		InstructionDisplayProcessor instructionProc(points);
		instructions = instructionProc.getInstructions();
	}

	double bestTime;
	unsigned i1, i2;
	std::vector<unsigned> points;
	std::vector<InstructionDisplayElement> instructions;
};

TEST_FIXTURE(College_Spadina_to_Queen_Yonge, output_directions){
	for(unsigned i = 0; i<instructions.size(); i++){
		std::cout<<instructions[i] << "\n";
	}
}

TEST(right_angle_calculation){
	std::pair <double,double> v1 = std::make_pair(1,0);
	std::pair <double,double> v2 = std::make_pair(0,1);

	double angle1 = getAngleTwoVectors(v1,v2);
	CHECK_EQUAL(-90,angle1);

	double angle2 = getAngleTwoVectors(v2,v1);
	CHECK_EQUAL(90, angle2);
}

TEST(45_angle_calculation){
	std::pair <double,double> v1 = std::make_pair(1,1);
	std::pair <double,double> v2 = std::make_pair(1,0);

	double angle1 = getAngleTwoVectors(v1,v2);
	CHECK_EQUAL(45,angle1);

	double angle2 = getAngleTwoVectors(v2,v1);
	CHECK_EQUAL(-45, angle2);
}




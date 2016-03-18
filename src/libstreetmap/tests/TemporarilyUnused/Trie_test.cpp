/*
 * Trie_test.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: eddd
 */

#include <unittest++/UnitTest++.h>
#include "../src/Trie.h"
#include <stdio.h>
#include <string>
#include "../src/mapProcessor.h"


struct SearchTest{
	SearchTest(){
		myTree.insertWord("Unicorn", 3);
		myTree.insertWord("Unicornorn", 5);
		myTree.insertWord("Uni", 2);
		myTree.insertWord("Hulo World", 4);
	}
	Trie myTree;
};

//A very simple test to very Trie class works, regardless of map
SUITE(simple_accuracy_tests){
	//Send a search query that's too short - autocomplete should return exactly what was inputted
	TEST_FIXTURE(SearchTest, word_too_short){
		myTree.insertWord("Unico",3);
		std::string result = myTree.queryTopWord("U");
		CHECK_EQUAL("U", result);
	}

	//A basic auto complete search
	TEST_FIXTURE(SearchTest, basic_search){
		std::string result = myTree.queryTopWord("Hulo");
		CHECK_EQUAL("Hulo World", result);
	}

	//A basic auto complete all search
	TEST_FIXTURE(SearchTest, basic_all_search){
		std::vector<std::string> result = myTree.queryAllWords("Uni");

		CHECK_EQUAL(3, result.size());
		CHECK_EQUAL("Unicornorn", result[0]);
		CHECK_EQUAL("Unicorn", result[1]);
		CHECK_EQUAL("Uni", result[2]);
	}

	//Send a query that's already a known word - should return word back
	TEST_FIXTURE(SearchTest, word_matches_query){
		std::string result = myTree.queryTopWord("Unicorn");
		CHECK_EQUAL("Unicorn", result);
	}

	//Ensure highest priority word is returned
	//"Unicorn" will be found before "Unicornorn", but "Unicornorn" was inputted with higher priority
	TEST_FIXTURE(SearchTest, priority_check){
		std::string result = myTree.queryTopWord("Unic");
		CHECK_EQUAL("Unicornorn", result);
	}
}

//Simple error checks - check for strange input
SUITE(simple_error_checks){
	//Trie should store any possible char
	TEST_FIXTURE(SearchTest, strange_characters){
		std::string funnyHello = "h!e%l&l(@o w?or#ld[}/!";
		myTree.insertWord(funnyHello, 100);
		std::string result = myTree.queryTopWord("h!e%");
		CHECK_EQUAL(funnyHello, result);
	}

	TEST_FIXTURE(SearchTest, nonexistent_query){
		std::string result = myTree.queryTopWord("asdfghk");
		CHECK_EQUAL("", result);
	}

	TEST_FIXTURE(SearchTest, empty_query){
		std::string result = myTree.queryTopWord("");
		CHECK_EQUAL("", result);
	}
}

SUITE(toronto_tests){

	//Search College Street
	TEST(college){
		std::string result = autocompleteStringTopPriority("Coll");
		CHECK_EQUAL("College Street", result);
	}

	//Search Tims
	TEST(tims){
		std::string result = autocompleteStringTopPriority("Tim ");
		CHECK_EQUAL("Tim Horton's Drive Thru Lane", result);

		result = autocompleteStringTopPriority("Tim Horton's");
		CHECK_EQUAL ("Tim Horton's", result);
	}

	//Search St. George
	TEST(st_george){
		std::string result = autocompleteStringTopPriority("St. G");
		CHECK_EQUAL("St. George Street", result);
	}

	//Search St. Clair
	TEST(st_clair){
		std::string result = autocompleteStringTopPriority("St.");
		CHECK_EQUAL("St. Clair Avenue East", result);
	}

	//Search John Anderson Restaurant
	TEST(john_anderson){
		std::string result = autocompleteStringTopPriority("John and");
		CHECK_EQUAL("John Anderson Restaurant", result);
	}

	//Test garbage input
	TEST(garbage){
		std::vector<std::string> result = autocompleteAllResults("asdfasdfasdfasdf");
		CHECK_EQUAL(0, result.size());
	}

	//Test all Finch's
	TEST(all_finch){
		std::vector<std::string> result = autocompleteAllResults("finch");

		CHECK_EQUAL(15, result.size());
		CHECK_EQUAL("Finch Avenue West", result[0]);
		CHECK_EQUAL("Finch Avenue East", result[1]);
		CHECK_EQUAL("Finch Avenue", result[2]);
		CHECK_EQUAL("Fincham Avenue", result[3]);
		CHECK_EQUAL("Finchdene Square", result[4]);
		CHECK_EQUAL("Finchley Circle", result[5]);
		CHECK_EQUAL("Finch Bus Terminal", result[6]);
		CHECK_EQUAL("Finchgate Boulevard", result[7]);
		CHECK_EQUAL("Finch Passenger Pick-Up/Drop-Off", result[8]);
	}

	//Test all Tim
	TEST(all_tims){
		std::vector<std::string> result = autocompleteAllResults("tim h");

		for(int unsigned i = 0; i<result.size(); i++){
			//std::cout<<result[i] << "\n";
		}
	}
}

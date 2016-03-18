#include <unittest++/UnitTest++.h>
#include <algorithm>
#include <random>
#include "utility.h"

TEST(findkth) {
	std::vector<double> inputArray;
	double expected, actual;
	unsigned index;

	// for generating random doubles
	std::uniform_real_distribution<double> unif(-9999,9999);
    std::default_random_engine re;

    std::cout << "Running findkth func tests" << std::endl;

	// generate 100 random arrays, check if the value at the index is the same
	// as the k'th value of the sorted array
	for (unsigned i=0; i<100; i++) {
		// add 3000 random numbers to vector
		for (unsigned j=0; j<3000; j++) {
			inputArray.push_back(unif(re));
		}
		// find 1500th value
		index = findkth(inputArray, 1500);
		actual = inputArray[index];

		// check against actual 15th value
		std::sort(inputArray.begin(), inputArray.end());
		expected = inputArray[1500];
		CHECK_EQUAL(expected, actual);

		inputArray.clear();
	}
}

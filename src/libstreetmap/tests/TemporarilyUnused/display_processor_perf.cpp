#include <unittest++/UnitTest++.h>
#include <algorithm>
#include <random>
#include "unit_test_util.h"
#include "utility.h"

#include "mapProcessor.h"

/*TEST(polygonToTriangle_perf) {
	// for generating random doubles
	std::uniform_real_distribution<double> unif(-3,3);
	std::default_random_engine re;
	std::vector<TriangleXY> triangles;

	std::cout << "=== POLYGON TO TRIANGLE ===" << std::endl;

	// speed test triangulating 100 random polygons
	{
		ECE297_TIME_CONSTRAINT(1);
		for (unsigned i=0; i<100; i+=10) {
			std::cout << "Testing polygon with " << i << " points" << std::endl;
			std::vector<std::pair<double,double>> points;
			for (unsigned j=0; j<=i; j++) {
				points.push_back(std::make_pair(unif(re), unif(re)));
			}
			triangles = polygonToTriangles(points);
		}
	}
}*/

TEST(checkPointInsideFeature_perf) {
	MapData* map = MapData::instance();
	unsigned numFeatures = map->getNumLargeFeatures();
	FeatureXY** features = map->getLargeFeatures();

	ECE297_TIME_CONSTRAINT(80);
	for (unsigned j=0; j<100; j++) {
		for (unsigned i=0; i<numFeatures; i++) {
			if (features[i]->getDrawPriority() == always) {
				map->checkPointInsideFeature(i, 1.34, -2.4, always);
			}
		}
	}
}

TEST(findkth_perf) {
	std::vector<double> inputArray;
	unsigned dummy;

	// for generating random doubles
	std::uniform_real_distribution<double> unif(-9999,9999);
    std::default_random_engine re;

    std::cout << "Running findkth speed tests" << std::endl;


	// add 60 random numbers to vector
	for (unsigned j=0; j<60; j++) {
		inputArray.push_back(unif(re));
	}
	// find kth value, test speed
	{
		ECE297_TIME_CONSTRAINT(10);
		for (unsigned k=0; k<60; k++) {
			dummy = findkth(inputArray, k);
		}
	}
}

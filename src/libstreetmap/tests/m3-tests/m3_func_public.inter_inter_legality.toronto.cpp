#include <random>
#include <unittest++/UnitTest++.h>
#include "StreetsDatabaseAPI.h"
#include "mapProcessor.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;
struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(0);
        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections()-1);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<float>(43.48, 43.91998);
        rand_lon = std::uniform_real_distribution<float>(-79.78998, -79.00001);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<float> rand_lat;
    std::uniform_real_distribution<float> rand_lon;
};

SUITE(inter_inter_path_func_public) {
    TEST(find_path_between_intersections_legality) {
        std::vector<unsigned> path;
        
		ECE297_TIME_CONSTRAINT(1);
		path = find_path_between_intersections(62818, 17844);
		CHECK(path_is_legal(62818, 17844, path));

        path = find_path_between_intersections(2, 9256);
        CHECK(path_is_legal(2, 9256, path));

        path = find_path_between_intersections(17930, 93400);
        CHECK(path_is_legal(17930, 93400, path));
        
        path = find_path_between_intersections(24341, 84950);
        CHECK(path_is_legal(24341, 84950, path));
        
        path = find_path_between_intersections(28619, 80938);
        CHECK(path_is_legal(28619, 80938, path));
        
        path = find_path_between_intersections(32342, 46379);
        CHECK(path_is_legal(32342, 46379, path));
        
        path = find_path_between_intersections(33578, 56051);
        CHECK(path_is_legal(33578, 56051, path));
        
        path = find_path_between_intersections(43046, 85976);
        CHECK(path_is_legal(43046, 85976, path));
        
        path = find_path_between_intersections(47772, 44848);
        CHECK(path_is_legal(47772, 44848, path));
        
        path = find_path_between_intersections(53600, 84132);
        CHECK(path_is_legal(53600, 84132, path));
        
        path = find_path_between_intersections(56060, 43327);
        CHECK(path_is_legal(56060, 43327, path));
        
        path = find_path_between_intersections(62818, 17844);
        CHECK(path_is_legal(62818, 17844, path));
        
        path = find_path_between_intersections(63381, 88129);
        CHECK(path_is_legal(63381, 88129, path));
        
        path = find_path_between_intersections(64436, 55705);
        CHECK(path_is_legal(64436, 55705, path));
        
        path = find_path_between_intersections(65267, 91978);
        CHECK(path_is_legal(65267, 91978, path));
        
        path = find_path_between_intersections(65463, 97060);
        CHECK(path_is_legal(65463, 97060, path));
        
        path = find_path_between_intersections(66348, 47055);
        CHECK(path_is_legal(66348, 47055, path));
        
        path = find_path_between_intersections(67133, 63045);
        CHECK(path_is_legal(67133, 63045, path));
        
        path = find_path_between_intersections(79055, 105225);
        CHECK(path_is_legal(79055, 105225, path));
        
        path = find_path_between_intersections(80481, 34196);
        CHECK(path_is_legal(80481, 34196, path));
        
        path = find_path_between_intersections(64489, 10192);
        CHECK(path_is_legal(64489, 10192, path));
        
        path = find_path_between_intersections(95430, 108325);
        CHECK(path_is_legal(95430, 108325, path));
        
        path = find_path_between_intersections(9140, 19741);
        CHECK(path_is_legal(9140, 19741, path));
        
        path = find_path_between_intersections(97919, 71085);
        CHECK(path_is_legal(97919, 71085, path));
        
        path = find_path_between_intersections(98141, 104672);
        CHECK(path_is_legal(98141, 104672, path));
        
        path = find_path_between_intersections(98719, 32007);
        CHECK(path_is_legal(98719, 32007, path));
        
        path = find_path_between_intersections(98941, 108022);
        CHECK(path_is_legal(98941, 108022, path));
        
        path = find_path_between_intersections(9748, 61004);
        CHECK(path_is_legal(9748, 61004, path));
        
        path = find_path_between_intersections(101919, 45138);
        CHECK(path_is_legal(101919, 45138, path));
        
        path = find_path_between_intersections(105371, 20649);
        CHECK(path_is_legal(105371, 20649, path));
        
        path = find_path_between_intersections(7801, 20885);
        CHECK(path_is_legal(7801, 20885, path));
        
    } //find_path_between_intersections_legality

    TEST(find_path_between_intersections_optimality) {
        std::vector<unsigned> path;
        path = find_path_between_intersections(2, 9256);
        CHECK(path_is_legal(2, 9256, path));
        double travelTime = compute_path_travel_time(path);
        std::cout << "travel time = " << travelTime << std::endl;
        CHECK(travelTime <= 16.97107155614322);
        
        path = find_path_between_intersections(17930, 93400);
        CHECK(path_is_legal(17930, 93400, path));
        travelTime = compute_path_travel_time(path);
        std::cout << "travel time = " << travelTime << std::endl;
        CHECK(travelTime <= 18.60141603252727);

        path = find_path_between_intersections(24341, 84950);
        CHECK(path_is_legal(24341, 84950, path));
        CHECK(compute_path_travel_time(path) <= 14.42634934251768);
        
        path = find_path_between_intersections(28619, 80938);
        CHECK(path_is_legal(28619, 80938, path));
        CHECK(compute_path_travel_time(path) <= 34.93224887390062);
        
        path = find_path_between_intersections(32342, 46379);
        CHECK(path_is_legal(32342, 46379, path));
        CHECK(compute_path_travel_time(path) <= 44.34490195154542);
        
        path = find_path_between_intersections(33578, 56051);
        CHECK(path_is_legal(33578, 56051, path));
        CHECK(compute_path_travel_time(path) <= 38.46870425901721);
        
        path = find_path_between_intersections(43046, 85976);
        CHECK(path_is_legal(43046, 85976, path));
        CHECK(compute_path_travel_time(path) <= 42.34206636523066);
        
        path = find_path_between_intersections(47772, 44848);
        CHECK(path_is_legal(47772, 44848, path));
        CHECK(compute_path_travel_time(path) <= 27.52103760108951);
        
        path = find_path_between_intersections(53600, 84132);
        CHECK(path_is_legal(53600, 84132, path));
        CHECK(compute_path_travel_time(path) <= 21.86841541006605);
        
        path = find_path_between_intersections(56060, 43327);
        CHECK(path_is_legal(56060, 43327, path));
        CHECK(compute_path_travel_time(path) <= 45.23658886267624);
        
        path = find_path_between_intersections(62818, 17844);
        CHECK(path_is_legal(62818, 17844, path));
        CHECK(compute_path_travel_time(path) <= 5.975310074617227);
        
        path = find_path_between_intersections(63381, 88129);
        CHECK(path_is_legal(63381, 88129, path));
        CHECK(compute_path_travel_time(path) <= 20.03380477663629);
        
        path = find_path_between_intersections(64436, 55705);
        CHECK(path_is_legal(64436, 55705, path));
        CHECK(compute_path_travel_time(path) <= 28.14439495235417);
        
        path = find_path_between_intersections(65267, 91978);
        CHECK(path_is_legal(65267, 91978, path));
        CHECK(compute_path_travel_time(path) <= 18.53861473816822);
        
        path = find_path_between_intersections(65463, 97060);
        CHECK(path_is_legal(65463, 97060, path));
        CHECK(compute_path_travel_time(path) <= 36.23090190715419);
        
        path = find_path_between_intersections(66348, 47055);
        CHECK(path_is_legal(66348, 47055, path));
        CHECK(compute_path_travel_time(path) <= 36.00071271261419);
        
        path = find_path_between_intersections(67133, 63045);
        CHECK(path_is_legal(67133, 63045, path));
        CHECK(compute_path_travel_time(path) <= 48.89588246836037);
        
        path = find_path_between_intersections(79055, 105225);
        CHECK(path_is_legal(79055, 105225, path));
        CHECK(compute_path_travel_time(path) <= 32.61748952451359);
        
        path = find_path_between_intersections(80481, 34196);
        CHECK(path_is_legal(80481, 34196, path));
        CHECK(compute_path_travel_time(path) <= 31.22172067427089);
        
        path = find_path_between_intersections(64489, 10192);
        CHECK(path_is_legal(64489, 10192, path));
        CHECK(compute_path_travel_time(path) <= 44.78823700347708);
        
        path = find_path_between_intersections(95430, 108325);
        CHECK(path_is_legal(95430, 108325, path));
        CHECK(compute_path_travel_time(path) <= 18.90825258302869);
        
        path = find_path_between_intersections(9140, 19741);
        CHECK(path_is_legal(9140, 19741, path));
        CHECK(compute_path_travel_time(path) <= 8.533098894535689);
        
        path = find_path_between_intersections(97919, 71085);
        CHECK(path_is_legal(97919, 71085, path));
        CHECK(compute_path_travel_time(path) <= 19.09424675995584);
        
        path = find_path_between_intersections(98141, 104672);
        CHECK(path_is_legal(98141, 104672, path));
        CHECK(compute_path_travel_time(path) <= 8.374395601146057);
        
        path = find_path_between_intersections(98719, 32007);
        CHECK(path_is_legal(98719, 32007, path));
        CHECK(compute_path_travel_time(path) <= 16.83079257668522);
        
        path = find_path_between_intersections(98941, 108022);
        CHECK(path_is_legal(98941, 108022, path));
        CHECK(compute_path_travel_time(path) <= 25.30575757016142);
        
        path = find_path_between_intersections(9748, 61004);
        CHECK(path_is_legal(9748, 61004, path));
        CHECK(compute_path_travel_time(path) <= 20.01393594386155);
        
        path = find_path_between_intersections(101919, 45138);
        CHECK(path_is_legal(101919, 45138, path));
        CHECK(compute_path_travel_time(path) <= 33.28684118995628);
        
        path = find_path_between_intersections(105371, 20649);
        CHECK(path_is_legal(105371, 20649, path));
        CHECK(compute_path_travel_time(path) <= 11.25282465218332);
        
        path = find_path_between_intersections(7801, 20885);
        CHECK(path_is_legal(7801, 20885, path));
        CHECK(compute_path_travel_time(path) <= 36.53998218118426);
        
    } //find_path_between_intersections_optimality

} //inter_inter_path_func_public


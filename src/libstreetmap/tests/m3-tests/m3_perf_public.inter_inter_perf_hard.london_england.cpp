#include <random>
#include <unittest++/UnitTest++.h>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
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
        rand_lat = std::uniform_real_distribution<float>(50.94104, 51.984);
        rand_lon = std::uniform_real_distribution<float>(-1.114989, 0.8949982);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<float> rand_lat;
    std::uniform_real_distribution<float> rand_lon;
};

SUITE(inter_inter_path_perf_hard_public) {
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_hard) {
        //Verify Functionality
        std::vector<unsigned> path;
        path = find_path_between_intersections(7452, 59356);
        CHECK(path_is_legal(7452, 59356, path));
        CHECK(compute_path_travel_time(path) <= 62.54544456358283);
        
        path = find_path_between_intersections(45804, 83915);
        CHECK(path_is_legal(45804, 83915, path));
        CHECK(compute_path_travel_time(path) <= 31.40849178231873);
        
        path = find_path_between_intersections(76316, 143595);
        CHECK(path_is_legal(76316, 143595, path));
        CHECK(compute_path_travel_time(path) <= 20.67060860026884);
        
        path = find_path_between_intersections(82195, 400788);
        CHECK(path_is_legal(82195, 400788, path));
        CHECK(compute_path_travel_time(path) <= 80.29526901772239);
        
        path = find_path_between_intersections(107580, 192369);
        CHECK(path_is_legal(107580, 192369, path));
        CHECK(compute_path_travel_time(path) <= 15.25983968418343);
        
        path = find_path_between_intersections(108532, 206272);
        CHECK(path_is_legal(108532, 206272, path));
        CHECK(compute_path_travel_time(path) <= 40.70767540517902);
        
        path = find_path_between_intersections(110064, 203263);
        CHECK(path_is_legal(110064, 203263, path));
        CHECK(compute_path_travel_time(path) <= 44.36330583119448);
        
        path = find_path_between_intersections(113141, 434912);
        CHECK(path_is_legal(113141, 434912, path));
        CHECK(compute_path_travel_time(path) <= 58.73116964692449);
        
        path = find_path_between_intersections(130724, 305741);
        CHECK(path_is_legal(130724, 305741, path));
        CHECK(compute_path_travel_time(path) <= 80.09762996059688);
        
        path = find_path_between_intersections(131581, 111893);
        CHECK(path_is_legal(131581, 111893, path));
        CHECK(compute_path_travel_time(path) <= 56.39418345957549);
        
        path = find_path_between_intersections(139923, 247994);
        CHECK(path_is_legal(139923, 247994, path));
        CHECK(compute_path_travel_time(path) <= 31.56682192677339);
        
        path = find_path_between_intersections(170276, 80188);
        CHECK(path_is_legal(170276, 80188, path));
        CHECK(compute_path_travel_time(path) <= 88.12858485627466);
        
        path = find_path_between_intersections(190154, 61822);
        CHECK(path_is_legal(190154, 61822, path));
        CHECK(compute_path_travel_time(path) <= 27.94576741103604);
        
        path = find_path_between_intersections(132269, 68211);
        CHECK(path_is_legal(132269, 68211, path));
        CHECK(compute_path_travel_time(path) <= 23.20272784403407);
        
        path = find_path_between_intersections(209487, 30357);
        CHECK(path_is_legal(209487, 30357, path));
        CHECK(compute_path_travel_time(path) <= 63.67178849797453);
        
        path = find_path_between_intersections(215219, 170474);
        CHECK(path_is_legal(215219, 170474, path));
        CHECK(compute_path_travel_time(path) <= 38.07393406315788);
        
        path = find_path_between_intersections(146649, 292869);
        CHECK(path_is_legal(146649, 292869, path));
        CHECK(compute_path_travel_time(path) <= 73.68346780962129);
        
        path = find_path_between_intersections(239093, 112668);
        CHECK(path_is_legal(239093, 112668, path));
        CHECK(compute_path_travel_time(path) <= 71.18436193686259);
        
        path = find_path_between_intersections(291106, 172642);
        CHECK(path_is_legal(291106, 172642, path));
        CHECK(compute_path_travel_time(path) <= 46.56440134085701);
        
        path = find_path_between_intersections(291638, 15967);
        CHECK(path_is_legal(291638, 15967, path));
        CHECK(compute_path_travel_time(path) <= 46.86157891488039);
        
        path = find_path_between_intersections(309150, 3564);
        CHECK(path_is_legal(309150, 3564, path));
        CHECK(compute_path_travel_time(path) <= 48.36307131374199);
        
        path = find_path_between_intersections(324969, 77004);
        CHECK(path_is_legal(324969, 77004, path));
        CHECK(compute_path_travel_time(path) <= 66.69702495249626);
        
        path = find_path_between_intersections(358371, 85979);
        CHECK(path_is_legal(358371, 85979, path));
        CHECK(compute_path_travel_time(path) <= 53.36839516316481);
        
        path = find_path_between_intersections(397517, 350840);
        CHECK(path_is_legal(397517, 350840, path));
        CHECK(compute_path_travel_time(path) <= 87.28687613842455);
        
        path = find_path_between_intersections(398678, 370756);
        CHECK(path_is_legal(398678, 370756, path));
        CHECK(compute_path_travel_time(path) <= 24.59091434015031);
        
        path = find_path_between_intersections(377157, 270862);
        CHECK(path_is_legal(377157, 270862, path));
        CHECK(compute_path_travel_time(path) <= 24.3759564174199);
        
        path = find_path_between_intersections(321658, 395386);
        CHECK(path_is_legal(321658, 395386, path));
        CHECK(compute_path_travel_time(path) <= 77.43958575394815);
        
        path = find_path_between_intersections(430248, 330154);
        CHECK(path_is_legal(430248, 330154, path));
        CHECK(compute_path_travel_time(path) <= 28.99166410844632);
        
        path = find_path_between_intersections(226218, 363416);
        CHECK(path_is_legal(226218, 363416, path));
        CHECK(compute_path_travel_time(path) <= 74.44022332381819);
        
        path = find_path_between_intersections(336095, 267144);
        CHECK(path_is_legal(336095, 267144, path));
        CHECK(compute_path_travel_time(path) <= 15.64803109866175);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids1;
        std::vector<unsigned> intersection_ids2;
        for(size_t i = 0; i < 100; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            //ECE297_TIME_CONSTRAINT(16368);
        	ECE297_TIME_CONSTRAINT(1);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 100; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i]);
            }
        }
    } //find_path_between_intersections_perf_hard

} //inter_inter_path_perf_hard_public


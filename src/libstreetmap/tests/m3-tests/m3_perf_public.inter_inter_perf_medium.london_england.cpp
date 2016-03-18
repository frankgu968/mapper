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

SUITE(inter_inter_path_perf_medium_public) {
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_medium) {
        //Verify Functionality
        std::vector<unsigned> path;
        path = find_path_between_intersections(14965, 21644);
        CHECK(path_is_legal(14965, 21644, path));
        CHECK(compute_path_travel_time(path) <= 38.61818528639427);
        
        path = find_path_between_intersections(30028, 355126);
        CHECK(path_is_legal(30028, 355126, path));
        CHECK(compute_path_travel_time(path) <= 44.10835735993478);
        
        path = find_path_between_intersections(78221, 228140);
        CHECK(path_is_legal(78221, 228140, path));
        CHECK(compute_path_travel_time(path) <= 55.7833066314223);
        
        path = find_path_between_intersections(82195, 400788);
        CHECK(path_is_legal(82195, 400788, path));
        CHECK(compute_path_travel_time(path) <= 80.29526901772239);
        
        path = find_path_between_intersections(88883, 321635);
        CHECK(path_is_legal(88883, 321635, path));
        CHECK(compute_path_travel_time(path) <= 60.27091479941468);
        
        path = find_path_between_intersections(134454, 98809);
        CHECK(path_is_legal(134454, 98809, path));
        CHECK(compute_path_travel_time(path) <= 13.8507363490141);
        
        path = find_path_between_intersections(139923, 247994);
        CHECK(path_is_legal(139923, 247994, path));
        CHECK(compute_path_travel_time(path) <= 31.56682192677339);
        
        path = find_path_between_intersections(158399, 417252);
        CHECK(path_is_legal(158399, 417252, path));
        CHECK(compute_path_travel_time(path) <= 50.9108085587625);
        
        path = find_path_between_intersections(190923, 430464);
        CHECK(path_is_legal(190923, 430464, path));
        CHECK(compute_path_travel_time(path) <= 75.24077532946312);
        
        path = find_path_between_intersections(241639, 45811);
        CHECK(path_is_legal(241639, 45811, path));
        CHECK(compute_path_travel_time(path) <= 28.07172405577372);
        
        path = find_path_between_intersections(268562, 132801);
        CHECK(path_is_legal(268562, 132801, path));
        CHECK(compute_path_travel_time(path) <= 74.54327824907031);
        
        path = find_path_between_intersections(268569, 22342);
        CHECK(path_is_legal(268569, 22342, path));
        CHECK(compute_path_travel_time(path) <= 54.94627450806831);
        
        path = find_path_between_intersections(291638, 15967);
        CHECK(path_is_legal(291638, 15967, path));
        CHECK(compute_path_travel_time(path) <= 46.86157891488039);
        
        path = find_path_between_intersections(309467, 427859);
        CHECK(path_is_legal(309467, 427859, path));
        CHECK(compute_path_travel_time(path) <= 69.18224286322227);
        
        path = find_path_between_intersections(324969, 77004);
        CHECK(path_is_legal(324969, 77004, path));
        CHECK(compute_path_travel_time(path) <= 66.69702495249626);
        
        path = find_path_between_intersections(192566, 138981);
        CHECK(path_is_legal(192566, 138981, path));
        CHECK(compute_path_travel_time(path) <= 53.53275581573963);
        
        path = find_path_between_intersections(357276, 177806);
        CHECK(path_is_legal(357276, 177806, path));
        CHECK(compute_path_travel_time(path) <= 75.46066641673242);
        
        path = find_path_between_intersections(365443, 144381);
        CHECK(path_is_legal(365443, 144381, path));
        CHECK(compute_path_travel_time(path) <= 76.69894018423101);
        
        path = find_path_between_intersections(368243, 104925);
        CHECK(path_is_legal(368243, 104925, path));
        CHECK(compute_path_travel_time(path) <= 37.79042679220409);
        
        path = find_path_between_intersections(384888, 250512);
        CHECK(path_is_legal(384888, 250512, path));
        CHECK(compute_path_travel_time(path) <= 71.44882143671038);
        
        path = find_path_between_intersections(401426, 24593);
        CHECK(path_is_legal(401426, 24593, path));
        CHECK(compute_path_travel_time(path) <= 99.04289235368519);
        
        path = find_path_between_intersections(415272, 233999);
        CHECK(path_is_legal(415272, 233999, path));
        CHECK(compute_path_travel_time(path) <= 91.0109750436952);
        
        path = find_path_between_intersections(377157, 270862);
        CHECK(path_is_legal(377157, 270862, path));
        CHECK(compute_path_travel_time(path) <= 24.3759564174199);
        
        path = find_path_between_intersections(321658, 395386);
        CHECK(path_is_legal(321658, 395386, path));
        CHECK(compute_path_travel_time(path) <= 77.43958575394815);
        
        path = find_path_between_intersections(97228, 196267);
        CHECK(path_is_legal(97228, 196267, path));
        CHECK(compute_path_travel_time(path) <= 62.997217092971);
        
        path = find_path_between_intersections(103268, 350877);
        CHECK(path_is_legal(103268, 350877, path));
        CHECK(compute_path_travel_time(path) <= 49.86954298467268);
        
        path = find_path_between_intersections(429580, 26622);
        CHECK(path_is_legal(429580, 26622, path));
        CHECK(compute_path_travel_time(path) <= 44.46262549796924);
        
        path = find_path_between_intersections(430248, 330154);
        CHECK(path_is_legal(430248, 330154, path));
        CHECK(compute_path_travel_time(path) <= 28.99166410844632);
        
        path = find_path_between_intersections(52129, 428440);
        CHECK(path_is_legal(52129, 428440, path));
        CHECK(compute_path_travel_time(path) <= 78.94919054417522);
        
        path = find_path_between_intersections(226218, 363416);
        CHECK(path_is_legal(226218, 363416, path));
        CHECK(compute_path_travel_time(path) <= 74.44022332381819);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids1;
        std::vector<unsigned> intersection_ids2;
        for(size_t i = 0; i < 40; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(16948);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 40; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i]);
            }
        }
    } //find_path_between_intersections_perf_medium

} //inter_inter_path_perf_medium_public


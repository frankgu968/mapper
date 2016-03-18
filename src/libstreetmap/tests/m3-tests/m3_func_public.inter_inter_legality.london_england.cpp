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

SUITE(inter_inter_path_func_public) {
    TEST(find_path_between_intersections_legality) {
        std::vector<unsigned> path;
        
        path = find_path_between_intersections(9, 37205);
        CHECK(path_is_legal(9, 37205, path));
        
        path = find_path_between_intersections(36739, 79346);
        CHECK(path_is_legal(36739, 79346, path));
        
        path = find_path_between_intersections(39181, 245197);
        CHECK(path_is_legal(39181, 245197, path));
        
        path = find_path_between_intersections(72069, 375410);
        CHECK(path_is_legal(72069, 375410, path));
        
        path = find_path_between_intersections(115033, 325322);
        CHECK(path_is_legal(115033, 325322, path));
        
        path = find_path_between_intersections(129996, 186417);
        CHECK(path_is_legal(129996, 186417, path));
        
        path = find_path_between_intersections(134964, 225290);
        CHECK(path_is_legal(134964, 225290, path));
        
        path = find_path_between_intersections(173019, 345568);
        CHECK(path_is_legal(173019, 345568, path));
        
        path = find_path_between_intersections(192014, 180262);
        CHECK(path_is_legal(192014, 180262, path));
        
        path = find_path_between_intersections(225326, 174147);
        CHECK(path_is_legal(225326, 174147, path));
        
        path = find_path_between_intersections(254753, 354223);
        CHECK(path_is_legal(254753, 354223, path));
        
        path = find_path_between_intersections(262331, 369695);
        CHECK(path_is_legal(262331, 369695, path));
        
        path = find_path_between_intersections(263120, 390122);
        CHECK(path_is_legal(263120, 390122, path));
        
        path = find_path_between_intersections(266678, 189134);
        CHECK(path_is_legal(266678, 189134, path));
        
        path = find_path_between_intersections(317752, 422938);
        CHECK(path_is_legal(317752, 422938, path));
        
        path = find_path_between_intersections(323484, 137447);
        CHECK(path_is_legal(323484, 137447, path));
        
        path = find_path_between_intersections(259206, 40967);
        CHECK(path_is_legal(259206, 40967, path));
        
        path = find_path_between_intersections(383569, 435396);
        CHECK(path_is_legal(383569, 435396, path));
        
        path = find_path_between_intersections(269834, 253401);
        CHECK(path_is_legal(269834, 253401, path));
        
        path = find_path_between_intersections(393573, 285718);
        CHECK(path_is_legal(393573, 285718, path));
        
        path = find_path_between_intersections(394464, 420716);
        CHECK(path_is_legal(394464, 420716, path));
        
        path = find_path_between_intersections(396787, 128650);
        CHECK(path_is_legal(396787, 128650, path));
        
        path = find_path_between_intersections(397682, 434180);
        CHECK(path_is_legal(397682, 434180, path));
        
        path = find_path_between_intersections(252490, 71723);
        CHECK(path_is_legal(252490, 71723, path));
        
        path = find_path_between_intersections(409651, 181426);
        CHECK(path_is_legal(409651, 181426, path));
        
        path = find_path_between_intersections(31355, 83947);
        CHECK(path_is_legal(31355, 83947, path));
        
        path = find_path_between_intersections(258992, 223898);
        CHECK(path_is_legal(258992, 223898, path));
        
        path = find_path_between_intersections(423526, 82998);
        CHECK(path_is_legal(423526, 82998, path));
        
        path = find_path_between_intersections(215440, 338157);
        CHECK(path_is_legal(215440, 338157, path));
        
        path = find_path_between_intersections(301532, 238145);
        CHECK(path_is_legal(301532, 238145, path));
        
    } //find_path_between_intersections_legality

    TEST(find_path_between_intersections_optimality) {
        std::vector<unsigned> path;
        path = find_path_between_intersections(9, 37205);
        CHECK(path_is_legal(9, 37205, path));
        CHECK(compute_path_travel_time(path) <= 14.58041111816543);
        
        path = find_path_between_intersections(36739, 79346);
        CHECK(path_is_legal(36739, 79346, path));
        CHECK(compute_path_travel_time(path) <= 42.61074411011153);
        
        path = find_path_between_intersections(39181, 245197);
        CHECK(path_is_legal(39181, 245197, path));
        CHECK(compute_path_travel_time(path) <= 47.14571032640144);
        
        path = find_path_between_intersections(72069, 375410);
        CHECK(path_is_legal(72069, 375410, path));
        CHECK(compute_path_travel_time(path) <= 42.29149425438123);
        
        path = find_path_between_intersections(115033, 325322);
        CHECK(path_is_legal(115033, 325322, path));
        CHECK(compute_path_travel_time(path) <= 32.30564367164832);
        
        path = find_path_between_intersections(129996, 186417);
        CHECK(path_is_legal(129996, 186417, path));
        CHECK(compute_path_travel_time(path) <= 50.45756308878594);
        
        path = find_path_between_intersections(134964, 225290);
        CHECK(path_is_legal(134964, 225290, path));
        CHECK(compute_path_travel_time(path) <= 18.14788625031177);
        
        path = find_path_between_intersections(173019, 345568);
        CHECK(path_is_legal(173019, 345568, path));
        CHECK(compute_path_travel_time(path) <= 44.17847562958508);
        
        path = find_path_between_intersections(192014, 180262);
        CHECK(path_is_legal(192014, 180262, path));
        CHECK(compute_path_travel_time(path) <= 64.84763344887986);
        
        path = find_path_between_intersections(225326, 174147);
        CHECK(path_is_legal(225326, 174147, path));
        CHECK(compute_path_travel_time(path) <= 76.56061190226308);
        
        path = find_path_between_intersections(254753, 354223);
        CHECK(path_is_legal(254753, 354223, path));
        CHECK(compute_path_travel_time(path) <= 94.79831888019777);
        
        path = find_path_between_intersections(262331, 369695);
        CHECK(path_is_legal(262331, 369695, path));
        CHECK(compute_path_travel_time(path) <= 60.68190449008559);
        
        path = find_path_between_intersections(263120, 390122);
        CHECK(path_is_legal(263120, 390122, path));
        CHECK(compute_path_travel_time(path) <= 62.1680855828268);
        
        path = find_path_between_intersections(266678, 189134);
        CHECK(path_is_legal(266678, 189134, path));
        CHECK(compute_path_travel_time(path) <= 66.3328801334699);
        
        path = find_path_between_intersections(317752, 422938);
        CHECK(path_is_legal(317752, 422938, path));
        CHECK(compute_path_travel_time(path) <= 55.51822845169531);
        
        path = find_path_between_intersections(323484, 137447);
        CHECK(path_is_legal(323484, 137447, path));
        CHECK(compute_path_travel_time(path) <= 60.58072757004295);
        
        path = find_path_between_intersections(259206, 40967);
        CHECK(path_is_legal(259206, 40967, path));
        CHECK(compute_path_travel_time(path) <= 73.0555896694477);
        
        path = find_path_between_intersections(383569, 435396);
        CHECK(path_is_legal(383569, 435396, path));
        CHECK(compute_path_travel_time(path) <= 55.30350046879541);
        
        path = find_path_between_intersections(269834, 253401);
        CHECK(path_is_legal(269834, 253401, path));
        CHECK(compute_path_travel_time(path) <= 29.31883837164353);
        
        path = find_path_between_intersections(393573, 285718);
        CHECK(path_is_legal(393573, 285718, path));
        CHECK(compute_path_travel_time(path) <= 32.81781643537216);
        
        path = find_path_between_intersections(394464, 420716);
        CHECK(path_is_legal(394464, 420716, path));
        CHECK(compute_path_travel_time(path) <= 25.0654601730387);
        
        path = find_path_between_intersections(396787, 128650);
        CHECK(path_is_legal(396787, 128650, path));
        CHECK(compute_path_travel_time(path) <= 30.37215216722221);
        
        path = find_path_between_intersections(397682, 434180);
        CHECK(path_is_legal(397682, 434180, path));
        CHECK(compute_path_travel_time(path) <= 33.8077844192964);
        
        path = find_path_between_intersections(252490, 71723);
        CHECK(path_is_legal(252490, 71723, path));
        CHECK(compute_path_travel_time(path) <= 38.47418148020807);
        
        path = find_path_between_intersections(409651, 181426);
        CHECK(path_is_legal(409651, 181426, path));
        CHECK(compute_path_travel_time(path) <= 74.88750919080289);
        
        path = find_path_between_intersections(31355, 83947);
        CHECK(path_is_legal(31355, 83947, path));
        CHECK(compute_path_travel_time(path) <= 42.38530245997912);
        
        path = find_path_between_intersections(258992, 223898);
        CHECK(path_is_legal(258992, 223898, path));
        CHECK(compute_path_travel_time(path) <= 57.59802814693);
        
        path = find_path_between_intersections(423526, 82998);
        CHECK(path_is_legal(423526, 82998, path));
        CHECK(compute_path_travel_time(path) <= 65.12518342483267);
        
        path = find_path_between_intersections(215440, 338157);
        CHECK(path_is_legal(215440, 338157, path));
        CHECK(compute_path_travel_time(path) <= 63.33339160570408);
        
        path = find_path_between_intersections(301532, 238145);
        CHECK(path_is_legal(301532, 238145, path));
        CHECK(compute_path_travel_time(path) <= 84.55687516549828);
        
    } //find_path_between_intersections_optimality

} //inter_inter_path_func_public


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

SUITE(inter_inter_path_perf_medium_public) {
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_medium) {
        //Verify Functionality
        std::vector<unsigned> path;
        path = find_path_between_intersections(8728, 56234);
        CHECK(path_is_legal(8728, 56234, path));
        CHECK(compute_path_travel_time(path) <= 22.40935826505194);
        
        path = find_path_between_intersections(18023, 17557);
        CHECK(path_is_legal(18023, 17557, path));
        CHECK(compute_path_travel_time(path) <= 17.30127091530115);
        
        path = find_path_between_intersections(19861, 21094);
        CHECK(path_is_legal(19861, 21094, path));
        CHECK(compute_path_travel_time(path) <= 10.05885076570119);
        
        path = find_path_between_intersections(26357, 42816);
        CHECK(path_is_legal(26357, 42816, path));
        CHECK(compute_path_travel_time(path) <= 24.84135019639521);
        
        path = find_path_between_intersections(30713, 15029);
        CHECK(path_is_legal(30713, 15029, path));
        CHECK(compute_path_travel_time(path) <= 33.23231562285459);
        
        path = find_path_between_intersections(33161, 83491);
        CHECK(path_is_legal(33161, 83491, path));
        CHECK(compute_path_travel_time(path) <= 51.51496850626595);
        
        path = find_path_between_intersections(35624, 52858);
        CHECK(path_is_legal(35624, 52858, path));
        CHECK(compute_path_travel_time(path) <= 26.48040392658803);
        
        path = find_path_between_intersections(35868, 80436);
        CHECK(path_is_legal(35868, 80436, path));
        CHECK(compute_path_travel_time(path) <= 11.40767908664305);
        
        path = find_path_between_intersections(39596, 86413);
        CHECK(path_is_legal(39596, 86413, path));
        CHECK(compute_path_travel_time(path) <= 30.64138105036562);
        
        path = find_path_between_intersections(40525, 12160);
        CHECK(path_is_legal(40525, 12160, path));
        CHECK(compute_path_travel_time(path) <= 23.06917674343352);
        
        path = find_path_between_intersections(42070, 89655);
        CHECK(path_is_legal(42070, 89655, path));
        CHECK(compute_path_travel_time(path) <= 17.23554494902636);
        
        path = find_path_between_intersections(50302, 15940);
        CHECK(path_is_legal(50302, 15940, path));
        CHECK(compute_path_travel_time(path) <= 41.26897985832751);
        
        path = find_path_between_intersections(62386, 61937);
        CHECK(path_is_legal(62386, 61937, path));
        CHECK(compute_path_travel_time(path) <= 30.92292054231461);
        
        path = find_path_between_intersections(62720, 52143);
        CHECK(path_is_legal(62720, 52143, path));
        CHECK(compute_path_travel_time(path) <= 23.0774896951146);
        
        path = find_path_between_intersections(2636, 31486);
        CHECK(path_is_legal(2636, 31486, path));
        CHECK(compute_path_travel_time(path) <= 32.1434768034575);
        
        path = find_path_between_intersections(33081, 21942);
        CHECK(path_is_legal(33081, 21942, path));
        CHECK(compute_path_travel_time(path) <= 21.69964962484224);
        
        path = find_path_between_intersections(67741, 102448);
        CHECK(path_is_legal(67741, 102448, path));
        CHECK(compute_path_travel_time(path) <= 23.10109201629788);
        
        path = find_path_between_intersections(72596, 104857);
        CHECK(path_is_legal(72596, 104857, path));
        CHECK(compute_path_travel_time(path) <= 13.11177246812927);
        
        path = find_path_between_intersections(76743, 87438);
        CHECK(path_is_legal(76743, 87438, path));
        CHECK(compute_path_travel_time(path) <= 40.56964713061821);
        
        path = find_path_between_intersections(31538, 78070);
        CHECK(path_is_legal(31538, 78070, path));
        CHECK(compute_path_travel_time(path) <= 47.9225699183859);
        
        path = find_path_between_intersections(24418, 96758);
        CHECK(path_is_legal(24418, 96758, path));
        CHECK(compute_path_travel_time(path) <= 13.48719450091125);
        
        path = find_path_between_intersections(82778, 97952);
        CHECK(path_is_legal(82778, 97952, path));
        CHECK(compute_path_travel_time(path) <= 36.36921398423257);
        
        path = find_path_between_intersections(38003, 58857);
        CHECK(path_is_legal(38003, 58857, path));
        CHECK(compute_path_travel_time(path) <= 35.09693214546925);
        
        path = find_path_between_intersections(86312, 101157);
        CHECK(path_is_legal(86312, 101157, path));
        CHECK(compute_path_travel_time(path) <= 34.69812253868157);
        
        path = find_path_between_intersections(93150, 103069);
        CHECK(path_is_legal(93150, 103069, path));
        CHECK(compute_path_travel_time(path) <= 17.10188183469547);
        
        path = find_path_between_intersections(98845, 35874);
        CHECK(path_is_legal(98845, 35874, path));
        CHECK(compute_path_travel_time(path) <= 26.16976705265342);
        
        path = find_path_between_intersections(104879, 83021);
        CHECK(path_is_legal(104879, 83021, path));
        CHECK(compute_path_travel_time(path) <= 42.03826598789122);
        
        path = find_path_between_intersections(2497, 70944);
        CHECK(path_is_legal(2497, 70944, path));
        CHECK(compute_path_travel_time(path) <= 17.15226476170689);
        
        path = find_path_between_intersections(106503, 23930);
        CHECK(path_is_legal(106503, 23930, path));
        CHECK(compute_path_travel_time(path) <= 10.12525316911284);
        
        path = find_path_between_intersections(43626, 100197);
        CHECK(path_is_legal(43626, 100197, path));
        CHECK(compute_path_travel_time(path) <= 20.48027808918411);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids1;
        std::vector<unsigned> intersection_ids2;
        for(size_t i = 0; i < 150; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(13767);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 150; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i]);
            }
        }
    } //find_path_between_intersections_perf_medium

} //inter_inter_path_perf_medium_public


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

SUITE(inter_inter_path_perf_very_hard_public) {
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_very_hard) {
        //Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(527, "Subway");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(527, 71262, path));
        CHECK(path_cost <= 1.068165822485444);
        
        path = find_path_to_point_of_interest(1783, "Dollar Thrifty");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(1783, 55999, path));
        CHECK(path_cost <= 18.35207504427872);
        
        path = find_path_to_point_of_interest(7723, "Williams");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(7723, 28412, path));
        CHECK(path_cost <= 17.39004298898955);
        
        path = find_path_to_point_of_interest(8324, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(8324, 16263, path));
        CHECK(path_cost <= 4.25336854039895);
        
        path = find_path_to_point_of_interest(11212, "Cango");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(11212, 17219, path));
        CHECK(path_cost <= 6.422993380043536);
        
        path = find_path_to_point_of_interest(24765, "Dutch Dreams");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(24765, 12449, path));
        CHECK(path_cost <= 21.7227154516025);
        
        path = find_path_to_point_of_interest(702, "Bier Markt");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(702, 22506, path));
        CHECK(path_cost <= 14.20112906566564);
        
        path = find_path_to_point_of_interest(38189, "Bus Terminal Family Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(38189, 16695, path));
        CHECK(path_cost <= 39.58259363666975);
        
        path = find_path_to_point_of_interest(11136, "Beach Hill Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(11136, 11112, path));
        CHECK(path_cost <= 1.856114717023139);
        
        path = find_path_to_point_of_interest(54519, "Dairy Queen");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(54519, 33174, path));
        CHECK(path_cost <= 6.971907501740946);
        
        path = find_path_to_point_of_interest(35906, "Woodview Park Public Library");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(35906, 57564, path));
        CHECK(path_cost <= 18.49301807856099);
        
        path = find_path_to_point_of_interest(55506, "Laterna Family Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(55506, 5988, path));
        CHECK(path_cost <= 24.42633754838081);
        
        path = find_path_to_point_of_interest(59713, "Cmb");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(59713, 47577, path));
        CHECK(path_cost <= 37.60392894562936);
        
        path = find_path_to_point_of_interest(62214, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(62214, 26814, path));
        CHECK(path_cost <= 2.446867408221389);
        
        path = find_path_to_point_of_interest(70886, "Sheridan Pharmacy");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(70886, 31279, path));
        CHECK(path_cost <= 30.12085038282869);
        
        path = find_path_to_point_of_interest(16036, "Redbox");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(16036, 4182, path));
        CHECK(path_cost <= 7.719554871891305);
        
        path = find_path_to_point_of_interest(80264, "Quizno's Sub");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(80264, 4111, path));
        CHECK(path_cost <= 1.602191932434293);
        
        path = find_path_to_point_of_interest(81243, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(81243, 35124, path));
        CHECK(path_cost <= 12.7763364449506);
        
        path = find_path_to_point_of_interest(82647, "High Seas Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(82647, 58670, path));
        CHECK(path_cost <= 44.1368841763901);
        
        path = find_path_to_point_of_interest(84631, "Miyako Sushi");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(84631, 75066, path));
        CHECK(path_cost <= 37.05987747497068);
        
        path = find_path_to_point_of_interest(86030, "Swiss Chalet");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(86030, 1587, path));
        CHECK(path_cost <= 7.650192862378792);
        
        path = find_path_to_point_of_interest(88743, "Luna Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(88743, 2455, path));
        CHECK(path_cost <= 26.25460428704528);
        
        path = find_path_to_point_of_interest(80174, "Graydon Bar & Grill");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(80174, 27076, path));
        CHECK(path_cost <= 9.819533850703051);
        
        path = find_path_to_point_of_interest(89524, "Fran's Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(89524, 2276, path));
        CHECK(path_cost <= 22.67845734504223);
        
        path = find_path_to_point_of_interest(17397, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(17397, 15793, path));
        CHECK(path_cost <= 3.830482855024635);
        
        path = find_path_to_point_of_interest(21562, "BMO Bank of Montreal");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(21562, 43893, path));
        CHECK(path_cost <= 6.005368811622251);
        
        path = find_path_to_point_of_interest(5982, "McDonald's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(5982, 25725, path));
        CHECK(path_cost <= 0.8674770880435791);
        
        path = find_path_to_point_of_interest(104831, "Union Market");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(104831, 70808, path));
        CHECK(path_cost <= 32.61650091088489);
        
        path = find_path_to_point_of_interest(4280, "Swiss Chalet");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(4280, 12790, path));
        CHECK(path_cost <= 2.071676038562121);
        
        path = find_path_to_point_of_interest(50702, "Fran's Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(50702, 2276, path));
        CHECK(path_cost <= 2.634898685822257);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 400; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(15455);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 400; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_very_hard

} //inter_inter_path_perf_very_hard_public


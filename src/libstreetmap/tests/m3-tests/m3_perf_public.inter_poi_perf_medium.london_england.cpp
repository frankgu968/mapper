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
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_medium) {
        //Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(76316, "Medicos");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(76316, 70682, path));
        CHECK(path_cost <= 21.29621563067802);
        
        path = find_path_to_point_of_interest(82195, "Oasis Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(82195, 26781, path));
        CHECK(path_cost <= 40.29698924757187);
        
        path = find_path_to_point_of_interest(108532, "Percento");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(108532, 3471, path));
        CHECK(path_cost <= 30.56397828686709);
        
        path = find_path_to_point_of_interest(110064, "Grayshott Library");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(110064, 280628, path));
        CHECK(path_cost <= 60.68541600691006);
        
        path = find_path_to_point_of_interest(113141, "Pizza de Vinci");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(113141, 44645, path));
        CHECK(path_cost <= 44.60555790895157);
        
        path = find_path_to_point_of_interest(130724, "Sweet Patisserie & Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(130724, 145577, path));
        CHECK(path_cost <= 16.4685669184868);
        
        path = find_path_to_point_of_interest(131581, "Troy Bar");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(131581, 45255, path));
        CHECK(path_cost <= 71.79641055915388);
        
        path = find_path_to_point_of_interest(139923, "Internet Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(139923, 32619, path));
        CHECK(path_cost <= 33.06489092681248);
        
        path = find_path_to_point_of_interest(158399, "Motorcycle parking");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(158399, 247124, path));
        CHECK(path_cost <= 7.148995277140836);
        
        path = find_path_to_point_of_interest(170276, "Halstead Hospital");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(170276, 219231, path));
        CHECK(path_cost <= 39.60576692295461);
        
        path = find_path_to_point_of_interest(190154, "Rumwong");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(190154, 18357, path));
        CHECK(path_cost <= 63.11682335907485);
        
        path = find_path_to_point_of_interest(132269, "Dunstable Recycling Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(132269, 221944, path));
        CHECK(path_cost <= 30.90929999968671);
        
        path = find_path_to_point_of_interest(209487, "Fairfields Arts Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(209487, 32317, path));
        CHECK(path_cost <= 23.53831432303961);
        
        path = find_path_to_point_of_interest(146649, "Goffs Manor");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(146649, 166611, path));
        CHECK(path_cost <= 64.09318859557992);
        
        path = find_path_to_point_of_interest(239093, "The Bickley");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(239093, 106262, path));
        CHECK(path_cost <= 65.17483139236703);
        
        path = find_path_to_point_of_interest(90214, "Electric Car Charge Point");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(90214, 134664, path));
        CHECK(path_cost <= 14.52467153521052);
        
        path = find_path_to_point_of_interest(291106, "The Marquis");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(291106, 296272, path));
        CHECK(path_cost <= 71.49889830141258);
        
        path = find_path_to_point_of_interest(291638, "Harrow Road Dental Surgery");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(291638, 80804, path));
        CHECK(path_cost <= 36.85516068653396);
        
        path = find_path_to_point_of_interest(324969, "Harvester");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(324969, 3155, path));
        CHECK(path_cost <= 7.472819574493185);
        
        path = find_path_to_point_of_interest(358371, "The Green Man");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(358371, 10712, path));
        CHECK(path_cost <= 17.74043088598555);
        
        path = find_path_to_point_of_interest(397517, "Kinloch");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(397517, 368121, path));
        CHECK(path_cost <= 109.4826306308485);
        
        path = find_path_to_point_of_interest(409651, "The Swan");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(409651, 295423, path));
        CHECK(path_cost <= 26.64839857919893);
        
        path = find_path_to_point_of_interest(377157, "KFC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(377157, 288024, path));
        CHECK(path_cost <= 15.42802469773);
        
        path = find_path_to_point_of_interest(321658, "Caledonia");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(321658, 121590, path));
        CHECK(path_cost <= 85.02395437096368);
        
        path = find_path_to_point_of_interest(212011, "The Woolpack");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(212011, 37308, path));
        CHECK(path_cost <= 38.91986059019084);
        
        path = find_path_to_point_of_interest(430248, "Starbucks");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(430248, 430236, path));
        CHECK(path_cost <= 5.033377498195328);
        
        path = find_path_to_point_of_interest(52129, "Little Unicorn Day Nursery");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52129, 62149, path));
        CHECK(path_cost <= 63.19101685673784);
        
        path = find_path_to_point_of_interest(226218, "Venue 1");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(226218, 33965, path));
        CHECK(path_cost <= 59.41285589002651);
        
        path = find_path_to_point_of_interest(336095, "The Blue Anchor");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(336095, 34050, path));
        CHECK(path_cost <= 58.34728491963622);
        
        path = find_path_to_point_of_interest(398678, "Perfect Pizza");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(398678, 339721, path));
        CHECK(path_cost <= 29.35339202137003);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 10; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(22927);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 10; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_medium

} //inter_inter_path_perf_medium_public


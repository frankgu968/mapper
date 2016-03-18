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
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_hard) {
        //Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(7452, "Highfield Junior School");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(7452, 219602, path));
        CHECK(path_cost <= 14.26246753080903);
        
        path = find_path_to_point_of_interest(45804, "Salvation Army");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(45804, 65002, path));
        CHECK(path_cost <= 22.71730485802648);
        
        path = find_path_to_point_of_interest(78336, "Lloyds TSB");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(78336, 76743, path));
        CHECK(path_cost <= 3.506731923311324);
        
        path = find_path_to_point_of_interest(107580, "The Stockpot");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(107580, 47497, path));
        CHECK(path_cost <= 45.70915401953278);
        
        path = find_path_to_point_of_interest(108532, "Percento");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(108532, 3471, path));
        CHECK(path_cost <= 30.56397828686709);
        
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
        
        path = find_path_to_point_of_interest(158399, "Motorcycle parking");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(158399, 247124, path));
        CHECK(path_cost <= 7.148995277140836);
        
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
        
        path = find_path_to_point_of_interest(215219, "Buffet Brazil");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(215219, 289869, path));
        CHECK(path_cost <= 40.45832358112393);
        
        path = find_path_to_point_of_interest(146649, "Goffs Manor");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(146649, 166611, path));
        CHECK(path_cost <= 64.09318859557992);
        
        path = find_path_to_point_of_interest(90214, "Electric Car Charge Point");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(90214, 134664, path));
        CHECK(path_cost <= 14.52467153521052);
        
        path = find_path_to_point_of_interest(279160, "Railway Tavern");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(279160, 296506, path));
        CHECK(path_cost <= 28.5457770726319);
        
        path = find_path_to_point_of_interest(309150, "The Chequers Inn");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(309150, 361635, path));
        CHECK(path_cost <= 28.07966209309162);
        
        path = find_path_to_point_of_interest(320808, "Kew Baptist Church");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(320808, 75468, path));
        CHECK(path_cost <= 54.36824764378157);
        
        path = find_path_to_point_of_interest(349638, "Stepping Stones Cookie Bar");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(349638, 325487, path));
        CHECK(path_cost <= 64.0169670320157);
        
        path = find_path_to_point_of_interest(358371, "The Green Man");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(358371, 10712, path));
        CHECK(path_cost <= 17.74043088598555);
        
        path = find_path_to_point_of_interest(397517, "Kinloch");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(397517, 368121, path));
        CHECK(path_cost <= 109.4826306308485);
        
        path = find_path_to_point_of_interest(412105, "St. Mary's C. of E. Controlled Infant School");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(412105, 81231, path));
        CHECK(path_cost <= 87.44695453769459);
        
        path = find_path_to_point_of_interest(377157, "KFC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(377157, 288024, path));
        CHECK(path_cost <= 15.42802469773);
        
        path = find_path_to_point_of_interest(418914, "Brumus");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(418914, 48050, path));
        CHECK(path_cost <= 4.516655855115053);
        
        path = find_path_to_point_of_interest(212011, "The Woolpack");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(212011, 37308, path));
        CHECK(path_cost <= 38.91986059019084);
        
        path = find_path_to_point_of_interest(131200, "Bar 104");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(131200, 384442, path));
        CHECK(path_cost <= 30.82296858580054);
        
        path = find_path_to_point_of_interest(369072, "Rayne Road Service Station");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(369072, 197642, path));
        CHECK(path_cost <= 91.49420379509925);
        
        path = find_path_to_point_of_interest(52129, "Little Unicorn Day Nursery");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52129, 62149, path));
        CHECK(path_cost <= 63.19101685673784);
        
        path = find_path_to_point_of_interest(93567, "Temple Bar");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(93567, 33984, path));
        CHECK(path_cost <= 16.36287564244939);
        
        path = find_path_to_point_of_interest(398678, "Perfect Pizza");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(398678, 339721, path));
        CHECK(path_cost <= 29.35339202137003);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 20; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(19056);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 20; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_hard

} //inter_inter_path_perf_hard_public


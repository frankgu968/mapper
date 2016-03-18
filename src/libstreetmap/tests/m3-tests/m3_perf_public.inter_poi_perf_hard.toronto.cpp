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

SUITE(inter_inter_path_perf_hard_public) {
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_hard) {
        //Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(27551, "Academy of Learning");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(27551, 52516, path));
        CHECK(path_cost <= 33.67080749813042);
        
        path = find_path_to_point_of_interest(29446, "Burger King");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(29446, 105466, path));
        CHECK(path_cost <= 7.393560266448425);
        
        path = find_path_to_point_of_interest(29579, "Bluewater Imaging");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(29579, 8890, path));
        CHECK(path_cost <= 24.98098147288382);
        
        path = find_path_to_point_of_interest(33720, "Kelsey's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(33720, 74813, path));
        CHECK(path_cost <= 4.976781721203628);
        
        path = find_path_to_point_of_interest(34192, "Kinderschool");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(34192, 95292, path));
        CHECK(path_cost <= 31.42754752632752);
        
        path = find_path_to_point_of_interest(38779, "Texas Longhorn Grill");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(38779, 77280, path));
        CHECK(path_cost <= 20.77506179912472);
        
        path = find_path_to_point_of_interest(39235, "Muddy Duck");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(39235, 82019, path));
        CHECK(path_cost <= 20.51244522054791);
        
        path = find_path_to_point_of_interest(44189, "Subway");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(44189, 44161, path));
        CHECK(path_cost <= 0.8982448094684926);
        
        path = find_path_to_point_of_interest(44813, "Petro-Canada");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(44813, 26459, path));
        CHECK(path_cost <= 3.869955419511921);
        
        path = find_path_to_point_of_interest(29017, "Mr. Sub");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(29017, 54633, path));
        CHECK(path_cost <= 3.165728826004802);
        
        path = find_path_to_point_of_interest(52414, "Dirty Martini");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52414, 39078, path));
        CHECK(path_cost <= 37.54370260407567);
        
        path = find_path_to_point_of_interest(49190, "St. Boniface Rectory");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(49190, 18401, path));
        CHECK(path_cost <= 12.84518096762889);
        
        path = find_path_to_point_of_interest(65937, "New College Cafeteria");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(65937, 1081, path));
        CHECK(path_cost <= 30.97132000558135);
        
        path = find_path_to_point_of_interest(66262, "CIBC Banking Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(66262, 27882, path));
        CHECK(path_cost <= 7.261224079325205);
        
        path = find_path_to_point_of_interest(67359, "the abelard school");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(67359, 5206, path));
        CHECK(path_cost <= 21.05697692543371);
        
        path = find_path_to_point_of_interest(51890, "CIBC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(51890, 3678, path));
        CHECK(path_cost <= 4.736476049197243);
        
        path = find_path_to_point_of_interest(1467, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(1467, 1666, path));
        CHECK(path_cost <= 0.9331013293257654);
        
        path = find_path_to_point_of_interest(74732, "Sports Centre Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(74732, 106666, path));
        CHECK(path_cost <= 29.65107550038755);
        
        path = find_path_to_point_of_interest(74985, "Gino's Pizza");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(74985, 3705, path));
        CHECK(path_cost <= 15.26939503876498);
        
        path = find_path_to_point_of_interest(77800, "Eden");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(77800, 22258, path));
        CHECK(path_cost <= 24.12142525637355);
        
        path = find_path_to_point_of_interest(80807, "Subway's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(80807, 94357, path));
        CHECK(path_cost <= 20.24645198894281);
        
        path = find_path_to_point_of_interest(82200, "Subway");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(82200, 7854, path));
        CHECK(path_cost <= 3.589387186784175);
        
        path = find_path_to_point_of_interest(84663, "Screen Lounge & Rooftop Patio");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(84663, 8874, path));
        CHECK(path_cost <= 20.25483012965738);
        
        path = find_path_to_point_of_interest(49676, "Second Cup");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(49676, 29789, path));
        CHECK(path_cost <= 13.81580315698098);
        
        path = find_path_to_point_of_interest(92575, "The Seanachai Irish Pub");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(92575, 13789, path));
        CHECK(path_cost <= 26.42421600796336);
        
        path = find_path_to_point_of_interest(93303, "Scotiabank");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(93303, 93135, path));
        CHECK(path_cost <= 2.278649210048938);
        
        path = find_path_to_point_of_interest(78838, "Colossus");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(78838, 33556, path));
        CHECK(path_cost <= 40.01807577361612);
        
        path = find_path_to_point_of_interest(99513, "Cafe Supreme");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(99513, 1896, path));
        CHECK(path_cost <= 19.7168139891804);
        
        path = find_path_to_point_of_interest(97872, "Galilee Presbyterian Church");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(97872, 11805, path));
        CHECK(path_cost <= 23.09619674208252);
        
        path = find_path_to_point_of_interest(102078, "RBC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(102078, 27134, path));
        CHECK(path_cost <= 4.374672505561911);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 200; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(17396);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 200; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_hard

} //inter_inter_path_perf_hard_public


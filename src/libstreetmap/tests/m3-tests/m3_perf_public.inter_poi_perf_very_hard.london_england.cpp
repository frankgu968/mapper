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

SUITE(inter_inter_path_perf_very_hard_public) {
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_very_hard) {
        std::cout << "HARDEST LONDON TESTS" << std::endl;

    	//Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(12647, "Shaftesbury Medical Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(12647, 199685, path));
        CHECK(path_cost <= 20.65636001246935);
        
        path = find_path_to_point_of_interest(40365, "Raynes Park Tavern");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(40365, 23035, path));
        CHECK(path_cost <= 66.01601578611825);
        
        path = find_path_to_point_of_interest(46460, "McColl's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(46460, 340210, path));
        CHECK(path_cost <= 47.09511474222683);
        
        path = find_path_to_point_of_interest(65219, "The Crown and Greyhound");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(65219, 90535, path));
        CHECK(path_cost <= 44.961388506456);
        
        path = find_path_to_point_of_interest(74686, "Garden City Practice: Knightsfield Surgery");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(74686, 89639, path));
        CHECK(path_cost <= 35.12358827899183);
        
        path = find_path_to_point_of_interest(98689, "Redemption Academy");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(98689, 358644, path));
        CHECK(path_cost <= 71.43241781789156);
        
        path = find_path_to_point_of_interest(53007, "Salon Graduates");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(53007, 14673, path));
        CHECK(path_cost <= 22.37869525247826);
        
        path = find_path_to_point_of_interest(135726, "Grosvenor Fountain");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(135726, 431957, path));
        CHECK(path_cost <= 72.81046108676794);
        
        path = find_path_to_point_of_interest(150510, "Munchies");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(150510, 44559, path));
        CHECK(path_cost <= 47.34413276141531);
        
        path = find_path_to_point_of_interest(157536, "Islington Local History Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(157536, 273769, path));
        CHECK(path_cost <= 52.36321432898309);
        
        path = find_path_to_point_of_interest(158399, "Motorcycle parking");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(158399, 247124, path));
        CHECK(path_cost <= 7.148995277140836);
        
        path = find_path_to_point_of_interest(160835, "Carluccio's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(160835, 404814, path));
        CHECK(path_cost <= 44.95730269247647);
        
        path = find_path_to_point_of_interest(162838, "John Bell & Croyden");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(162838, 161109, path));
        CHECK(path_cost <= 42.05919997477861);
        
        path = find_path_to_point_of_interest(177568, "Chapel");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(177568, 110094, path));
        CHECK(path_cost <= 27.12518359389975);
        
        path = find_path_to_point_of_interest(182075, "Arch 1");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(182075, 145194, path));
        CHECK(path_cost <= 36.3298824429364);
        
        path = find_path_to_point_of_interest(187180, "Earlsfield Library");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(187180, 52041, path));
        CHECK(path_cost <= 29.45376514753411);
        
        path = find_path_to_point_of_interest(188089, "The Plough");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(188089, 184029, path));
        CHECK(path_cost <= 26.72120874122366);
        
        path = find_path_to_point_of_interest(245969, "Barclays");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(245969, 291141, path));
        CHECK(path_cost <= 2.933597736049049);
        
        path = find_path_to_point_of_interest(79201, "Rest A While");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(79201, 182919, path));
        CHECK(path_cost <= 64.44052713549944);
        
        path = find_path_to_point_of_interest(278332, "University of London Union");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(278332, 26746, path));
        CHECK(path_cost <= 52.40330794949509);
        
        path = find_path_to_point_of_interest(17398, "Thai Dream");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(17398, 33448, path));
        CHECK(path_cost <= 22.11559760897634);
        
        path = find_path_to_point_of_interest(315109, "Wagamama");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(315109, 18321, path));
        CHECK(path_cost <= 7.615334349805999);
        
        path = find_path_to_point_of_interest(318831, "The Royal Oak");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(318831, 299312, path));
        CHECK(path_cost <= 25.342983096254);
        
        path = find_path_to_point_of_interest(358371, "The Green Man");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(358371, 10712, path));
        CHECK(path_cost <= 17.74043088598555);
        
        path = find_path_to_point_of_interest(293937, "Tangerine Dream Cafe");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(293937, 20825, path));
        CHECK(path_cost <= 64.70293597063464);
        
        path = find_path_to_point_of_interest(169435, "Man of Kent");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(169435, 87532, path));
        CHECK(path_cost <= 56.31327098992601);
        
        path = find_path_to_point_of_interest(377157, "KFC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(377157, 288024, path));
        CHECK(path_cost <= 15.42802469773);
        
        path = find_path_to_point_of_interest(422500, "Motorcycle parking");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(422500, 90163, path));
        CHECK(path_cost <= 9.033395535841429);
        
        path = find_path_to_point_of_interest(402070, "The Waterloo");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(402070, 87560, path));
        CHECK(path_cost <= 72.9336220733915);
        
        path = find_path_to_point_of_interest(52129, "Little Unicorn Day Nursery");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52129, 62149, path));
        CHECK(path_cost <= 63.19101685673784);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 70; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            //ECE297_TIME_CONSTRAINT(14834);
        	ECE297_TIME_CONSTRAINT(1);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 70; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_very_hard

} //inter_inter_path_perf_very_hard_public


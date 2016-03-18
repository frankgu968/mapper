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
    TEST_FIXTURE(MapFixture, find_path_to_point_of_interest_perf_medium) {
    	//Verify Functionality
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(2521, "Dairy Queen");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(2521, 70806, path));
        CHECK(path_cost <= 6.901171572310488);
        
        path = find_path_to_point_of_interest(11395, "Touchless wash");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(11395, 20009, path));
        CHECK(path_cost <= 22.82105704747717);
        
        path = find_path_to_point_of_interest(13931, "Fresh");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(13931, 1505, path));
        CHECK(path_cost <= 9.455237772415813);
        
        path = find_path_to_point_of_interest(18320, "Comfort Zone");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(18320, 1116, path));
        CHECK(path_cost <= 24.66880614971382);
        
        path = find_path_to_point_of_interest(22657, "Alterna Savings");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(22657, 35700, path));
        CHECK(path_cost <= 8.66742845372306);
        
        path = find_path_to_point_of_interest(27383, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(27383, 92235, path));
        CHECK(path_cost <= 5.075067854009808);
        
        path = find_path_to_point_of_interest(31403, "Lakeshore Humber Community Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(31403, 99039, path));
        CHECK(path_cost <= 20.12608934974415);
        
        path = find_path_to_point_of_interest(35108, "Muddy Duck");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(35108, 82019, path));
        CHECK(path_cost <= 38.94614214664826);
        
        path = find_path_to_point_of_interest(38968, "Trio 3");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(38968, 62037, path));
        CHECK(path_cost <= 8.120344486859755);
        
        path = find_path_to_point_of_interest(41138, "RBC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(41138, 20668, path));
        CHECK(path_cost <= 10.37366125696136);
        
        path = find_path_to_point_of_interest(43679, "The Local");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(43679, 51820, path));
        CHECK(path_cost <= 37.39178432504364);
        
        path = find_path_to_point_of_interest(45533, "Pizza Hut/Wing Street");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(45533, 45638, path));
        CHECK(path_cost <= 2.062825696088247);
        
        path = find_path_to_point_of_interest(59662, "Shoppers Drug Mart");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(59662, 99962, path));
        CHECK(path_cost <= 1.381910700105829);
        
        path = find_path_to_point_of_interest(59889, "Harvey's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(59889, 82122, path));
        CHECK(path_cost <= 6.716786844298737);
        
        path = find_path_to_point_of_interest(63608, "Cultures");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(63608, 8874, path));
        CHECK(path_cost <= 32.24495934120613);
        
        path = find_path_to_point_of_interest(12677, "Shell");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(12677, 94640, path));
        CHECK(path_cost <= 4.539348936444736);
        
        path = find_path_to_point_of_interest(51656, "John's San Francesco Food");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(51656, 83300, path));
        CHECK(path_cost <= 35.2289180940803);
        
        path = find_path_to_point_of_interest(63975, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(63975, 106756, path));
        CHECK(path_cost <= 2.062360198697549);
        
        path = find_path_to_point_of_interest(80079, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(80079, 73443, path));
        CHECK(path_cost <= 3.084545423207498);
        
        path = find_path_to_point_of_interest(80851, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(80851, 4297, path));
        CHECK(path_cost <= 2.428228191552658);
        
        path = find_path_to_point_of_interest(81171, "Bull Dog Coffee");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(81171, 8889, path));
        CHECK(path_cost <= 16.15522179833042);
        
        path = find_path_to_point_of_interest(44178, "The Harp");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(44178, 28501, path));
        CHECK(path_cost <= 19.40524105772512);
        
        path = find_path_to_point_of_interest(47060, "Beverley St & Grange Ave");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(47060, 5171, path));
        CHECK(path_cost <= 37.12068600752257);
        
        path = find_path_to_point_of_interest(71751, "Beach Hill Restaurant");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(71751, 11112, path));
        CHECK(path_cost <= 12.69172055264667);
        
        path = find_path_to_point_of_interest(100186, "Ora Mangiamo");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(100186, 35146, path));
        CHECK(path_cost <= 39.92518970726919);
        
        path = find_path_to_point_of_interest(104453, "Esso Car Wash");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(104453, 4131, path));
        CHECK(path_cost <= 30.03922838965145);
        
        path = find_path_to_point_of_interest(104947, "Pizza Nova");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(104947, 104874, path));
        CHECK(path_cost <= 4.940068463551871);
        
        path = find_path_to_point_of_interest(98769, "Positive Changes Hypnosis");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(98769, 88404, path));
        CHECK(path_cost <= 34.48280651395154);
        
        path = find_path_to_point_of_interest(102530, "Canadian Tire");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(102530, 72339, path));
        CHECK(path_cost <= 18.43564011601132);
        
        path = find_path_to_point_of_interest(52119, "Druxy's Famous Deli");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52119, 1861, path));
        CHECK(path_cost <= 11.19798310569871);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        std::vector<std::string> poi_names;
        for(size_t i = 0; i < 100; i++) {
            intersection_ids.push_back(rand_intersection(rng));
            poi_names.push_back(getPointOfInterestName(rand_poi(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(150887);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 100; i++) {
                result = find_path_to_point_of_interest(intersection_ids[i], poi_names[i]);
            }
        }
    } //find_path_to_point_of_interest_perf_medium

} //inter_inter_path_perf_medium_public


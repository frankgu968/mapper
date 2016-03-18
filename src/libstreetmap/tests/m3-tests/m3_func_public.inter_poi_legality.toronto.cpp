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

SUITE(inter_poi_path_func_public) {
    TEST(find_path_to_point_of_interest_functionality) {
        std::vector<unsigned> path;
        double path_cost;
        
        path = find_path_to_point_of_interest(9672, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(9672, 107639, path));
        CHECK(path_cost <= 11.40146041953046);
        
        path = find_path_to_point_of_interest(16427, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(16427, 10077, path));
        CHECK(path_cost <= 2.918858378412318);
        
        path = find_path_to_point_of_interest(2, "Shell");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(2, 52441, path));
        CHECK(path_cost <= 3.576138618193494);
        
        path = find_path_to_point_of_interest(22444, "Nirvana");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(22444, 44616, path));
        CHECK(path_cost <= 16.31553549357065);
        
        path = find_path_to_point_of_interest(10168, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(10168, 90879, path));
        CHECK(path_cost <= 3.992551157558185);
        
        path = find_path_to_point_of_interest(4501, "Mr. Sub");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(4501, 19081, path));
        CHECK(path_cost <= 3.524752465574961);
        
        path = find_path_to_point_of_interest(43148, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(43148, 20638, path));
        CHECK(path_cost <= 4.318390847120953);
        
        path = find_path_to_point_of_interest(42823, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(42823, 75739, path));
        CHECK(path_cost <= 2.704866167446373);
        
        path = find_path_to_point_of_interest(47772, "Pizza Pizza");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(47772, 91457, path));
        CHECK(path_cost <= 7.844468973248699);
        
        path = find_path_to_point_of_interest(52747, "Blumont Bistro");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(52747, 82104, path));
        CHECK(path_cost <= 18.93797118659853);
        
        path = find_path_to_point_of_interest(54883, "Swiss Chalet");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(54883, 78585, path));
        CHECK(path_cost <= 6.774841700851454);
        
        path = find_path_to_point_of_interest(58853, "Shoppers Drug Mart");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(58853, 55855, path));
        CHECK(path_cost <= 3.488990021124339);
        
        path = find_path_to_point_of_interest(59870, "Second Cup");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(59870, 79574, path));
        CHECK(path_cost <= 7.288782476808191);
        
        path = find_path_to_point_of_interest(61340, "Clarkson Montessori School");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(61340, 20125, path));
        CHECK(path_cost <= 33.27888336124622);
        
        path = find_path_to_point_of_interest(70550, "Florentia Ristorante");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(70550, 11504, path));
        CHECK(path_cost <= 19.43104924063637);
        
        path = find_path_to_point_of_interest(26565, "Kelsey's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(26565, 79523, path));
        CHECK(path_cost <= 4.349855073036835);
        
        path = find_path_to_point_of_interest(75396, "Maple Education");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(75396, 80949, path));
        CHECK(path_cost <= 29.00910961786277);
        
        path = find_path_to_point_of_interest(789, "Shoppers Drug Mart");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(789, 81689, path));
        CHECK(path_cost <= 7.412784627563598);
        
        path = find_path_to_point_of_interest(13617, "McDonald's");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(13617, 56532, path));
        CHECK(path_cost <= 4.011949321583365);
        
        path = find_path_to_point_of_interest(39826, "Pango");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(39826, 35320, path));
        CHECK(path_cost <= 15.13886963103538);
        
        path = find_path_to_point_of_interest(84955, "CIBC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(84955, 98160, path));
        CHECK(path_cost <= 3.378731400872689);
        
        path = find_path_to_point_of_interest(86972, "Jiffy Self Storage");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(86972, 59827, path));
        CHECK(path_cost <= 8.701278320743345);
        
        path = find_path_to_point_of_interest(90183, "Dentist");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(90183, 88674, path));
        CHECK(path_cost <= 32.36520774989595);
        
        path = find_path_to_point_of_interest(21548, "Tim Hortons");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(21548, 21238, path));
        CHECK(path_cost <= 4.032169785232221);
        
        path = find_path_to_point_of_interest(18521, "BMO Bank of Montreal");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(18521, 94123, path));
        CHECK(path_cost <= 5.276644859615445);
        
        path = find_path_to_point_of_interest(9140, "TD Canada Trust");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(9140, 4320, path));
        CHECK(path_cost <= 5.509199130058581);
        
        path = find_path_to_point_of_interest(103682, "Animal Hospital");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(103682, 68361, path));
        CHECK(path_cost <= 45.79181940814287);
        
        path = find_path_to_point_of_interest(97609, "Jade Tree Healing Arts Centre");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(97609, 26675, path));
        CHECK(path_cost <= 44.80030980036317);
        
        path = find_path_to_point_of_interest(12519, "RBC");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(12519, 11808, path));
        CHECK(path_cost <= 1.643418843279593);
        
        path = find_path_to_point_of_interest(63719, "Kingsway Baptist Church");
        path_cost = compute_path_travel_time(path);
        CHECK(path_is_legal(63719, 26579, path));
        CHECK(path_cost <= 39.97706228611789);
        
    } //find_path_to_point_of_interest_functionality

} //inter_poi_path_func_public


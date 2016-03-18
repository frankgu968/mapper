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
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_hard) {
        //Verify Functionality
        std::vector<unsigned> path;
        path = find_path_between_intersections(6961, 95058);
        CHECK(path_is_legal(6961, 95058, path));
        CHECK(compute_path_travel_time(path) <= 19.78950655765987);
        
        path = find_path_between_intersections(32225, 31533);
        CHECK(path_is_legal(32225, 31533, path));
        CHECK(compute_path_travel_time(path) <= 21.27588646497684);
        
        path = find_path_between_intersections(17331, 105257);
        CHECK(path_is_legal(17331, 105257, path));
        CHECK(compute_path_travel_time(path) <= 6.258117168598498);
        
        path = find_path_between_intersections(49015, 58530);
        CHECK(path_is_legal(49015, 58530, path));
        CHECK(compute_path_travel_time(path) <= 29.20848826408953);
        
        path = find_path_between_intersections(53629, 58600);
        CHECK(path_is_legal(53629, 58600, path));
        CHECK(compute_path_travel_time(path) <= 33.10854341494707);
        
        path = find_path_between_intersections(59197, 72950);
        CHECK(path_is_legal(59197, 72950, path));
        CHECK(compute_path_travel_time(path) <= 31.71960843061983);
        
        path = find_path_between_intersections(59316, 41071);
        CHECK(path_is_legal(59316, 41071, path));
        CHECK(compute_path_travel_time(path) <= 21.49864992912196);
        
        path = find_path_between_intersections(62093, 65040);
        CHECK(path_is_legal(62093, 65040, path));
        CHECK(compute_path_travel_time(path) <= 34.03046034271838);
        
        path = find_path_between_intersections(65493, 29975);
        CHECK(path_is_legal(65493, 29975, path));
        CHECK(compute_path_travel_time(path) <= 29.33194995465077);
        
        path = find_path_between_intersections(69580, 51986);
        CHECK(path_is_legal(69580, 51986, path));
        CHECK(compute_path_travel_time(path) <= 40.82321820972078);
        
        path = find_path_between_intersections(74192, 81800);
        CHECK(path_is_legal(74192, 81800, path));
        CHECK(compute_path_travel_time(path) <= 27.84435860871897);
        
        path = find_path_between_intersections(77998, 19802);
        CHECK(path_is_legal(77998, 19802, path));
        CHECK(compute_path_travel_time(path) <= 32.54802652055523);
        
        path = find_path_between_intersections(78709, 50714);
        CHECK(path_is_legal(78709, 50714, path));
        CHECK(compute_path_travel_time(path) <= 16.32523275513745);
        
        path = find_path_between_intersections(79873, 82068);
        CHECK(path_is_legal(79873, 82068, path));
        CHECK(compute_path_travel_time(path) <= 25.19464350489908);
        
        path = find_path_between_intersections(31100, 68159);
        CHECK(path_is_legal(31100, 68159, path));
        CHECK(compute_path_travel_time(path) <= 9.528234985127222);
        
        path = find_path_between_intersections(83234, 21756);
        CHECK(path_is_legal(83234, 21756, path));
        CHECK(compute_path_travel_time(path) <= 19.95407533262406);
        
        path = find_path_between_intersections(60400, 84598);
        CHECK(path_is_legal(60400, 84598, path));
        CHECK(compute_path_travel_time(path) <= 23.58388827440856);
        
        path = find_path_between_intersections(87061, 1357);
        CHECK(path_is_legal(87061, 1357, path));
        CHECK(compute_path_travel_time(path) <= 5.254122998204624);
        
        path = find_path_between_intersections(86273, 79201);
        CHECK(path_is_legal(86273, 79201, path));
        CHECK(compute_path_travel_time(path) <= 45.90860142568895);
        
        path = find_path_between_intersections(22978, 11856);
        CHECK(path_is_legal(22978, 11856, path));
        CHECK(compute_path_travel_time(path) <= 29.83903544225734);
        
        path = find_path_between_intersections(69717, 17053);
        CHECK(path_is_legal(69717, 17053, path));
        CHECK(compute_path_travel_time(path) <= 25.39271828582792);
        
        path = find_path_between_intersections(1578, 84940);
        CHECK(path_is_legal(1578, 84940, path));
        CHECK(compute_path_travel_time(path) <= 30.61268289204087);
        
        path = find_path_between_intersections(4727, 6512);
        CHECK(path_is_legal(4727, 6512, path));
        CHECK(compute_path_travel_time(path) <= 21.69787109543915);
        
        path = find_path_between_intersections(85287, 38020);
        CHECK(path_is_legal(85287, 38020, path));
        CHECK(compute_path_travel_time(path) <= 31.7610582021393);
        
        path = find_path_between_intersections(101771, 103558);
        CHECK(path_is_legal(101771, 103558, path));
        CHECK(compute_path_travel_time(path) <= 29.39459231833045);
        
        path = find_path_between_intersections(104335, 96065);
        CHECK(path_is_legal(104335, 96065, path));
        CHECK(compute_path_travel_time(path) <= 22.45160389710521);
        
        path = find_path_between_intersections(8745, 87872);
        CHECK(path_is_legal(8745, 87872, path));
        CHECK(compute_path_travel_time(path) <= 16.61808074682049);
        
        path = find_path_between_intersections(107356, 29786);
        CHECK(path_is_legal(107356, 29786, path));
        CHECK(compute_path_travel_time(path) <= 26.07575930866577);
        
        path = find_path_between_intersections(8227, 51276);
        CHECK(path_is_legal(8227, 51276, path));
        CHECK(compute_path_travel_time(path) <= 18.87324392862305);
        
        path = find_path_between_intersections(107986, 67125);
        CHECK(path_is_legal(107986, 67125, path));
        CHECK(compute_path_travel_time(path) <= 7.94495555079255);
        
        //Generate random inputs
        std::vector<unsigned> intersection_ids1;
        std::vector<unsigned> intersection_ids2;
        for(size_t i = 0; i < 475; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(16075);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 475; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i]);
            }
        }
    } //find_path_between_intersections_perf_hard

} //inter_inter_path_perf_hard_public


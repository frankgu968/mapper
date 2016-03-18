#include <unittest++/UnitTest++.h>

#include "../src/mapProcessor.h"

std::string map_dir = "mapper/Maps/";

SUITE(M1_Public_Load_Maps) {
    TEST(load_saint_helena) {
        {
        	close_map();
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "saint_helena.streets.bin"));
        }
        close_map();
    }
    
    TEST(load_hamilton) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "hamilton_canada.streets.bin"));
        }
        close_map();
    }

    TEST(load_moscow) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "moscow.streets.bin"));
        }
        close_map();
    }

    TEST(load_toronto) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "toronto.streets.bin"));
        }
        close_map();
    }

    TEST(load_newyork) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "newyork.streets.bin"));
        }
        close_map();
    }

    TEST(load_golden_horseshoe) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "toronto_canada.streets.bin"));
        }
        close_map();
    }

    TEST(load_invalid_map_path) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(!load_map("/this/path/does/not/exist"));
        }
        close_map();
        load_map(map_dir + "toronto.streets.bin");
    }
}

#include <iostream>
#include <unittest++/UnitTest++.h>

#include "../src/mapProcessor.h"

/*
 * This is the main that drives running
 * unit tests.
 */
int main(int argc, char** argv) {
    //Load the test map
    std::string map_name = "mapper/Maps/toronto.streets.bin";
    if(!load_map(map_name)) {
        std::cout << "ERROR: Could not load map file: '" << map_name << "'!";
        std::cout << " Subsequent tests will likely fail." << std::endl;
        std::cout << "A common cause of this error is that the relative file location mapper/Maps/toronto.streets.bin is not valid for where you are launching this executable from." << std::endl;
        //Don't abort tests, since we still want to show that all
        //tests fail.
    }


    //Run the unit tests
    int num_failures = UnitTest::RunAllTests();

    //Cleanup
    close_map();

    return num_failures;
}

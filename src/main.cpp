
#include <mapProcessor.h>
#include "StreetsDatabaseAPI.h"
#include "mapperGraphicsInterface.h"
#include "Street.h"
#include "MapData.h"
#include "FeatureXY.h"

//using namespace vl;

int main(int argc, char** argv) {
	std::cout << "Enter the name of the city you want to load (e.g. toronto): ";
	std::string mapName;
	std::cin >> mapName;
    //mapName = "/cad2/ece297s/public/maps/" + mapName + ".streets.bin";
    ref<vlSDL::SDLWindow> mainWindow = new vlSDL::SDLWindow;	// Window object holder

    bool load_success = load_map("Maps/toronto.streets.bin");
    //bool load_success = load_map(mapName);
	if (!load_success) {
		std::cout << "load_map failed to load map using StreetsDatabaseAPI!" << std::endl;
		std::cout << "A common cause of this error is that the relative file location mapper/Maps/toronto.streets.bin is not valid for where you are launching this executable from." << std::endl;
	}
	else {
		// give the user instructions on how to use this program
        showHelpMessage();

		// Initialize graphics window and context
        initGraphics(mainWindow);
	}

	// Destroy any remaining entities
    mainWindow = NULL;
    VisualizationLibrary::shutdown();
	close_map();
    return 0;
}

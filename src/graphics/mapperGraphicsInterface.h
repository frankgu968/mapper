/*
 * mapperGraphicsInterface.hpp
 *
 *  Created on: Mar 2, 2016
 *      Author: root
 */

#ifndef MAPPERGRAPHICSINTERFACE_H_
#define MAPPERGRAPHICSINTERFACE_H_

#include <vlCore/VisualizationLibrary.hpp>
#include <vlSDL/SDLWindow.hpp>
#include "polygonRenderer.h"
#include "TerminalInput.h"

#define APPLET_NAME "Polygon Renderer"

// Concrete definition in polygonRenderer.cpp
namespace mapGraphics{
	extern int x;
	extern int y;
	extern double initCamX;
	extern double initCamY;
	extern double initCamZ;
	extern int width;
	extern int height;
	extern double translationSpeed;
	extern double zoom;
	extern double zoomInc;
	extern double zoomMax;
	extern bool enableLighting;
	extern bool enableOcclusion;
	extern bool enableMultisample;
	extern PointXY * clickedCoords;
}

void initGraphics(ref<vlSDL::SDLWindow> mainWindow);

// Initial function to load all the objects onto the map
void populateWorld();

// Function that is called whenever things need to be updated in the world
void updateWorld();

// Additions to the keyReleaseEvent callback in the polygonRenderer
void keyPress(EKey);

void centerCamera(PointXY);
void centerCamera(PointXY, double);

#endif /* MAPPERGRAPHICSINTERFACE_H_ */

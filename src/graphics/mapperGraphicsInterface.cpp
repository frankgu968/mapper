/*
 * mapperGraphicsInterface.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: frank
 */

#include "mapperGraphicsInterface.h"
#include "FeatureXY.h"
#include "MapData.h"
#include <iostream>

#define fuckedness 50

namespace mapGraphics{
	ref<polygonRenderer> app;
	// Window location
	int x = 0;
	int y = 0;

	// Window size
	int width = 1024;
	int height = 768;

	// Camera initial position
	double initCamX = 0;
	double initCamY = 0;
	double initCamZ = 1000;
	mat4 initialModelMat;

	// Camera controls
	double translationSpeed = 0.4;		// Translation speed with drag
	double zoom = initCamZ;				// Current zoom
	double zoomInc = 15;				// Zoom speed
	double zoomMax = 1;					// Maximum zoom level

	bool enableOcclusion = false;		// Enable occlusion culling
	bool enableLighting = false;;			// Enable dynamic lighting
	bool enableMultisample = false;

	PointXY * clickedCoords;

	FeatureXY** smallFeatures;
	FeatureXY** mediumSmallFeatures;
	FeatureXY** mediumLargeFeatures;
	FeatureXY** largeFeatures;

	bool highlightedDirty = false;
	bool enableHighlight = false;
	PointXY startXY;
	PointXY endXY;
	std::vector<unsigned int> highlightedSegs;
}

using namespace vl;					// Use the VisualizationLibrary namespace
using namespace mapGraphics;

void initGraphics(ref<vlSDL::SDLWindow> mainWindow){
	VisualizationLibrary::init();

	// begin ---- Set OpenGL Context ----
	OpenGLContextFormat format;		// Create an OpenGL context
	format.setDoubleBuffer(true);	// Enable double buffering
	format.setRGBABits(8,8,8,8);
	format.setDepthBufferBits(24);
	format.setStencilBufferBits(8);
	format.setFullscreen(false);

	if(mapGraphics::enableMultisample){
		format.setMultisampleSamples(4);
		format.setMultisample(true);
	}
	// end ---- Set OpenGL Context ----

	// Kick start our main application
	app = new polygonRenderer;

	app->initialize();

	// Load our graphics application into the main window
	mainWindow->addEventListener(app.get());
	app->rendering()->as<Rendering>()->renderer()->setFramebuffer(mainWindow->framebuffer());
	app->rendering()->as<Rendering>()->camera()->viewport()->setClearColor(white);
	vec3 eye = vec3(mapGraphics::initCamX,mapGraphics::initCamY,mapGraphics::initCamZ);
	vec3 center = vec3(0,0,0);
	vec3 up = vec3(0,1,0);
	mat4 view_at = mat4::getLookAt(eye, center, up);
	app->rendering()->as<Rendering>()->camera()->setViewMatrix(view_at);
	mapGraphics::initialModelMat = app->rendering()->as<Rendering>()->camera()->modelingMatrix();

	// Launch the window
	if(!mainWindow->initSDLWindow("Visualization Library polygon rendition test", format,
			mapGraphics::x, mapGraphics::y, mapGraphics::width, mapGraphics::height))
	{	std::cout << "Window initialization failure!" << std::endl; }

	// Initialize the event loop
	vlSDL::messageLoop();
}

void populateWorld(){
	// Initialize data containers
	using namespace mapGraphics;
	smallFeatures = MapData::instance()->getSmallFeatures();
	mediumSmallFeatures = MapData::instance()->getMediumSmallFeatures();
	mediumLargeFeatures = MapData::instance()->getMediumLargeFeatures();
	largeFeatures = MapData::instance()->getLargeFeatures();

	app->setWorldBounds(-5000,-5000,5000,5000);

	for(unsigned i = 0; i < MapData::instance()->getNumLargeFeatures(); i++){
		app->addFeature(largeFeatures[i],1);	// Layer this on the very top because it has lakes in parks
	}

	for(unsigned i = 0; i < MapData::instance()->getNumMediumLargeFeatures(); i++){
		app->addFeature(mediumLargeFeatures[i],2);
	}

	for(unsigned i = 0; i < MapData::instance()->getNumMediumSmallFeatures(); i++){
		app->addFeature(mediumSmallFeatures[i],3);
	}

	for(unsigned i = 0; i < MapData::instance()->getNumSmallFeatures(); i++){
		app->addFeature(smallFeatures[i],4);
	}

	for(unsigned i = 0; i < MapData::instance()->getNumStreetSegments();i++){
		StreetSegment * seg = MapData::instance()->getStreetSegments();
		//if((int)seg[i].getStreetType() == (int)RD_MOTORWAYR){
			app->addRoadSegment(seg[i],7);
		//}
	}

	String toWrite = "test string";
	app->addText(toWrite,0,0,0,8,white);

}

void updateWorld(){
	if(TerminalInterface::highlightedDirty){
		TerminalInterface::highlightedDirty = false;
		app->clearHighlightedSegments();
		for(unsigned i = 0; i < TerminalInterface::highlightedSegs.size();i++){
			StreetSegment * seg = MapData::instance()->getStreetSegments();
			app->addHighlightedRoadSegment(seg[TerminalInterface::highlightedSegs[i]],9);
		}
		app->registerHighlightedPath();

		app->startPoint->setLocalMatrix( mat4() );
		app->startPoint->translate(vec3(TerminalInterface::startXY.x,TerminalInterface::startXY.y,0));
		app->startPoint->computeWorldMatrix();

		app->endPoint->setLocalMatrix( mat4() );
		app->endPoint->translate(vec3(TerminalInterface::endXY.x,TerminalInterface::endXY.y,0));
		app->endPoint->computeWorldMatrix();
	}
}

void keyPress(EKey key){
	if(key == Key_Return){
		readFromTerminal();
	}
}

void centerCamera(PointXY pos, double zoom){
	vec3 eye = vec3(pos.x,pos.y,zoom);
	vec3 center = vec3(pos.x,pos.y,0);
	vec3 up = vec3(0,1,0);
	mat4 view_at = mat4::getLookAt(eye, center, up);
	app->rendering()->as<Rendering>()->camera()->setViewMatrix(view_at);

}

void centerCamera(PointXY pos){
	double z = app->rendering()->as<Rendering>()->camera()->modelingMatrix().getT().z();
	centerCamera(pos, z);
}

/*
 * polygonRender.h
 *
 *  Created on: Mar 3, 2016
 *      Author: frank
 *      PRODUCTION
 */

#ifndef POLYGONRENDERER_H_
#define POLYGONRENDERER_H_

#include <vlCore/VisualizationLibrary.hpp>
#include <vlCore/ResourceDatabase.hpp>
#include <vlCore/Time.hpp>

#include <vlGraphics/Effect.hpp>
#include <vlGraphics/Geometry.hpp>
#include <vlGraphics/Text.hpp>
#include <vlGraphics/Font.hpp>
#include <vlGraphics/FontManager.hpp>
#include <vlGraphics/Extrusion.hpp>
#include <vlGraphics/OcclusionCullRenderer.hpp>
#include <vlGraphics/RayIntersector.hpp>
#include <vlGraphics/Tessellator.hpp>
#include <vlGraphics/Applet.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/GeometryPrimitives.hpp>

#include <vector>

#include "Extruder.h"
#include "StreetSegment.h"
#include "TrackCameraManipulator.h"
#include "FeatureXY.h"

using namespace vl;

typedef enum{
	FX_COLOR,
	FX_BLEND
} EFx;

#define MAX_ROADS 6
typedef enum{
	RD_MOTORWAY,
	RD_PRIMARY,
	RD_SECONDARY,
	RD_TERTIARY,
	RD_UNCLASSIFIED,
	NOP
} ERoad;

#define MAX_FEATURES 11
typedef enum{
	FE_DEFAULT = 0,
	FE_PARK = 1,
	FE_BEACH = 2,
	FE_LAKE = 3,
	FE_BUILDING = 7,
	FE_GREENERY = 8,
	FE_GOLFCOURSE = 9,
	FE_STREAM = 10
} EFeature;

class polygonRenderer : public Applet {
public:
	// Override functions
	polygonRenderer();

	~polygonRenderer();
	void initEvent();		// Implement initialization
	void updateScene();		// Implement frame updates
	void keyReleaseEvent(unsigned short, EKey);	// Override the key press event handler for the windows
	void mouseUpEvent(EMouseButton, int x, int y);

	// Interface functions
	void addFeature(std::vector<std::pair<double, double> >, EFeature, unsigned int);
	void addFeature(FeatureXY *, int layer);
	void addRoadSegment(std::vector<std::pair<double, double> > coords, ERoad type, double width, unsigned int LOD);
	void addRoadSegment(StreetSegment &,double,int);
	void addRoadSegment(StreetSegment &,int);

	void registerHighlightedPath();
	void clearHighlightedSegments();
	void addHighlightedRoadSegment(StreetSegment &,int);
	void addText(String&, double, double, double, int, fvec4);

	void setWorldBounds(double, double, double, double);

	Transform * startPoint;
	Transform * endPoint;

protected:
	std::vector<Actor*> featureActors;	// Container of all the polygon actors
	std::vector<Actor*> roadActors;
	std::vector<Actor*> textActors;
	ref<Actor>	highlightedActor;
	ref<Actor> worldActor;
	ref<Actor> startPointActor;
	ref<Actor> endPointActor;
	Transform * mIntersectionPoint;

private:
	ref<Text> mText;
	std::vector<ref<Effect> > effects;
	std::vector<ref<Geometry> > featureGeometries;
	std::vector<ref<Geometry> > roadGeometries;
	ref<Geometry> highlightedPath;
	std::vector<ref<Tessellator> > featureTess;
	std::vector<ref<Tessellator> > roadTess;
	ref<Tessellator> highlightTess;
	std::vector<ref<Extruder> > extruder;
	Time mTimer;
	ref<OcclusionCullRenderer> mOcclusionRenderer;
	ERoad curRoadDrawType;
	double curRoadWidth;

	// Helper functions
	void initTessellator();
	void initExtruder();

	// Road extrusion silhouettes
	void silhouetteOneWay(double width, ref<Extruder>);
	fvec4 getStreetColor(ERoad type);
	fvec4 getFeatureColor(EFeature type);
};


#endif /* POLYGONRENDER_H_ */

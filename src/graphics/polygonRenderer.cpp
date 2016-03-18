/*
 * polygonRender.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: Frank
 *
 *      For mapper graphics
 */

#include "polygonRenderer.h"
#include "mapperGraphicsInterface.h"
#include <iostream>

using namespace vl;

polygonRenderer::polygonRenderer(){
	curRoadDrawType = NOP;
	curRoadWidth = 0.0;
}

/*
 * Defines the street colors
 * All color defintiions for streets should go here!
 */
fvec4 polygonRenderer::getStreetColor(ERoad type){
	switch(type){
	case RD_MOTORWAY:
		return fvec4(0.4471,0.7255,0.6706,1.0);
	case RD_PRIMARY:
		return fvec4(0.549,0.698,0.3373,1.0);
	case RD_SECONDARY:
		return fvec4(0.5216,0.5216,1.0,1.0);
	case RD_TERTIARY:
		return fvec4(1.0,0.9098,0.702,1.0);		//TODO
	case RD_UNCLASSIFIED:
		return fvec4(1.0,0.9098,0.702,1.0);		//TODO

	default:
		return fvec4(1.0,0.0,0.0,1.0);
	}
}

fvec4 polygonRenderer::getFeatureColor(EFeature type){
	switch(type){
	case FE_PARK:
		return fvec4(0.5961,0.9843,0.5961,1.0);

	case FE_BEACH:
		return fvec4(0.9333,0.9098,0.6667,1.0);

	case FE_LAKE:
		return fvec4(0.5333,0.8039,0.9804,1.0);

	case FE_BUILDING:
		return fvec4(0.7451,0.7451,0.7451,1.0);

	case FE_GREENERY:
		return fvec4(0.5961,0.9843,0.5961,1.0);

	case FE_GOLFCOURSE:
		return fvec4(0.5961,0.9843,0.5961,1.0);

	case FE_STREAM:
		return fvec4(0.5333,0.8039,0.9804,1.0);

	default:
		return fvec4(1.0,1.0,0.8784,1.0);

	}
}

polygonRenderer::~polygonRenderer(){
	if(this->featureActors.size() > 0)
	{
		this->featureActors.clear();
	}

	if(this->roadActors.size() > 0)
		{
			this->roadActors.clear();
		}

	if(this->effects.size() > 0){
		this->effects.clear();
	}

}

void polygonRenderer::initEvent() {

	// Set applet name
	this->setAppletName(APPLET_NAME);

	// Start the timer
	mTimer.start();

	// Initialize the tessellator properties
	this->initTessellator();
	this->initExtruder();


// BEGIN - Set camera manipulation bindings --------------------------------------

	ref<TrackballManipulator> cam = new TrackCameraManipulator;		// Create new manipulator with smart pointers
	openglContext()->removeEventListener(this->trackball());		// Remove the old trackball manipulator
	this->setTrackball(cam.get());									// Set the new Track Camera as the manipulator
	cam->setCamera(rendering()->as<Rendering>()->camera());			// Bind the scene camera to the manipulator
	cam->setTranslationButton(LeftButton);							// Reset the key bindings
	cam->setRotationButton(RightButton);
	cam->setZoomButton(MiddleButton);
	cam->setTranslationSpeed(mapGraphics::translationSpeed);									// Modify the speed
	openglContext()->addEventListener(cam.get());					// Register the new manipulator object

// END - Set camera manipulation bindings ----------------------------------------


// BEGIN: Set Effect shaders -----------------------------------------------------

	// color_fx - Color filled effect with lighting and field depth
	ref<Effect> color_fx = new Effect;
	if(mapGraphics::enableLighting){
		color_fx->shader()->setRenderState( new Light, 0 );	// *Adding light source does not impact FPS
		color_fx->shader()->enable(EN_LIGHTING);
		color_fx->shader()->enable(EN_DEPTH_TEST);
	}
	color_fx->shader()->enable(EN_ALPHA_TEST);
	color_fx->shader()->enable(EN_BLEND);
	color_fx->shader()->gocMaterial()->setColorMaterialEnabled(true);	// Enable color material to visualize the generated vertex colors.
	this->effects.push_back(color_fx);

	// blend_fx - Enable blend post-processing for text
	ref<Effect> blend_fx = new Effect;
	blend_fx->shader()->enable(EN_BLEND);
	this->effects.push_back(blend_fx);

// END: Set Effect shaders -------------------------------------------------------

// BEGIN - Set OpenGL accelerated occlusion culling ------------------------------------------

	// Check if hardware support is enabled
	if(mapGraphics::enableOcclusion){
		if(!Has_Occlusion_Query)
		{
			Log::error("No support to hardware occlusion culling found... Occlusion culling is disabled! \n");
		}
		else {
			Renderer* regular_renderer = rendering()->as<vl::Rendering>()->renderer();
			mOcclusionRenderer = new vl::OcclusionCullRenderer;
			mOcclusionRenderer->setWrappedRenderer( regular_renderer );
			rendering()->as<vl::Rendering>()->setRenderer( mOcclusionRenderer.get());

			// Occulusion statistic logging
			mText = new Text;
			mText->setText("*** N/A ***");
			mText->setFont( defFontManager()->acquireFont("/home/frank/font/bitstream-vera/VeraMono.ttf", 10) );
			mText->setAlignment( AlignLeft | AlignTop );
			mText->setViewportAlignment( AlignLeft | AlignTop );
			mText->setTextAlignment(TextAlignLeft);
			mText->translate(+5,-5,0);
			mText->setColor(white);
			Actor* text_actor = sceneManager()->tree()->addActor(mText.get(), this->effects[FX_BLEND].get());
			text_actor->setOccludee(false);
		}
	}

// END - Set OpenGL accelerated occlusion culling --------------------------------------------

	// Add the geometry actor
	populateWorld();

	for(int i = 0; i < MAX_FEATURES; i++){

		this->featureGeometries.push_back( new Geometry);		// The geometry container
		ref<ArrayFloat3> tess_va = new ArrayFloat3;		// The vertex array for the geometry container
		featureGeometries[i]->setVertexArray(tess_va.get());
		tess_va->initFrom(featureTess[i]->tessellatedTris());

		if(tess_va->size()>0){
			featureGeometries[i]->setColorArray(getFeatureColor((EFeature)i));
			featureGeometries[i]->drawCalls()->push_back(new DrawArrays(PT_TRIANGLES, 0, (int)tess_va->size()));
			Actor * polygon_temp = sceneManager()->tree()->addActor(new Actor(NULL, effects[FX_COLOR].get(), new Transform));
			rendering()->as<Rendering>()->transform()->addChild(polygon_temp->transform());

			polygon_temp->setLod(0, featureGeometries[i].get());	// Set the geometry to appear at a particular LOD
			this->featureActors.push_back(polygon_temp);
		}
	}

	for(int i= 0; i < MAX_ROADS;i++){
		this->roadGeometries.push_back( new Geometry);		// The geometry container
		ref<ArrayFloat3> tess_va = new ArrayFloat3;		// The vertex array for the geometry container
		roadGeometries[i]->setVertexArray(tess_va.get());
		tess_va->initFrom(roadTess[i]->tessellatedTris());

		if(tess_va->size()>0){
			roadGeometries[i]->setColorArray(getStreetColor((ERoad)i));
			roadGeometries[i]->drawCalls()->push_back(new DrawArrays(PT_TRIANGLES, 0, (int)tess_va->size()));
			Actor * road_temp = sceneManager()->tree()->addActor(new Actor(NULL, effects[FX_COLOR].get(), new Transform));
			rendering()->as<Rendering>()->transform()->addChild(road_temp->transform());

			road_temp->setLod(0, roadGeometries[i].get());	// Set the geometry to appear at a particular LOD
			this->roadActors.push_back(road_temp);
		}
	}

	/*
	for(int i =0; i < MAX_ROADS; i++){
		if(!extruder[i]->getError()){
			ref<Geometry> extrusionGeometry = extruder[i]->exportGeometry()->deepCopy();
			extrusionGeometry->setColorArray(getStreetColor((ERoad)i));
			Actor * _extrusion = sceneManager()->tree()->addActor(NULL, this->effects[FX_COLOR].get(), NULL);
			_extrusion->setLod(0, extrusionGeometry.get());
			this->roadActors.push_back(_extrusion);
			extruder[i]->clear();
		}
	}
	*/

	ref<Geometry> intersection_point_geom = makeUVSphere(vec3(0,0,0), 1.0f);
	intersection_point_geom->computeNormals();
	intersection_point_geom->setColorArray(fvec4(0.0,0,1,0.5));
	Actor* intersection_point_act = sceneManager()->tree()->addActor( intersection_point_geom.get(), this->effects[FX_COLOR].get(), new Transform );
	mIntersectionPoint = intersection_point_act->transform();

	ref<Geometry> start_geom = makeUVSphere(vec3(0,0,0), 1.0f);
	start_geom->computeNormals();
	start_geom->setColorArray(fvec4(0,1,0,0.5));
	this->startPointActor = sceneManager()->tree()->addActor( start_geom.get(), this->effects[FX_COLOR].get(), new Transform );
	startPoint = startPointActor->transform();

	ref<Geometry> stop_geom = makeUVSphere(vec3(0,0,0), 1.0f);
	stop_geom->computeNormals();
	stop_geom->setColorArray(fvec4(0.0,1,0,0.5));
	this->endPointActor = sceneManager()->tree()->addActor( stop_geom.get(), this->effects[FX_COLOR].get(), new Transform );
	endPoint = endPointActor->transform();
}

/*
 * Initializes the tessellator
 */
void polygonRenderer::initTessellator(){

	for(int i = 0; i < MAX_FEATURES;i++){
		featureTess.push_back(new Tessellator);
		featureTess[i]->setTessNormal(fvec3(0,0,1));		// Set the normal director to the plane of tessellation
		featureTess[i]->setBoundaryOnly(false);
		featureTess[i]->setTolerance(0.0);
		featureTess[i]->setTessellateIntoSinglePolygon(true);
	}

	// One more to account for the roads not in classification
	for(int i = 0; i < MAX_ROADS+1;i++){
		roadTess.push_back(new Tessellator);
		roadTess[i]->setTessNormal(fvec3(0,0,1));		// Set the normal director to the plane of tessellation
		roadTess[i]->setBoundaryOnly(false);
		roadTess[i]->setTolerance(0.0);
		roadTess[i]->setTessellateIntoSinglePolygon(true);
	}

	highlightTess = new Tessellator;
	highlightTess->setTessNormal(fvec3(0,0,1));		// Set the normal director to the plane of tessellation
	highlightTess->setBoundaryOnly(false);
	highlightTess->setTolerance(0.0);
	highlightTess->setTessellateIntoSinglePolygon(true);
}

void polygonRenderer::initExtruder(){
	// Setup the extrusion options.
	for(int i = 0; i < MAX_ROADS; i++)
	{
		extruder.push_back(new Extruder);
		extruder[i]->setSilhouetteMode(SilhouetteClosed);
		extruder[i]->setSmooth(true);
		extruder[i]->setFillBottom(true); // start of the extrusion is closed
		extruder[i]->setFillTop(true);    // end of the extrusion is closed
		extruder[i]->init();
		this->curRoadDrawType = NOP;
	}
}

void polygonRenderer::updateScene() {
	/*
	 * Polygon display steps
	 * 1. Set the context and initialize everything
	 * 2. Define the geometrical shape in the coordinate world
	 * 3. Run the tessellation to obtain a series of triangle vertex points
	 * 4. Issue OpenGL call to render the Vertex Array Objects
	 */

	// FPS Counter
	if(mTimer.elapsed() > 2)
	{
		mTimer.start();
		openglContext()->setWindowTitle( vl::Say("[%.1n] %s") << fps() << appletName());
	}

	// DEBUG: print out occlusion ratio statistics
	if(mapGraphics::enableOcclusion){
		String msg = Say("Occlusion ratio = %.1n%% (%n/%n)\n")
			<< 100.0f * mOcclusionRenderer->statsOccludedObjects() / mOcclusionRenderer->statsTotalObjects()
			<< mOcclusionRenderer->statsTotalObjects() - mOcclusionRenderer->statsOccludedObjects()
			<< mOcclusionRenderer->statsTotalObjects();
		mText->setText( msg );
	}

	// Dynamically enable or disable lighting to switch between 2D/3D effect
	if(mapGraphics::enableLighting){
		this->effects[FX_COLOR]->shader()->setRenderState( new Light, 0 );	// *Adding light source does not impact FPS
		this->effects[FX_COLOR]->shader()->enable(EN_LIGHTING);
		this->effects[FX_COLOR]->shader()->enable(EN_DEPTH_TEST);
	}
	else{
		this->effects[FX_COLOR]->shader()->disable(EN_LIGHTING);
		this->effects[FX_COLOR]->shader()->disable(EN_DEPTH_TEST);
	}

	updateWorld();

}

// Tested!
void polygonRenderer::keyReleaseEvent(unsigned short, EKey key){
	if(key == Key_Escape){
		openglContext()->quitApplication();
	}
	if(key == Key_Space){
		mapGraphics::enableLighting = !mapGraphics::enableLighting;
	}

	keyPress(key);
}

/*
 * Add a polygon to the rendition tree
 * Arguments:
 * - coords: a vector of pairs of x, y double precision coordinates
 * - color: 4 dimensional vector (R,G,B,ALPHA)
 * - lod: Level of Detail to bind polygon to
 */
/*
 * Add any complex polygon to the draw tree
 * Approach:
 * 1. Clear the tessellator contours and contour vertices
 * 2. Push the new coordinates into tessellator
 * 3. Tessellate the polygon
 * 4. Create new actor and push into sceneManager and actor container
 * 5. Bind the vertices of tessellation to the actor polygon
 */

void polygonRenderer::addFeature(FeatureXY * feature, int layer){

	int tessIdx = feature->getType();
	if(tessIdx == 10)
	{
		// Process streams display here
		return;
	}
	else {
		featureTess[tessIdx]->contours().clear();
		featureTess[tessIdx]->contours().push_back(feature->getNumCoordinates());
		featureTess[tessIdx]->contourVerts().clear();
		for(unsigned int i = 0; i < feature->getNumCoordinates(); i++){
			featureTess[tessIdx]->contourVerts().push_back(dvec3((feature->getDrawCoordinates()[i].x), (feature->getDrawCoordinates()[i].y), layer*0.01));
		}
		featureTess[tessIdx]->tessellate(true);
	}
}

void polygonRenderer::addRoadSegment( std::vector<std::pair<double, double> > curvePoints, ERoad type,
		double width, unsigned int LOD){
	//TODO
	// Change the road type
	// Will check if the extruder silhouette has to be changed depending on road type
	int extrIdx = (int)type;

	switch(type){
		case RD_MOTORWAY:
			if(curRoadDrawType != RD_MOTORWAY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_MOTORWAY]);
				curRoadWidth = width;
				curRoadDrawType = RD_MOTORWAY;
			}
			break;

		case RD_PRIMARY:
			if(curRoadDrawType != RD_PRIMARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_PRIMARY]);
				curRoadDrawType = RD_PRIMARY;
				curRoadWidth = width;
			}
			break;
		case RD_SECONDARY:
			if(curRoadDrawType != RD_SECONDARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_SECONDARY]);
				curRoadWidth = width;
				curRoadDrawType = RD_SECONDARY;
			}
			break;

		case RD_TERTIARY:
			if(curRoadDrawType != RD_TERTIARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_TERTIARY]);
				curRoadWidth = width;
				curRoadDrawType = RD_TERTIARY;
			}
			break;

		case RD_UNCLASSIFIED:
			if(curRoadDrawType != RD_UNCLASSIFIED || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_UNCLASSIFIED]);
				curRoadWidth = width;
				curRoadDrawType = RD_UNCLASSIFIED;
			}
			break;
	}

	extruder[extrIdx]->positionPath().clear();	// Clear the residual path information

	// Push all curve points into extrusion container
	// First and last point define the orientation of the extrusion
	extruder[extrIdx]->positionPath().push_back(fvec3(curvePoints[0].first, curvePoints[0].second,0.01));
	for(unsigned int i = 0; i < curvePoints.size(); i++){
		extruder[extrIdx]->positionPath().push_back(fvec3(curvePoints[i].first, curvePoints[i].second,0.01));
	}
	extruder[extrIdx]->positionPath().push_back(fvec3(curvePoints[curvePoints.size()-1].first, curvePoints[curvePoints.size()-1].second,0.01));

	extruder[extrIdx]->extrude();

}

void polygonRenderer::addRoadSegment(StreetSegment & segment, int layer){
	int tessIdx = (int)segment.getStreetType();
	for(unsigned int i = 0; i < segment.getNumRectangles(); i++){
		roadTess[tessIdx]->contours().clear();
		roadTess[tessIdx]->contours().push_back(4);
		roadTess[tessIdx]->contourVerts().clear();
		roadTess[tessIdx]->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->a.x),
				(segment.getRectangles()[i]->a.y), layer*0.01));
		roadTess[tessIdx]->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->b.x),
					(segment.getRectangles()[i]->b.y), layer*0.01));
		roadTess[tessIdx]->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->c.x),
					(segment.getRectangles()[i]->c.y), layer*0.01));
		roadTess[tessIdx]->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->d.x),
					(segment.getRectangles()[i]->d.y), layer*0.01));
		roadTess[tessIdx]->tessellate(true);

	}
}

void polygonRenderer::addHighlightedRoadSegment(StreetSegment & segment, int layer){
	for(unsigned int i = 0; i < segment.getNumRectangles(); i++){
		highlightTess->contours().clear();
		highlightTess->contours().push_back(4);
		highlightTess->contourVerts().clear();
		highlightTess->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->a.x),
				(segment.getRectangles()[i]->a.y), layer*0.01));
		highlightTess->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->b.x),
					(segment.getRectangles()[i]->b.y), layer*0.01));
		highlightTess->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->c.x),
					(segment.getRectangles()[i]->c.y), layer*0.01));
		highlightTess->contourVerts().push_back( dvec3( (segment.getRectangles()[i]->d.x),
					(segment.getRectangles()[i]->d.y), layer*0.01));
		highlightTess->tessellate(true);

	}
}

void polygonRenderer::clearHighlightedSegments(){
	this->highlightTess->contours().clear();
	this->highlightTess->contourVerts().clear();
	this->highlightTess->tessellatedTris().clear();

}

void polygonRenderer::addRoadSegment(StreetSegment & segment, double width, int layer){
	int extrIdx = (int)segment.getStreetType();

	switch(extrIdx){
		case RD_MOTORWAY:
			if(curRoadDrawType != RD_MOTORWAY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_MOTORWAY]);
				curRoadWidth = width;
				curRoadDrawType = RD_MOTORWAY;
			}
			break;

		case RD_PRIMARY:
			if(curRoadDrawType != RD_PRIMARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_PRIMARY]);
				curRoadDrawType = RD_PRIMARY;
				curRoadWidth = width;
			}
			break;
		case RD_SECONDARY:
			if(curRoadDrawType != RD_SECONDARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_SECONDARY]);
				curRoadWidth = width;
				curRoadDrawType = RD_SECONDARY;
			}
			break;

		case RD_TERTIARY:
			if(curRoadDrawType != RD_TERTIARY || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_TERTIARY]);
				curRoadWidth = width;
				curRoadDrawType = RD_TERTIARY;
			}
			break;

		case RD_UNCLASSIFIED:
			if(curRoadDrawType != RD_UNCLASSIFIED || curRoadWidth != width)
			{
				silhouetteOneWay(width, extruder[RD_UNCLASSIFIED]);
				curRoadWidth = width;
				curRoadDrawType = RD_UNCLASSIFIED;
			}
			break;
	}

	extruder[extrIdx]->positionPath().clear();	// Clear the residual path information

	// Push all curve points into extrusion container
	// First and last point define the orientation of the extrusion
	extruder[extrIdx]->positionPath().push_back(fvec3(segment.getDrawCurvePoints()[0].x, segment.getDrawCurvePoints()[0].y, layer * 0.01));
	for(unsigned int i = 0; i < segment.getDrawCurvePoints().size(); i++){
		extruder[extrIdx]->positionPath().push_back(fvec3(segment.getDrawCurvePoints()[i].x, segment.getDrawCurvePoints()[i].y,layer * 0.01));
	}
	extruder[extrIdx]->positionPath().push_back(fvec3(segment.getDrawCurvePoints()[segment.getDrawCurvePoints().size()-1].x,
			segment.getDrawCurvePoints()[segment.getDrawCurvePoints().size()-1].y,layer * 0.01));

	extruder[extrIdx]->extrude();
}

void polygonRenderer::addText(String& text, double offsetX, double offsetY, double rotation, int size, fvec4 color){
	ref<Text> tempText = new Text;
	tempText->setFont( defFontManager()->acquireFont("/home/frank/font/bitstream-vera/Vera.ttf", size, true) );
	tempText->rotate(rotation,0,0,1);
	tempText->translate(offsetX,offsetY,0);
	tempText->setColor(color);
	tempText->setText(text);
	tempText->setKerningEnabled(true);
	tempText->setMode(Text3D);		// Set text mode for 3D manipulation

	Actor * _text = sceneManager()->tree()->addActor(new Actor(tempText.get(), this->effects[FX_BLEND].get(), new Transform));
	this->textActors.push_back(_text);
	rendering()->as<Rendering>()->transform()->addChild(_text->transform());
}

/*
 * Silhouette definition for a ONE WAY street
 */
void polygonRenderer::silhouetteOneWay(double width, ref<Extruder> _extruder){

	/*  a
	 * ----                 ----
	 * |  | b     0         |  |
	 * |  |_________________|  | height
	 * |                       |
	 * ------------------------- offset-> Prevents Z fighting
	 *          width
	 *
	 * _________________________   0
	 */

	double a = 0.1;
	double b = 0.1;
	double height = 0.2;
	_extruder->silhouette().clear();
	_extruder->silhouette().push_back(fvec2(-width / 2,height));
	_extruder->silhouette().push_back(fvec2(-width / 2,0));
	_extruder->silhouette().push_back(fvec2(width / 2,0));
	_extruder->silhouette().push_back(fvec2(width / 2,height));
	_extruder->silhouette().push_back(fvec2(width / 2 - a,height));
	_extruder->silhouette().push_back(fvec2(width / 2 - a,height - b));
	_extruder->silhouette().push_back(fvec2(-(width / 2 - a),height - b));
	_extruder->silhouette().push_back(fvec2(-(width / 2 - a),height));
}

void polygonRenderer::mouseUpEvent(EMouseButton, int x, int y)
{
	Camera* camera = rendering()->as<Rendering>()->camera();

	// convert Y coordinates to the OpenGL conventions
	y = openglContext()->height() - y;
	// compute the ray passing through the selected pixel
	Ray ray = camera->computeRay(x,y);
	// instance our ray-intersector
	RayIntersector intersector;
	// compute a frustum along the ray to accelerate the intersection test
	intersector.setFrustum( camera->computeRayFrustum( x,y ) );
	intersector.actors()->clear();
	intersector.actors()->push_back(this->worldActor.get());
	// set the intersecting ray
	intersector.setRay(ray);
	// run intersection test
	intersector.intersect();

	// inspect our intersections, the intersections returned are sorted according to their distance, the first one is the closest.
	if (intersector.intersections().size())
	{
		// highlight the intersection point by moving the green sphere there
		mIntersectionPoint->setLocalMatrix( mat4() );
		mIntersectionPoint->translate( intersector.intersections()[0]->intersectionPoint() );
		mIntersectionPoint->computeWorldMatrix();

		// print the name of the picked object
		//Log::print( Say("Intersections detected = %n (%s).\n") << intersector.intersections().size() << intersector.intersections()[0]->actor()->objectName() );
		if(mapGraphics::clickedCoords != NULL)
		{
			delete mapGraphics::clickedCoords;
		}

		mapGraphics::clickedCoords = new PointXY(intersector.intersections()[0]->intersectionPoint().x(),
				intersector.intersections()[0]->intersectionPoint().y());
	}
}

void polygonRenderer::registerHighlightedPath(){
	this->highlightedPath = new Geometry;		// The geometry container
	ref<ArrayFloat3> tess_va = new ArrayFloat3;		// The vertex array for the geometry container
	highlightedPath->setVertexArray(tess_va.get());
	tess_va->initFrom(highlightTess->tessellatedTris());

	if(tess_va->size()>0){
		highlightedPath->setColorArray(fvec4(1.0,0,0,1));
		highlightedPath->drawCalls()->push_back(new DrawArrays(PT_TRIANGLES, 0, (int)tess_va->size()));
		highlightedActor = sceneManager()->tree()->addActor(new Actor(NULL, effects[FX_COLOR].get(), new Transform));
		rendering()->as<Rendering>()->transform()->addChild(highlightedActor->transform());

		highlightedActor->setLod(0, highlightedPath.get());	// Set the geometry to appear at a particular LOD
	}
}

void polygonRenderer::setWorldBounds(double lowX, double lowY, double hiX, double hiY){
	ref<Geometry> world = new Geometry;
	ref<ArrayFloat3> tess_va = new ArrayFloat3;
	world->setVertexArray(tess_va.get());
	ref<Tessellator> worldTess = new Tessellator;
	worldTess->setTessNormal(fvec3(0,0,1));		// Set the normal director to the plane of tessellation
	worldTess->setBoundaryOnly(false);
	worldTess->setTolerance(0.0);
	worldTess->setTessellateIntoSinglePolygon(true);
	worldTess->contours().push_back(4);
	worldTess->contourVerts().push_back(dvec3(lowX, lowY, -1));
	worldTess->contourVerts().push_back(dvec3(lowX, hiY, -1));
	worldTess->contourVerts().push_back(dvec3(hiX, hiY, -1));
	worldTess->contourVerts().push_back(dvec3(hiX, lowY, -1));
	worldTess->tessellate();
	tess_va->initFrom(worldTess->tessellatedTris());

	world->setColorArray(fvec4(1.0,0,0,0.0));
	world->drawCalls()->push_back(new DrawArrays(PT_TRIANGLES, 0, (int)tess_va->size()));
	this->worldActor = sceneManager()->tree()->addActor(new Actor(NULL, effects[FX_COLOR].get(), new Transform));
	rendering()->as<Rendering>()->transform()->addChild(this->worldActor->transform());
	this->worldActor->setLod(0, world.get());
}


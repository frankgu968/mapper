/*
 * TrackCameraManipulator.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: frank
 *
 *      Implements a camera that provides panning, zoom, and rotation
 */

#include "TrackCameraManipulator.h"
#include "mapperGraphicsInterface.h"
#include <iostream>

// Empty constructor
TrackCameraManipulator::TrackCameraManipulator(){}

// Any mouse CLICK; mouse wheel scrolls are not handled here!
void TrackCameraManipulator::mouseDownEvent(EMouseButton btn, int x, int y){
	if ( camera() == NULL && mode() != NoMode)
	{	return;	}

	// enter new mode
	if (btn == rotationButton())
	{	mMode = RotationMode;	}
	else if (btn == translationButton())
	{	mMode = TranslationMode;	}

	VL_CHECK(openglContext()->framebuffer())

	// set x/y relative to the viewport
	x -= camera()->viewport()->x();
	y -= openglContext()->framebuffer()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

	// check that the click is in the viewport
	int w = camera()->viewport()->width();
	int h = camera()->viewport()->height();

	// Check out of bounds condition
	if (x<0 || y<0 || x>=w || y>=h)
	{	return;	}

	mMouseStart.x() = x;
	mMouseStart.y() = y;

	if (mTransform)
	{
		mStartMatrix = mTransform->localMatrix();
		mPivot = mStartMatrix.getT();
	}
	else
	{	mStartMatrix = camera()->modelingMatrix();	}

	mStartCameraPos = camera()->modelingMatrix().getT();
	mStartPivot = mPivot;
}

void TrackCameraManipulator::mouseMoveEvent(int x, int y){
	// Ignore event - No mode selected or camera object is not set up properly
	if ( camera() == NULL && mode() == NoMode)
	{	return;	}

	VL_CHECK(openglContext()->framebuffer())
	// set x/y relative to the top/left corner of the viewport
	x -= camera()->viewport()->x();
	y -= openglContext()->framebuffer()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

	if (mode() == RotationMode)
	{
		if (mTransform)
		{
		  mTransform->setLocalMatrix( mat4::getTranslation(mPivot) * trackballRotation(x,y) * mat4::getTranslation(-mPivot) * mStartMatrix );
		  mTransform->computeWorldMatrix();
		  mStartMatrix = mTransform->localMatrix();
		}
		else
		{
		  camera()->setModelingMatrix( mat4::getTranslation(mPivot) * trackballRotation(x,y) * mat4::getTranslation(-mPivot) * mStartMatrix );
		  mStartMatrix = camera()->modelingMatrix();
		}

		// Update x y variables with the current mouse position
		mMouseStart.x() = x;
		mMouseStart.y() = y;
	}
	else if (mode() == TranslationMode)
	{
		float tx = (mMouseStart.x() - x)  / 400.0f;
		float ty = -(mMouseStart.y() - y) / 400.0f;
		tx *= translationSpeed();
		ty *= translationSpeed();
		real distance = (mStartCameraPos - mPivot).length();
		vec3 up    = camera()->modelingMatrix().getY();
		vec3 right = camera()->modelingMatrix().getX();
		mat4 m = camera()->modelingMatrix();
		m.setT(mStartCameraPos + up*distance*ty + right*distance*tx);
		camera()->setModelingMatrix(m);
		mPivot = mStartPivot + up*distance*ty + right*distance*tx;
	}

	// update the view
	openglContext()->update();
}

//TODO: Key press event


void TrackCameraManipulator::mouseWheelEvent(int val){
	/*
	 * Zoom function
	 * Approach:
	 * 1. Get current camera position
	 * 2. Add/subtract z-coordinate defined by zoomInc to move camera towards/away from scene
	 * 3. Update the camera position and zoom
	 */
	using namespace mapGraphics;

	fvec3 camera_pos;
	mStartCameraPos = camera()->modelingMatrix().getT();
	if(val == 1) {
		camera_pos = mStartCameraPos - vec3(0,0,zoomInc);
	}
	else if (val == -1) {
		camera_pos = mStartCameraPos + vec3(0,0,zoomInc);
	}

	zoom = camera_pos.z();
	mat4 m = camera()->modelingMatrix();
	m.setT(camera_pos);
	camera()->setModelingMatrix(m);
}

void TrackCameraManipulator::setStartPos(vec3 newPos){
	this->mStartCameraPos = newPos;
}

// TODO: A mathematical function to control the zoom speed an factor depending on zoom
double TrackCameraManipulator::zoomFunc(double _zoom, double _zoomMax){
	return -1 / _zoom - _zoomMax;
}

/*
 * TrackCameraManipulator.hpp
 *
 *  Created on: Mar 8, 2016
 *      Author: frank
 *      Allows the user to freely manipulate the camera with intuitive mouse controls
 */

#ifndef TRACKCAMERAMANIPULATOR_H_
#define TRACKCAMERAMANIPULATOR_H_

#include <vlGraphics/GhostCameraManipulator.hpp>
#include <vlGraphics/TrackballManipulator.hpp>

using namespace vl;

class TrackCameraManipulator : public TrackballManipulator{
public:
	TrackCameraManipulator();
	void mouseDownEvent(EMouseButton, int, int);
	void mouseWheelEvent(int);
	void mouseMoveEvent(int, int);
	void setStartPos(vec3);

private:
	double zoomFunc(double, double);

};



#endif /* TRACKCAMERAMANIPULATOR_H_ */

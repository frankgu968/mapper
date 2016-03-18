/*
 * Extruder.h
 *
 *  Created on: Mar 12, 2016
 *      Author: frank
 */

#ifndef EXTRUDER_H_
#define EXTRUDER_H_

#include <vlCore/VisualizationLibrary.hpp>
#include <vlGraphics/Extrusion.hpp>
#include <vlGraphics/Tessellator.hpp>

using namespace vl;

class Extruder : public Extrusion {
public:
	void extrude();
	void clear();
	void init();
	ref<Geometry> exportGeometry();
	bool getError();

private:
	std::vector<fvec3> vertices;
	ref<Geometry> geom;
	ref<DrawElementsUInt> de;
	unsigned int currPos;
	unsigned int currDE;
	bool errFlag;

};



#endif /* EXTRUDER_H_ */

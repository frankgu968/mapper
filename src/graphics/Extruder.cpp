/*
 * Extruder.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: frank
 */

#include "Extruder.h"
#include <iostream>

void Extruder::init(){
	this->geom = new Geometry;
	de = new DrawElementsUInt(PT_QUADS);
	this->errFlag = false;
}

void Extruder::extrude(){
	if (silhouette().empty())
	{
		Log::error("Extrusion::extrude(): no silhouette defined.\n");
		this->errFlag = true;
		return ;
	}
	if (positionPath().empty())
	{
		Log::error("Extrusion::extrude() needs at least a non empty positionPath().\n");
		this->errFlag = true;
		return ;
	}
	if (!scalingPath().empty() && scalingPath().size() != positionPath().size()-2)
	{
		Log::error("Extrusion::extrude(): scalingPath() must have the same number of control points as positionPath().\n");
		this->errFlag = true;
		return ;
	}
	if (!rotationPath().empty() && rotationPath().size() != positionPath().size()-2)
	{
		Log::error("Extrusion::extrude(): rotationPath() must have the same number of control points as positionPath().\n");
		this->errFlag = true;
		return ;
	}
	if (!colorPath().empty() && colorPath().size() != positionPath().size()-2)
	{
		Log::error("Extrusion::extrude(): colorPath() must have the same number of control points as positionPath().\n");
		this->errFlag = true;
		return ;
	}

	size_t segments = positionPath().size()-2;
	currPos = vertices.size();

	vertices.resize( currPos + silhouette().size() * segments );

	vl::fmat4 m = fmat4::getRotation(fvec3(0,1,0),positionPath()[1]-positionPath()[0]);

	// initialize silhouette on the x/z plane
	std::vector<vl::fvec3> projected_sil;
	projected_sil.resize(silhouette().size());
	for(unsigned i=0; i<silhouette().size(); ++i)
	{
		projected_sil[i] = m * vl::fvec3(silhouette()[i].x(),0,silhouette()[i].y()) + positionPath()[0];
	}

	// initialize plane normals from 1 to n-1 (end points are excluded)
	std::vector<fvec3> plane_normals;
	plane_normals.resize(positionPath().size());
	for(unsigned i=1; i<plane_normals.size()-1; ++i)
	{
		fvec3 p0 = positionPath()[i-1] - positionPath()[i];
		fvec3 p1 = positionPath()[i+1] - positionPath()[i];
		p0.normalize();
		p1.normalize();
		plane_normals[i] = (p1-p0).normalize();
	}

	for(unsigned i=1; i<positionPath().size()-1; ++i)
	{
		for(int j=0; j<(int)silhouette().size(); ++j)
		{
		  fvec3 V = (positionPath()[i] - positionPath()[i-1]).normalize();
		  const fvec3& P = projected_sil[j];
		  const fvec3& orig = positionPath()[i];
		  const fvec3& N = plane_normals [i];
		  float d = dot(N,orig);
		  float t = dot(V,N) ? (d-dot(P,N))/dot(V,N) : 0;
		  // project current projected_sil on next plane along p0->p1 vector
		  vertices.at(currPos+j+silhouette().size()*(i-1)) = projected_sil[j] = P + V*t;
		}
	}


	// rotation
	if(!rotationPath().empty())
	{
		for(unsigned i=1; i<positionPath().size()-1; ++i)
		{
			fvec3 r = (positionPath()[i+1] - positionPath()[i]).normalize();
			fmat4 mat = vl::fmat4::getRotation(rotationPath()[i-1],r);
			fvec3 c;
			for(int j=0; j<(int)silhouette().size(); ++j)
			c += vertices.at(currPos+j+silhouette().size()*(i-1));
			c /= (float)silhouette().size();
			for(int j=0; j<(int)silhouette().size(); ++j)
			vertices.at(currPos+j+silhouette().size()*(i-1)) = (mat*(vertices.at(currPos+j+silhouette().size()*(i-1))-c))+c;
		}
	}

	// scaling
	if(!scalingPath().empty())
	{
		for(unsigned i=1; i<positionPath().size()-1; ++i)
		{
			float s = scalingPath()[i-1];
			fvec3 c;
			for(int j=0; j<(int)silhouette().size(); ++j)
			c += vertices.at(currPos+j+silhouette().size()*(i-1));
			c /= (float)silhouette().size();
			for(int j=0; j<(int)silhouette().size(); ++j)
			vertices.at(currPos+j+silhouette().size()*(i-1)) = (s*(vertices.at(currPos+j+silhouette().size()*(i-1))-c))+c;
		}
	}

	int prof_count = silhouetteMode() == SilhouetteClosed ? (int)silhouette().size() : (int)silhouette().size()-1;
	currDE = de->indexBuffer()->size();
	de->indexBuffer()->resize(currDE + 4 * prof_count * (segments-1));
	for(size_t iseg=0; iseg<segments-1; ++iseg)
	{
		for(int iquad=0; iquad<prof_count; ++iquad)
		{
			de->indexBuffer()->at(currDE + iquad*4+iseg*4*prof_count + 3) = currPos +  (iseg + 0) * (GLuint)silhouette().size() + iquad;
			de->indexBuffer()->at(currDE + iquad*4+iseg*4*prof_count + 2) = currPos +(iseg + 0) * (GLuint)silhouette().size() + (iquad+1)%silhouette().size();
			de->indexBuffer()->at(currDE + iquad*4+iseg*4*prof_count + 1) = currPos +(iseg + 1) * (GLuint)silhouette().size() + (iquad+1)%silhouette().size();
			de->indexBuffer()->at(currDE + iquad*4+iseg*4*prof_count + 0) = currPos +(iseg + 1) * (GLuint)silhouette().size() + iquad;
		}
	}

	// bottom/top caps

	size_t tess_bottom_count = 0;
	size_t tess_top_count    = 0;

	if(fillBottom())
	{
		size_t start = vertices.size();
		Tessellator tessellator;
		tessellator.contours().push_back((int)silhouette().size());
		for(unsigned i=0; i<silhouette().size(); ++i){
			tessellator.contourVerts().push_back((dvec3)vertices[currPos+i]);
		}
		tessellator.setWindingRule(vl::TW_TESS_WINDING_NONZERO);
		tessellator.tessellate();
		for(unsigned i=0; i<tessellator.tessellatedTris().size(); ++i){
			vertices.push_back(tessellator.tessellatedTris()[i]);
		}
		if (tessellator.tessellatedTris().size()){
			geom->drawCalls()->push_back( new DrawArrays(PT_TRIANGLES, start, tessellator.tessellatedTris().size()) );
		}tess_bottom_count = tessellator.tessellatedTris().size();
	}
	if(fillTop())
	{
		size_t start = vertices.size();
		Tessellator tessellator;
		tessellator.contours().push_back(silhouette().size());
		for(unsigned i=0; i<silhouette().size(); ++i){
			tessellator.contourVerts().push_back((dvec3)vertices[vertices.size()-i-1-tess_bottom_count]);
		}
		tessellator.setWindingRule(vl::TW_TESS_WINDING_NONZERO);
		tessellator.tessellate();
		for(unsigned i=0; i<tessellator.tessellatedTris().size(); ++i){
			vertices.push_back(tessellator.tessellatedTris()[i]);
		}
		if (tessellator.tessellatedTris().size()){
			geom->drawCalls()->push_back( new DrawArrays(PT_TRIANGLES, start, tessellator.tessellatedTris().size()) );
		}
		tess_top_count = tessellator.tessellatedTris().size();
	}

}

ref<Geometry> Extruder::exportGeometry(){
	ref<ArrayFloat3> vert_array = new ArrayFloat3;
	geom->setVertexArray( vert_array.get() );
	vert_array->initFrom(vertices);

	geom->drawCalls()->push_back(de.get());

	if (!smooth())
		geom->convertDrawCallToDrawArrays();

	geom->computeNormals();

	return geom;
}

void Extruder::clear(){
	this->vertices.clear();
	this->de->indexBuffer()->clear();
	this->geom->drawCalls()->clear();
	this->errFlag = false;
}

bool Extruder::getError(){
	// Check if the vertices are empty -> no object to render
	if(this->vertices.size() <= 0)
	{
		this->errFlag = true;
	}
	return this->errFlag;
}



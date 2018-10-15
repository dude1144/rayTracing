//Austin Anderson
//010640955
#pragma once

#include "ofMain.h"

class Material
{
public:
	ofColor diffuseColor, specularColor; // the diffuse and specular colors
	float p; // the power to use with specular shading

	Material(ofColor diffCol, ofColor specCol, float p)
	{
		diffuseColor = diffCol;
		specularColor = specCol;
		this->p = p;
	}
};

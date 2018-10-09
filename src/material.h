#pragma once

#include "ofMain.h"

class Material
{
public:
	ofColor diffuseColor, specularColor;
	float p;

	Material(ofColor diffCol, ofColor specCol, float p)
	{
		diffuseColor = diffCol;
		specularColor = specCol;
		this->p = p;
	}
};

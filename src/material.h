//Austin Anderson
//010640955
#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Material
{
public:
	ofColor diffuseColor, specularColor; // the diffuse and specular colors
	float p; // the power to use with specular shading
	ofxGuiGroup materialGroup;
	ofxInputField<float> pInput;
	ofxColorSlider diffuseInput;
	ofxColorSlider specularInput;

	Material(ofColor diffCol, ofColor specCol, float p)
	{
		diffuseColor = diffCol;
		specularColor = specCol;
		this->p = p;
		materialGroup.setup("Material");
		materialGroup.add(pInput.setup("P", this->p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}

	Material(const Material &mat)
	{
		diffuseColor = mat.diffuseColor;
		specularColor = mat.specularColor;
		p = mat.p;
		materialGroup.setup("Material");
		materialGroup.add(pInput.setup("P", this->p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}

	void setDiffuseColor(ofColor col)
	{
		diffuseColor = col;
		diffuseInput = col;
	}

	void setSpecularColor(ofColor col)
	{
		diffuseColor = col;
		specularInput = col;
	}

	void setP(float power)
	{
		p = power;
		pInput = p;
	}
};

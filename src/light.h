#pragma once
#include "ofMain.h"

class Light
{
public:
	float intensity;
	ofColor color;

	Light() { intensity = 0; color = ofColor(255, 255, 255); }
	Light(float i, ofColor col) { intensity = i; color = col; }
};
//Austin Anderson
//010640955
//from code given by professor
#pragma once
#include "ofMain.h"

class Ray 
{
public:
	Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
	void draw(float t) { ofDrawLine(p, p + t * d); }

	glm::vec3 evalPoint(float t) 
	{
		return (p + t * d);
	}

	glm::vec3 p, d;
};
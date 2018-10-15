//Austin Anderson
//010640955
//from code given by professor
#include "viewPlane.h"

glm::vec3 ViewPlane::toWorld(float u, float v)
{
	float w = width();
	float h = height();
	return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

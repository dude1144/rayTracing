//Austin Anderson
//010640955
//from code given by professor
#include "Plane.h"

bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 &normal) 
{
	float dist;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normal = this->normal;
	}
	return (hit);
}

bool Plane::intersectView(const Ray &ray, glm::vec3 & point, glm::vec3 &normal)
{
	return intersectRayTriangle(ray.p, ray.d, 
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		glm::vec3(0,0,0)) || intersectRayTriangle(ray.p, ray.d,
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3(0, 0, 0));
}
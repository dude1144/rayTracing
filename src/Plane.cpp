#include "Plane.h"

bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 &normal) {
	float dist;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normal = this->normal;
	}
	return (hit);
}
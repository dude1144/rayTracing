#include "bounds.h"

void OrientedBoundingBox::draw()
{
	ofDrawBox(center, extents[0]*2, extents[1]*2, extents[2]*2);
}

/*implemented from seperating axis theorem, found in 
the paper Dynamic Collision Detection by David Eberly,*/
bool OrientedBoundingBox::intersect(OrientedBoundingBox *box)
{
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = this->axes[i];

		bool nonIntersect = abs(glm::dot(L, box->center - this->center)) > abs(glm::dot(this->extents[0] * this->axes[0], L)) +
			abs(glm::dot(this->extents[1] * this->axes[1], L)) + abs(glm::dot(this->extents[2] * this->axes[2], L)) +
			abs(glm::dot(box->extents[0] * box->axes[0], L)) + abs(glm::dot(box->extents[1] * box->axes[1], L)) +
			abs(glm::dot(box->extents[2] * box->axes[2], L));

		if (nonIntersect)
			return false;
	}
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = box->axes[i];

		bool nonIntersect = abs(glm::dot(L, box->center - this->center)) > abs(glm::dot(this->extents[0] * this->axes[0], L)) +
			abs(glm::dot(this->extents[1] * this->axes[1], L)) + abs(glm::dot(this->extents[2] * this->axes[2], L)) +
			abs(glm::dot(box->extents[0] * box->axes[0], L)) + abs(glm::dot(box->extents[1] * box->axes[1], L)) +
			abs(glm::dot(box->extents[2] * box->axes[2], L));

		if (nonIntersect)
			return false;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 L = glm::cross(this->axes[i], box->axes[j]);

			bool nonIntersect = abs(glm::dot(L, box->center - this->center)) > abs(glm::dot(this->extents[0] * this->axes[0], L)) +
				abs(glm::dot(this->extents[1] * this->axes[1], L)) + abs(glm::dot(this->extents[2] * this->axes[2], L)) +
				abs(glm::dot(box->extents[0] * box->axes[0], L)) + abs(glm::dot(box->extents[1] * box->axes[1], L)) +
				abs(glm::dot(box->extents[2] * box->axes[2], L));

			if (nonIntersect)
				return false;
		}
	}

	return true;
}

/*implemented from seperating axis theorem, found in
the paper Dynamic Collision Detection by David Eberly,*/
bool OrientedBoundingBox::intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 edges[3] = { p2 - p1, p3 - p1, edges[1] - edges[0]};
	glm::vec3 normal= glm::normalize(glm::cross(edges[0], edges[1]));

	if ((glm::dot(normal, p1 - this->center) > (this->extents[0] * abs(glm::dot(normal, this->axes[0]))) +
		(this->extents[1] * abs(glm::dot(normal, this->axes[1]))) +
		(this->extents[2] * abs(glm::dot(normal, this->axes[2])))) ||
		(glm::dot(normal, p1 - this->center) < -1 * ((this->extents[0] * abs(glm::dot(normal, this->axes[0]))) +
		(this->extents[1] * abs(glm::dot(normal, this->axes[1]))) +
			(this->extents[2] * abs(glm::dot(normal, this->axes[2]))))))
		return false;

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = this->axes[i];

		float p0 = glm::dot(L, p1 - this->center);
		float p1 = p0 + glm::dot(L, edges[0]);
		float p2 = p0 + glm::dot(L, edges[1]);

		bool nonIntersect = min(min(p0, p1), p2) > this->extents[i] || max(max(p0, p1), p2) < -1 * this->extents[i];

		if (nonIntersect)
			return false;
	}
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = glm::cross(this->axes[0], edges[i]);

		float p0 = glm::dot(L, p1 - this->center);
		float p1 = p0 + glm::dot(L, edges[0]);
		float p2 = p0 + glm::dot(L, edges[1]);

		float R = (this->extents[1] * abs(glm::dot(this->axes[2], edges[i]))) + (this->extents[2] * abs(glm::dot(this->axes[1], edges[i])));

		bool nonIntersect = min(min(p0, p1), p2) > R || max(max(p0, p1), p2) < -1 * R;

		if (nonIntersect)
			return false;
	}
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = glm::cross(this->axes[1], edges[i]);

		float p0 = glm::dot(L, p1 - this->center);
		float p1 = p0 + glm::dot(L, edges[0]);
		float p2 = p0 + glm::dot(L, edges[1]);

		float R = (this->extents[0] * abs(glm::dot(this->axes[2], edges[i]))) + (this->extents[2] * abs(glm::dot(this->axes[0], edges[i])));

		bool nonIntersect = min(min(p0, p1), p2) > R || max(max(p0, p1), p2) < -1 * R;

		if (nonIntersect)
			return false;
	}
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 L = glm::cross(this->axes[0], edges[i]);

		float p0 = glm::dot(L, p1 - this->center);
		float p1 = p0 + glm::dot(L, edges[0]);
		float p2 = p0 + glm::dot(L, edges[1]);

		float R = (this->extents[0] * abs(glm::dot(this->axes[1], edges[i]))) + (this->extents[1] * abs(glm::dot(this->axes[0], edges[i])));

		bool nonIntersect = min(min(p0, p1), p2) > R || max(max(p0, p1), p2) < -1 * R;

		if (nonIntersect)
			return false;
	}
	return true;
}

bool OrientedBoundingBox::intersect(glm::vec3 p1, float radius)
{
	glm::vec3 closestPoint = center;
	glm::vec3 dir = p1 - center;

	for (int i = 0; i < 3; i++)
	{
		float dist = glm::dot(dir, axes[i]);

		if (dist > extents[i])
			dist = extents[i];
		else if (dist < -1 * extents[i])
			dist = -1 * extents[i];

		closestPoint = closestPoint + (axes[i] * dist);
	}

	if (glm::length(closestPoint - p1) > radius)
		return false;

	return true;
}
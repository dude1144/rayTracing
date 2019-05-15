#include "bounds.h"


#define hitInterval 1000
//-------------------------------------------------Oriented Bounding Box--------------------------------------------------------

void OrientedBoundingBox::draw()
{
	ofDrawBox(center, extents[0]*2, extents[1]*2, extents[2]*2);
}

//implemented from seperating axis theorem, found in 
//the paper Dynamic Collision Detection by David Eberly,
//found at https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
bool OrientedBoundingBox::intersectOBB(OrientedBoundingBox *box) const
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

//implemented from seperating axis theorem, found in 
//the paper Dynamic Collision Detection by David Eberly,
//found at https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
bool OrientedBoundingBox::intersectTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const
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

//intersect sphere by finding the closest point on the OBB then 
//checking if the distance to that point is greater than the radius
bool OrientedBoundingBox::intersectSphere(glm::vec3 p1, float radius) const
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

//intersection of OBB with plane using seperating axis theorem
bool OrientedBoundingBox::intersectPlane(glm::vec3 p1, glm::vec3 normal) const
{
	float interval = this->extents[0] * abs(glm::dot(normal, this->axes[0])) + 
					 this->extents[1] * abs(glm::dot(normal, this->axes[1])) +
					 this->extents[2] * abs(glm::dot(normal, this->axes[2]));
	float distance = abs(glm::dot(normal, this->center - p1));

	return distance <= interval;
}

//check if box contains given point
bool OrientedBoundingBox::contains(glm::vec3 point) const
{
	glm::vec3 p1 = point - this->center;

	if (abs(glm::dot(p1, this->axes[0])) > this->extents[0])
		return false;
	if (abs(glm::dot(p1, this->axes[1])) > this->extents[1])
		return false;
	if (abs(glm::dot(p1, this->axes[2])) > this->extents[2])
		return false;

	return true;
}

//implemented from Game Physics Cookbook
//https://github.com/gszauer/GamePhysicsCookbook
bool OrientedBoundingBox::intersectRay(glm::vec3 point, glm::vec3 dir) const
{
	glm::vec3 p = this->center - point;
	glm::vec3 f = glm::vec3(glm::dot(this->axes[0], dir), glm::dot(this->axes[1], dir), glm::dot(this->axes[2], dir));
	glm::vec3 e = glm::vec3(glm::dot(this->axes[0], p), glm::dot(this->axes[1], p), glm::dot(this->axes[2], p));

	float t[6];

	for (int i = 0; i < 3; i++)
	{
		if (f[i] == 0)
		{
			if (-1 * e[i] - extents[i] > 0 || -1 * e[i] + extents[0] > 0)
				return false;
			f[i] = .0000001;
		}
		t[i * 2 + 0] = (e[i] + extents[i]) / f[i];
		t[i * 2 + 1] = (e[i] - extents[i]) / f[i];
	}

	float tMin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
	float tMax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

	if (tMax < 0 || tMin > tMax)
		return false;

	if (tMin < 0)
		return tMax;

	return tMin;
}

//intersect an intersectable by calling it's intersect(OrientedBoundingBox) method
bool OrientedBoundingBox::intersect(Intersectable* toIntersect) const
{
	return toIntersect->intersect(*this);
}

//------------------------------------------------Axis-Aligned Bounding Box-----------------------------------------------------

void AxisAlignedBoundingBox::draw()
{
	ofDrawBox(center(), 2 * abs(max.x - center().x), 2 * abs(max.y - center().y), 2 * abs(max.z - center().z));
}

bool AxisAlignedBoundingBox::intersectOBB(OrientedBoundingBox * box) const
{
	return false;
}

bool AxisAlignedBoundingBox::intersectTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const
{
	return false;
}

bool AxisAlignedBoundingBox::intersectSphere(glm::vec3 p1, float radius) const
{
	return false;
}

bool AxisAlignedBoundingBox::intersectPlane(glm::vec3 p1, glm::vec3 normal) const
{
	return false;
}

/*
 Ray-box intersection using IEEE numerical properties to ensure that the
 test is both robust and efficient, as described in:
 
      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
       "An Efficient and Robust Ray-Box Intersection Algorithm"
       Journal of graphics tools, 10(1):49-54, 2005
 
 */
bool AxisAlignedBoundingBox::intersectRay(glm::vec3 point, glm::vec3 dir) const
{
	glm::vec3 params[2] = { min, max };
	glm::vec3 invDir = glm::vec3(1 / dir.x, 1 / dir.y, 1 / dir.z);
	int xSign = invDir.x < 0;
	int ySign = invDir.y < 0;
	int zSign = invDir.z < 0;

	float tXMin = (params[xSign].x - point.x) * invDir.x;
	float tXMax = (params[1 - xSign].x - point.x) * invDir.x;
	float tYMin = (params[ySign].y - point.y) * invDir.y;
	float tYMax = (params[1 - ySign].y - point.y) * invDir.y;
	if (tXMin > tYMax || tYMin > tXMax)
		return false;
	if (tYMin > tXMin)
		tXMin = tYMin;
	if (tYMax < tXMax)
		tXMax = tYMax;
	float tZMin = (params[zSign].z - point.z) * invDir.z;
	float tZMax = (params[1 - zSign].z - point.z) * invDir.z;
	if ((tXMin > tZMax) || (tZMin > tXMax))
		return false;
	if (tZMin > tXMin)
		tXMin = tZMin;
	if (tZMax < tXMax)
		tXMax = tZMax;
	return ((tXMin < hitInterval) && (tXMax > -hitInterval));
}

bool AxisAlignedBoundingBox::contains(glm::vec3 p1) const
{
	return false;
}

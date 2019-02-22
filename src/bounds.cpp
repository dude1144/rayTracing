#include "bounds.h"
#include "light.h"

void OrientedBoundingBox::draw()
{
	ofDrawBox(center, extents[0]*2, extents[1]*2, extents[2]*2);
}

//implemented from seperating axis theorem, found in 
//the paper Dynamic Collision Detection by David Eberly,
//found at https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
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

//implemented from seperating axis theorem, found in 
//the paper Dynamic Collision Detection by David Eberly,
//found at https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
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

//intersect sphere by finding the closest point on the OBB then 
//checking if the distance to that point is greater than the radius
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

//intersection of OBB with plane using seperating axis theorem
bool OrientedBoundingBox::intersect(glm::vec3 p1, glm::vec3 normal)
{
	float interval = this->extents[0] * abs(glm::dot(normal, this->axes[0])) + 
					 this->extents[1] * abs(glm::dot(normal, this->axes[1])) +
					 this->extents[2] * abs(glm::dot(normal, this->axes[2]));
	float distance = abs(glm::dot(normal, this->center - p1));

	return distance <= interval;
}

bool OrientedBoundingBox::contains(glm::vec3 p1)
{
	if (abs(glm::dot(p1, this->axes[0])) > this->extents[0])
		return false;
	if (abs(glm::dot(p1, this->axes[1])) > this->extents[1])
		return false;
	if (abs(glm::dot(p1, this->axes[2])) > this->extents[2])
		return false;

	return true;
}


//determine which intersect to call if a generic pointer is passed
bool OrientedBoundingBox::intersect(SceneObject* object)
{
	if (dynamic_cast<Mesh*>(object))
		return intersect(dynamic_cast<Mesh*>(object));
	else if (dynamic_cast<Sphere*>(object))
		return intersect(dynamic_cast<Sphere*>(object));
	else if (dynamic_cast<Plane*>(object))
		return intersect(dynamic_cast<Plane*>(object));
	else if (dynamic_cast<Light*>(object))
		return intersect(dynamic_cast<Light*>(object));
}

bool OrientedBoundingBox::intersect(Sphere* sphere)
{
	return intersect(sphere->position, sphere->radius);
}
bool OrientedBoundingBox::intersect(Plane* plane)
{
	return intersect(plane->position, plane->normal);
}
bool OrientedBoundingBox::intersect(Light* light)
{
	return contains(light->position);
}
bool OrientedBoundingBox::intersect(Mesh* mesh)
{
	glm::mat4 rInv = glm::inverse(mesh->getRotateMatrix());
	OrientedBoundingBox temp;
	temp.center = glm::inverse(mesh->getTranslateMatrix()) * glm::vec4(temp.center, 1);
	temp.axes[0] = glm::normalize(rInv * glm::vec4(temp.axes[0], 1));
	temp.axes[1] = glm::normalize(rInv * glm::vec4(temp.axes[1], 1));
	temp.axes[2] = glm::normalize(rInv * glm::vec4(temp.axes[2], 1));
	
	for (int i = 0; i < mesh->ofmeshes.size(); i++)
	{
		vector<ofIndexType> indices = mesh->ofmeshes[i].getIndices();

		for (int j = 0; j < indices.size(); j += 3)
		{
			glm::vec3 v1 = mesh->ofmeshes[i].getVertices()[indices[j]];
			glm::vec3 v2 = mesh->ofmeshes[i].getVertices()[indices[j + 1]];
			glm::vec3 v3 = mesh->ofmeshes[i].getVertices()[indices[j + 2]];

			if (this->intersect(v1, v2, v3))
				return true;
		}
	}

	return false;
}

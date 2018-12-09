#include "primitives.h"

//Count initilizations
int Sphere::count = 0;
int Plane::count = 0;

//UI Setups
void Sphere::setupUI()
{
	settings.setup(name);
	settings.add(positionGroup.setup("Position"));
	positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	settings.add(&mat.materialGroup);
	this->updateFromUI();
}

void Plane::setupUI()
{
	settings.setup(name);
	settings.add(positionGroup.setup("Position"));
	positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	settings.add(normalGroup.setup("Normal"));
	normalGroup.add(xNormalInput.setup("X", normal.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	normalGroup.add(yNormalInput.setup("Y", normal.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	normalGroup.add(zNormalInput.setup("Z", normal.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	settings.add(&mat.materialGroup);
	this->updateFromUI();
}

//Intersection methods
bool Sphere::intersect(const Ray &ray, IntersectInfo &intersect)
{
	//check for intersection and save point and normal into intersect
	if (glm::intersectRaySphere(ray.p, ray.d, position, radius, intersect.point, intersect.normal))
	{
		//if intersection happened, calculate the distance and return true
		intersect.dist = glm::length(intersect.point - ray.p);
		return true;
	}
	return false;
}

bool Sphere::intersectView(const Ray &ray, IntersectInfo &intersect)
{
	//check for intersection and save point and normal into intersect
	if (glm::intersectRaySphere(ray.p, ray.d, position, radius, intersect.point, intersect.normal))
	{
		//if intersection happened, calculate the distance and return true
		intersect.dist = glm::length(intersect.point - ray.p);
		return true;
	}
	return false;
}

bool Plane::intersect(const Ray &ray, IntersectInfo &intersect)
{
	//check for intersection and save point and normal into intersect
	if (glm::intersectRayPlane(ray.p, ray.d, position, this->normal, intersect.dist)) 
	{
		Ray r = ray;
		intersect.point = r.evalPoint(intersect.dist);
		intersect.normal = this->normal;

		return true;
	}
	return false;
}

bool Plane::intersectView(const Ray &ray, IntersectInfo &intersect)
{
	if( intersectRayTriangle(ray.p, ray.d,
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		intersect.barry) ) 
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)) * intersect.barry.x) + 
						  (glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)) * intersect.barry.y) + 
						  (glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)) * intersect.barry.z);
		intersect.dist = glm::length(intersect.point - ray.p);
		intersect.normal = this->normal;
		return true;
	}
	else if (intersectRayTriangle(ray.p, ray.d,
		glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)),
		glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)),
		glm::vec3(0, 0, 0)) )
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2)) * intersect.barry.x) + 
						  (glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)) * intersect.barry.y) + 
						  (glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2)) * intersect.barry.z);
		intersect.dist = glm::length(intersect.point - ray.p);
		intersect.normal = this->normal;
		return true;
	}
	return false;
}

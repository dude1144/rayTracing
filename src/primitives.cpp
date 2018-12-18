#include "primitives.h"

//Count initilizations
int Sphere::count = 0;
int Plane::count = 0;
int Mesh::count = 0;

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

void Mesh::setupUI()
{
	settings.setup(name);
	settings.add(positionGroup.setup("Position"));
	positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
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
		intersect.normal = glm::normalize(this->normal);

		return true;
	}
	return false;
}

bool Plane::intersectView(const Ray &ray, IntersectInfo &intersect)
{
	glm::vec3 p1 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));
	glm::vec3 p2 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
	glm::vec3 p3 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
	glm::vec3 p4 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));

	if( intersectRayTriangle(ray.p, ray.d, p1, p2, p3, intersect.barry) ) 
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (p1 * intersect.barry.z) + (p2 * intersect.barry.x) + (p3 * intersect.barry.y);
		intersect.dist = glm::length(intersect.point - ray.p);
		intersect.normal = glm::normalize(this->normal);
		return true;
	}
	else if ( intersectRayTriangle(ray.p, ray.d, p1, p3, p4, intersect.barry) )
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (p1 * intersect.barry.z) + (p3 * intersect.barry.x) + (p4 * intersect.barry.y);
		intersect.dist = glm::length(intersect.point - ray.p);
		intersect.normal = glm::normalize(this->normal);
		return true;
	}
	return false;
}

bool Mesh::intersect(const Ray &ray, IntersectInfo &intersect)
{
	IntersectInfo closest;
	for (int i = 0; i < model.getNumMeshes(); i++)
	{
		vector<ofMeshFace> triangles = model.getMesh(i).getUniqueFaces();
		for (int j = 0; j < triangles.size(); j++)
		{
			IntersectInfo temp;
			glm::vec3 v1 = triangles[j].getVertex(0);
			glm::vec3 v2 = triangles[j].getVertex(1);
			glm::vec3 v3 = triangles[j].getVertex(2);
			if (glm::intersectRayTriangle(ray.p, ray.d, v1, v2, v3, temp.barry))
			{
				temp.barry.z = 1 - (temp.barry.x + temp.barry.y);
				temp.point = (v1 * temp.barry.z) + (v2 * temp.barry.x) + (v3 * temp.barry.y);
				temp.dist = glm::length(temp.point - ray.p);
				temp.normal = glm::normalize(triangles[j].getFaceNormal());
				if (temp.dist < closest.dist)
				{
					closest = temp;
				}
			}
		}
	}
	if (closest.dist < std::numeric_limits<float>::max())
	{
		intersect = closest;
		return true;
	}
	else
		return false;
}

bool Mesh::intersectView(const Ray &ray, IntersectInfo &intersect)
{
	IntersectInfo closest;
	for (int i = 0; i < model.getNumMeshes(); i++)
	{
		vector<ofMeshFace> triangles = model.getMesh(i).getUniqueFaces();
		for (int j = 0; j < triangles.size(); j++)
		{
			IntersectInfo temp;
			glm::vec3 v1 = triangles[j].getVertex(0);
			glm::vec3 v2 = triangles[j].getVertex(1);
			glm::vec3 v3 = triangles[j].getVertex(2);
			if (glm::intersectRayTriangle(ray.p, ray.d, v1, v2, v3, temp.barry))
			{
				temp.barry.z = 1 - (temp.barry.x + temp.barry.y);
				temp.point = (v1 * temp.barry.z) + (v2 * temp.barry.x) + (v3 * temp.barry.y);
				temp.dist = glm::length(temp.point - ray.p);
				temp.normal = glm::normalize(triangles[j].getFaceNormal());
				if (temp.dist < closest.dist)
				{
					closest = temp;
				}
			}
		}
	}
	if (closest.dist < std::numeric_limits<float>::max())
	{
		intersect = closest;
		return true;
	}
	else
		return false;
}

bool Mesh::load(std::string name)
{
	bool m = model.loadModel(name);
	model.setScale(.1, .1, .1);
#if _DEBUG //print out model information
	cout << model.getScale() << endl;
	vector<std::string> names = model.getMeshNames();
	cout << names.size() << endl;
	for (int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
	}
#endif
	return m;
}

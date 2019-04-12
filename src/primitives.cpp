#include "primitives.h"

//----------------------------------------------Count initilizations------------------------------------------------------------
int Sphere::count = 0;
int Plane::count = 0;
int Mesh::count = 0;

//----------------------------------------------------UI Setups-----------------------------------------------------------------
void Sphere::setupUI()
{
	settings.setup(name);
	settings.add(positionGroup.setup("Position"));
	positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	//settings.add(radiusInput.setup("Radius", radius, 0, std::numeric_limits<float>::max()));
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
	settings.add(smooth.setup("Smooth Shading", false));
	settings.add(&mat.materialGroup);
	this->updateFromUI();
}


//-------------------------------------------Ray-Intersection methods-----------------------------------------------------------

bool Sphere::intersect(const Ray &ray, IntersectInfo &intersect)
{
	//check for intersection and save point and normal into intersect
	if (glm::intersectRaySphere(ray.point, ray.dir, position, radius, intersect.point, intersect.normal))
	{
		//if intersection happened, calculate the distance and return true
		intersect.dist = glm::length(intersect.point - ray.point);
		return true;
	}
	return false;
}

bool Plane::intersect(const Ray &ray, IntersectInfo &intersect)
{
	//check for intersection and save point and normal into intersect
	if (glm::intersectRayPlane(ray.point, ray.dir, position, this->normal, intersect.dist)) 
	{
		Ray r = ray;
		intersect.point = r.evalPoint(intersect.dist);
		intersect.normal = glm::normalize(this->normal);

		return true;
	}
	return false;
}

bool Mesh::intersect(const Ray &ray, IntersectInfo &intersect)
{
	IntersectInfo closest;

	//transform the ray into object space
	glm::mat4 mInv = glm::inverse(getMatrix());
	glm::vec3 p = mInv * glm::vec4(ray.point, 1.0);
	glm::vec3 p1 = mInv * glm::vec4(ray.point + ray.dir, 1.0);
	glm::vec3 d = glm::normalize(p1 - p);

	for (int i = 0; i < ofmeshes.size(); i++)
	{
		vector<ofIndexType> indices = ofmeshes[i].getIndices();

		for (int j = 0; j < indices.size(); j += 3)
		{
			IntersectInfo temp;

			glm::vec3 v1 = ofmeshes[i].getVertices()[indices[j]];
			glm::vec3 v2 = ofmeshes[i].getVertices()[indices[j + 1]];
			glm::vec3 v3 = ofmeshes[i].getVertices()[indices[j + 2]];

			if (glm::intersectRayTriangle(p, d, v1, v2, v3, temp.barry))
			{
				temp.barry.z = 1 - (temp.barry.x + temp.barry.y);
				temp.point = (v1 * temp.barry.z) + (v2 * temp.barry.x) + (v3 * temp.barry.y);
				temp.dist = glm::length(temp.point - p);
				if (temp.dist < closest.dist)
				{
					if (smooth)
					{
						temp.normal = (ofmeshes[i].getNormal(indices[j]) * temp.barry.z) + (ofmeshes[i].getNormal(indices[j + 1]) * temp.barry.x) + (ofmeshes[i].getNormal(indices[j + 2]) * temp.barry.y);
					}
					else
						temp.normal = glm::normalize(glm::cross((v1 - v2), (v1 - v3)));
					closest = temp;
				}
			}
		}
	}
	if (closest.dist < std::numeric_limits<float>::max())
	{
		intersect = closest;
		intersect.point = getMatrix() * glm::vec4(intersect.point, 1.0);
		return true;
	}
	else
		return false;
}

bool Plane::intersectView(const Ray &ray, IntersectInfo &intersect)
{
	glm::vec3 p1 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));
	glm::vec3 p2 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
	glm::vec3 p3 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
	glm::vec3 p4 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));

	if( intersectRayTriangle(ray.point, ray.dir, p1, p2, p3, intersect.barry) ) 
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (p1 * intersect.barry.z) + (p2 * intersect.barry.x) + (p3 * intersect.barry.y);
		intersect.dist = glm::length(intersect.point - ray.point);
		intersect.normal = glm::normalize(this->normal);
		return true;
	}
	else if ( intersectRayTriangle(ray.point, ray.dir, p1, p3, p4, intersect.barry) )
	{
		intersect.barry.z = 1 - (intersect.barry.x + intersect.barry.y);
		intersect.point = (p1 * intersect.barry.z) + (p3 * intersect.barry.x) + (p4 * intersect.barry.y);
		intersect.dist = glm::length(intersect.point - ray.point);
		intersect.normal = glm::normalize(this->normal);
		return true;
	}
	return false;
}

//--------------------------------------------------SDFs------------------------------------------------------------------------
float Sphere::sdf(glm::vec3 &p)
{
	return glm::length(p - this->position) - this->radius;
}

float Plane::sdf(glm::vec3 &p)
{
	return abs(glm::dot(normal, p - this->position));
}

//--------------------------------------------------Octree Intersects-----------------------------------------------------------

bool Sphere::intersect(const OrientedBoundingBox& box)
{
	return box.intersectSphere(this->position, this->radius);
}
bool Plane::intersect(const OrientedBoundingBox& box)
{
	return box.intersectPlane(this->position, this->normal);
}
bool Triangle::intersect(const OrientedBoundingBox& box)
{
	//create a temporary OBB that is transformed into the meshs space
	glm::mat4 rInv = glm::inverse(this->parent->getRotateMatrix());
	OrientedBoundingBox temp;
	temp.center = glm::inverse(this->parent->getTranslateMatrix()) * glm::vec4(box.center, 1);
	/*temp.axes[0] = glm::normalize(rInv * glm::vec4(this->axes[0], 1));
	temp.axes[1] = glm::normalize(rInv * glm::vec4(this->axes[1], 1));
	temp.axes[2] = glm::normalize(rInv * glm::vec4(this->axes[2], 1));*/
	temp.extents[0] = box.extents[0];
	temp.extents[1] = box.extents[1];
	temp.extents[2] = box.extents[2];

	//get the vertices of the triangle
	vector<ofIndexType> indices = this->parent->ofmeshes[this->meshNum].getIndices();
	glm::vec3 v1 = this->parent->ofmeshes[this->meshNum].getVertices()[this->indices[0]];
	glm::vec3 v2 = this->parent->ofmeshes[this->meshNum].getVertices()[this->indices[0]];
	glm::vec3 v3 = this->parent->ofmeshes[this->meshNum].getVertices()[this->indices[0]];

	//check intersection
	return temp.intersectTriangle(v1, v2, v3);
}
bool Mesh::intersect(const OrientedBoundingBox& box)
{
	//create a temporary OBB that is transformed into the meshs space
	glm::mat4 rInv = glm::inverse(this->getRotateMatrix());
	OrientedBoundingBox temp;
	temp.center = glm::inverse(this->getTranslateMatrix()) * glm::vec4(box.center, 1);
	/*temp.axes[0] = glm::normalize(rInv * glm::vec4(this->axes[0], 1));
	temp.axes[1] = glm::normalize(rInv * glm::vec4(this->axes[1], 1));
	temp.axes[2] = glm::normalize(rInv * glm::vec4(this->axes[2], 1));*/
	temp.extents[0] = box.extents[0];
	temp.extents[1] = box.extents[1];
	temp.extents[2] = box.extents[2];

	//iterate through all the meshes, checking every triangle
	for (int i = 0; i < this->ofmeshes.size(); i++)
	{
		vector<ofIndexType> indices = this->ofmeshes[i].getIndices();

		for (int j = 0; j < indices.size(); j += 3)
		{
			glm::vec3 v1 = this->ofmeshes[i].getVertices()[indices[j]];
			glm::vec3 v2 = this->ofmeshes[i].getVertices()[indices[j + 1]];
			glm::vec3 v3 = this->ofmeshes[i].getVertices()[indices[j + 2]];

			if (temp.intersectTriangle(v1, v2, v3))
				return true;
		}
	}

	return false;
}



bool Mesh::load(std::string name)
{
	ofxAssimpModelLoader model;
	bool m = model.loadModel(name);

	for (int i = 0; i < model.getMeshCount(); i++)
	{
		ofmeshes.push_back(model.getMesh(i));
	}

#if _DEBUG //print out model information
	cout << model.getScale() << endl;
	vector<std::string> names = model.getMeshNames();
	cout << names.size() << endl;
	for (int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
		cout << "\tNumVerts: " << model.getMesh(i).getNumVertices() << endl;
		cout << "\tNumFaces: " << model.getMesh(i).getUniqueFaces().size() << endl;
		cout << "\tNumNormals: " << model.getMesh(i).getNumNormals() << endl;
		cout << "\tNumIndices: " << model.getMesh(i).getIndices().size() << endl;
		cout << "\tNumTexCoords: " << model.getMesh(i).getTexCoords().size() << endl;
	}
#endif
	model.clear();
	return m;
}

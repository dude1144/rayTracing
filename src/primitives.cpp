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
	//get inverse of transformation matrix
	glm::mat4 mInv = glm::inverse(this->parent->getMatrix());

	//transform the triangle back to world space
	glm::vec3 v1 = this->parent->getMatrix() * glm::vec4(this->get(0), 0);
	glm::vec3 v2 = this->parent->getMatrix() * glm::vec4(this->get(1), 0);
	glm::vec3 v3 = this->parent->getMatrix() * glm::vec4(this->get(2), 0);

	//check intersection
	return box.intersectTriangle(v1, v2, v3);
}
bool Mesh::intersect(const OrientedBoundingBox& box)
{
	for (int i = 0; i < this->tris.size(); i++)
	{
		if (tris[i].intersect(box))
			return true;
	}
	return false;
}

bool Mesh::load(std::string name)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	//create a assimpModel loader and load the model from the string
	ofxAssimpModelLoader model;
	bool m = model.loadModel(name);


	//exctract the ofMeshes from the assimpModelLoader and create Triangles for all triangles in the mesh
	for (int i = 0; i < model.getMeshCount(); i++)
	{
		ofmeshes.push_back(model.getMesh(i));
		for (int j = 0; j < ofmeshes[i].getNumIndices(); j+=3)
		{
			tris.push_back(Triangle(this, i, ofmeshes[i].getIndex(j), ofmeshes[i].getIndex(j + 1), ofmeshes[i].getIndex(j+ 2 )));
		}
	}

	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();

#if _DEBUG //print out model information
	cout << "loaded in " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / (float)1000000000 << "seconds" << endl;
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

	//clear the assimpModelLoader
	model.clear();
	return m;
}

#include "scene.h"

void Scene::draw(SceneObject* selected, bool drawTree)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects[i] == selected)
			ofSetColor(ofColor(247, 189, 0));
		else
			ofSetColor(this->objects[i]->mat.diffuseColor);
		this->objects[i]->draw();
	}
	for (int i = 0; i < this->lights.size(); i++)
	{
		if (this->lights[i] == selected)
			ofSetColor(ofColor(247, 189, 0));
		else
			ofSetColor(this->lights[i]->mat.diffuseColor);
		this->lights[i]->draw();
	}

	if (drawTree)
	{
		bool x = ofGetFill();
		ofNoFill();
		ofSetColor(ofColor::aquamarine);
		tree.draw();
		if (x)
			ofFill();
	}
}

SceneObject* Scene::intersect(const Ray& ray, IntersectInfo& intersect)
{
	//IntersectInfo and index of current closest sceneObject
	IntersectInfo closest;
	int index = -1;

	for (int i = 0; i < objects.size(); i++)
	{
		//get intersect info of object i, if it is closer than the current closest, save it's IntersectInfo and index
		IntersectInfo temp;
		if (objects[i]->intersect(ray, temp))
			if (temp.dist < closest.dist)
			{
				closest = temp;
				index = i;
			}
	}
	if (index == -1)
		return nullptr;
	else
	{
		intersect = closest;
		return objects[index];
	}
}

SceneObject* Scene::intersectOctree(const Ray &ray, IntersectInfo &intersect)
{
	Intersectable* object = tree.intersect(ray.point, ray.dir);

	if (object)
	{
		if (dynamic_cast<Triangle*>(object))
		{
			dynamic_cast<Triangle*>(object)->intersect(ray, intersect);
			return dynamic_cast<Triangle*>(object)->parent;
		}
		else
		{
			dynamic_cast<SceneObject*>(object)->intersect(ray, intersect);
			return dynamic_cast<SceneObject*>(object);
		}
	}
	return nullptr;
}


SceneObject* Scene::intersectSelect(const Ray &ray)
{
	//IntersectInfo and index of current closest sceneObject
	IntersectInfo closest;
	int index = -1;
	bool object = true;

	for (int i = 0; i < objects.size(); i++)
	{
		//get intersect info of object i, if it is closer than the current closest, save it's IntersectInfo and index
		IntersectInfo temp;
		if (objects[i]->intersectView(ray, temp))
			if (temp.dist < closest.dist)
			{
				closest = temp;
				index = i;
			}
	}
	for (int i = 0; i < lights.size(); i++)
	{
		//get intersect info of object i, if it is closer than the current closest, save it's IntersectInfo and index
		IntersectInfo temp;
		if (lights[i]->intersectView(ray, temp))
			if (temp.dist < closest.dist)
			{
				closest = temp;
				index = i;
				object = false;
			}
	}
	if (index == -1)
		return nullptr;
	else
	{
		if (object)
			return objects[index];
		else
			return lights[index];
	}
}

bool Scene::remove(SceneObject* obj)
{
	if (dynamic_cast<Light*>(obj))
	{
		//loop through all lights to find the pointed to obj, remove it and delete it from memory
		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i] == obj)
			{
				if (i == lights.size() - 1)
					lights.pop_back();
				else
					lights.erase(lights.begin() + i);
				delete obj;
				return true;
			}
		}
		return false;
	}
	else
	{
		//loop through all objects to find the pointed to obj, remove it and delete it from memory
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == obj)
			{
				if (i == objects.size() - 1)
					objects.pop_back();
				else
					objects.erase(objects.begin() + i);
				delete obj;
				return true;
			}
		}
		return false;
	}
}

vector<Intersectable*> Scene::getIntersectables()
{
	vector<Intersectable*> intersectables;

	//loop through all object pointers, cast to Intersectable*, and add to intersectables
	for (int i = 0; i < objects.size(); i++)
	{
		intersectables.push_back(dynamic_cast<Intersectable*>(objects[i]));
	}
	return intersectables;
}

vector<Intersectable*> Scene::getIntersectablesMeshAsTris()
{
	vector<Intersectable*> intersectables;

	//loop through all object pointers, cast to Intersectable*, or add all tris if mesh
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Mesh*>(objects[i]))
		{
			Mesh* temp = dynamic_cast<Mesh*>(objects[i]);
			for (int j = 0; j < temp->tris.size(); j++)
				intersectables.push_back(&(temp->tris[j]));
		}
		else
			intersectables.push_back(dynamic_cast<Intersectable*>(objects[i]));
	}
	return intersectables;
}

void Scene::setupTree()
{
	tree = Octree(glm::vec3(-10, -10, -10), glm::vec3(10, 10, 10));
	tree.eval(this->getIntersectablesMeshAsTris());
}
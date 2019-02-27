#pragma once
#include "primitives.h"
#include "light.h"
#include "octree.h"
#include <vector>

class Scene
{
private:
	Octree tree;
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;

	vector<SceneObject*> getAll();

	void add(SceneObject* obj)
	{
		if (dynamic_cast<Light*>(obj))
			lights.push_back(dynamic_cast<Light*>(obj));
		else
		objects.push_back(obj);
	}

	void setupTree();


	bool remove(SceneObject* obj);

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
};
#pragma once
#include "primitives.h"
#include "light.h"
#include <vector>

class Scene
{
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;

	void add(SceneObject* obj)
	{
		objects.push_back(obj);
	}
	void add(Light* light)
	{
		lights.push_back(light);
	}

	bool remove(SceneObject* obj)
	{
		return false;
	}
	bool remove(Light* light)
	{
		return false;
	}

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
};
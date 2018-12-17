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

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
};
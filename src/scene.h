#pragma once
#include "primitives.h"
#include "light.h"
#include <vector>

class Scene
{
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
};
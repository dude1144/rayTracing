#pragma once
#include "primitives.h"
#include "light.h"
#include <vector>

class Scene
{
public:
	//vectors to hold sceneObjects
	vector<SceneObject*> objects;
	vector<Light*> lights;

	//tree for speeding intersection
	Octree tree;
	void setupTree();

	void draw(SceneObject* selected, bool drawTree);

	bool remove(SceneObject* obj);
	void add(SceneObject* obj)
	{
		if (dynamic_cast<Light*>(obj))
			lights.push_back(dynamic_cast<Light*>(obj));
		else
		objects.push_back(obj);
	}

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
	SceneObject* intersectOctree(const Ray &ray, IntersectInfo &intersect);
	SceneObject* intersectSelect(const Ray &ray);
	vector<Intersectable*> getIntersectablesMeshAsTris();
	vector<Intersectable*> getIntersectables();
};
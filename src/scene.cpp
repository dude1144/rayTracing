#include "scene.h"

SceneObject * Scene::intersect(const Ray & ray, IntersectInfo &intersect)
{
	IntersectInfo closest;
	int index = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		IntersectInfo temp;
		if(objects[i]->intersect(ray, temp))
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

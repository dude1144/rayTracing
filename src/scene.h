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
		if (dynamic_cast<Light*>(obj))
			lights.push_back(dynamic_cast<Light*>(obj));
		else
		objects.push_back(obj);
	}


	bool remove(SceneObject* obj)
	{
		if (dynamic_cast<Light*>(obj))
		{
#if _DEBUG
			cout << "Trying to remove Light" << endl;
#endif
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
#if _DEBUG
			cout << "Trying to remove Object" << endl;
#endif
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

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
};
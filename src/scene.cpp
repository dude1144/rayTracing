#include "scene.h"


vector<SceneObject*> Scene::getAll()
{
	vector<SceneObject*> all = objects;
	for (int i = 0; i < lights.size(); i++)
	{
		all.push_back(lights[i]);
	}
	return all;
}

void Scene::setupTree()
{
	vector<SceneObject*> obj = this->getAll();
	for (int i = 0; i < obj.size(); i++)
	{
		if (dynamic_cast<Mesh*>(obj[i]))
		{
			dynamic_cast<Mesh*>(obj[i])->getTris(&obj);
			obj.erase(obj.begin() + i);
			i--;
		}
	}
	tree = Octree(glm::vec3(-50, -50, 50), glm::vec3(50, 50, -50));
	tree.eval(obj);
}

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

bool Scene::remove(SceneObject* obj)
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
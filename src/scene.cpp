#include "scene.h"


//void Scene::meshToTri(vector<SceneObject*> obj)
//{
//	for (int i = 0; i < obj.size(); i++)
//	{
//		if (dynamic_cast<Mesh*>(obj[i]))
//		{
//			dynamic_cast<Mesh*>(obj[i])->getTris(&obj);
//			obj.erase(obj.begin() + i);
//			i--;
//		}
//	}
//}

vector<Intersectable*> Scene::getIntersectablesMeshAsTris()
{
	vector<Intersectable*> intersectables;
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Intersectable*>(objects[i]))
		{
			if (dynamic_cast<Mesh*>(objects[i]))
			{
				dynamic_cast<Mesh*>(objects[i])->getTris();
			}
			else
				intersectables.push_back(dynamic_cast<Intersectable*>(objects[i]));
		}
	}
	return intersectables;
}

vector<Intersectable*> Scene::getIntersectables()
{
	vector<Intersectable*> intersectables;
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Intersectable*>(objects[i]))
		{
			intersectables.push_back(dynamic_cast<Intersectable*>(objects[i]));
		}
	}
	return intersectables;
}

void Scene::setupTree()
{
	tree = Octree(glm::vec3(-20, -20, 20), glm::vec3(20, 20, -20));
	tree.eval(this->getIntersectablesMeshAsTris());
}

SceneObject* Scene::intersect(const Ray& ray, IntersectInfo& intersect)
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

SceneObject* Scene::rayMarch(Ray r, IntersectInfo &intersect, glm::vec3 repeat)
{
	int index = -1;
	float dist = sceneSDF(r.point, index, repeat);

	for(int i = 0; i < MAX_STEPS; i++)
	{
		if (dist <= HIT_DIST)
		{
			//cout << "hit!" << endl;
			SceneObject* obj = this->objects[index];
			if(repeat == glm::vec3(0,0,0))
				intersect.point = r.evalPoint(dist);
			else
			{
				glm::vec3 p = r.evalPoint(dist);
				intersect.point = glm::vec3(std::fmod(p.x, repeat.x), std::fmod(p.y, repeat.y), std::fmod(p.z, repeat.z)) -.5 * repeat;
			}
			intersect.normal.x = obj->sdf(intersect.point + glm::vec3(EPS, 0, 0)) - obj->sdf(intersect.point - glm::vec3(EPS, 0, 0));
			intersect.normal.y = obj->sdf(intersect.point + glm::vec3(0, EPS, 0)) - obj->sdf(intersect.point - glm::vec3(0, EPS, 0));
			intersect.normal.z = obj->sdf(intersect.point + glm::vec3(0, 0, EPS)) - obj->sdf(intersect.point - glm::vec3(0, 0, EPS));
			return obj;
		}
		if (dist >= MISS_DIST)
		{
			return nullptr;
		}
		r.point = r.evalPoint(dist);
		dist = sceneSDF(r.point, index, repeat);
	}
	return nullptr;
}

float Scene::sceneSDF(glm::vec3 &p, int &index, glm::vec3 repeat)
{
	float closest = std::numeric_limits<float>::max();
	glm::vec3 point = p;
	if (repeat != glm::vec3(0, 0, 0))
	{
		point = glm::vec3(std::fmod(p.x, repeat.x), std::fmod(p.y, repeat.y), std::fmod(p.z, repeat.z)) -.5*repeat;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		float dist = objects[i]->sdf(point);
		if (dist < closest)
		{
			closest = dist;
			index = i;
		}
	}
	return closest;
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
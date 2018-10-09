#include "renderCam.h"
#include <limits>
#include <iostream>
#include <cmath>

Ray RenderCam::getRay(float u, float v) 
{
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
}

void RenderCam::drawFrustum() {
	view.draw();
	Ray r1 = getRay(0, 0);
	Ray r2 = getRay(0, 1);
	Ray r3 = getRay(1, 1);
	Ray r4 = getRay(1, 0);
	float dist = glm::length((view.toWorld(0, 0) - position));
	r1.draw(dist);
	r2.draw(dist);
	r3.draw(dist);
	r4.draw(dist);
}

void RenderCam::renderImage(vector<SceneObject*> objects, ofImage *image, vector<Light*> lights)
{
	std::cout << "rendering with resolution: " << image->getWidth() << " x " << image->getHeight() << "\n";
	for (int i = 0; i < image->getWidth(); i++)
	{
		for (int j = 0; j < image->getHeight(); j++)
		{
			float u = (i + .5) / image->getWidth();
			float v = (j + .5) / image->getHeight();
			glm::vec3 point, normal, hitPoint, hitNormal;
			float shortestDistance = std::numeric_limits<float>::max();
			int index = -1;

			for (int k = 0; k < objects.size(); k++)
			{
				

				if (objects[k]->intersect(getRay(u, v), point, normal))
				{
					if (glm::length(point - position) < shortestDistance)
					{
						shortestDistance = glm::length(point - position);
						index = k;
						hitPoint = point;
						hitNormal = normal;
					}
				}
			}
			

			if (index == -1)
				image->setColor(i, (image->getHeight() -j) - 1, ofGetBackgroundColor());
			else
			{
				ofColor col = ofColor(0,0,0);
				for (int k = 0; k < lights.size(); k++)
				{
					if (!inShadow(objects, lights[k], hitPoint, hitNormal))
					{
						col = col + lambertian(objects[index], lights[k], hitPoint, hitNormal) + blinn_phong(objects[index], lights[k], hitPoint, hitNormal);
					}
				}

				image->setColor(i, (image->getHeight() - j) - 1, col  + (objects[index]->diffuseColor * ambientColor * ambientIntensity));
			}
			
		}
	}
	cout << "done\n";
}

ofColor RenderCam::lambertian(SceneObject * obj, Light * light, glm::vec3 point, glm::vec3 normal)
{
	return obj->diffuseColor *
			(light->intensity / std::pow(glm::length(light->position - point), 2)) *
			std::max(0.0f, glm::dot(glm::normalize(normal), glm::normalize(light->position - point))) *
			light->color;
}

ofColor RenderCam::blinn_phong(SceneObject *obj, Light *light, glm::vec3 point, glm::vec3 normal)
{
	glm::vec3 v = glm::normalize(this->position - point);
	glm::vec3 l = glm::normalize(light->position - point);
	return obj->specularColor *
			(light->intensity / std::pow(glm::length(light->position - point), 2)) *
			std::pow(std::max(0.0f, glm::dot(glm::normalize(normal), glm::normalize( (l + v) / glm::length(l + v) ))), obj->p) *
			light->color;
}

bool RenderCam::inShadow(vector<SceneObject*> objects, Light * light, glm::vec3 point, glm::vec3 normal)
{
	for (int i = 0; i < objects.size(); i++)
	{
		glm::vec3 p, n;
		if ( objects[i]->intersect(Ray((point + normal*.00005), glm::normalize(light->position - point)), p, n) )
			return true;
	}
	return false;
}

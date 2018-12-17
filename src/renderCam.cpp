//Austin Anderson
//010640955
//from code given by professor
#include "renderCam.h"
#include <iostream>
#include <cmath>

Ray RenderCam::getRay(float u, float v)
{
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
}

void RenderCam::drawFrustum() 
{
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

void RenderCam::renderImage(Scene scene, ofImage *image)
{
#if _DEBUG
	std::cout << "rendering with resolution: " << image->getWidth() << " x " << image->getHeight() << "\n";
#endif
	for (int i = 0; i < image->getWidth(); i++)
	{
		for (int j = 0; j < image->getHeight(); j++)
		{
			float u = (i + .5) / image->getWidth();
			float v = (j + .5) / image->getHeight();
			IntersectInfo intersect;
			SceneObject* hit = scene.intersect(this->getRay(u, v), intersect);


			if (!hit)  //if no object was hit, record the background color
				image->setColor(i, (image->getHeight() - j) - 1, ofGetBackgroundColor());
			else
			{
				ofColor col = ofColor(0, 0, 0);
				for (int k = 0; k < scene.lights.size(); k++)    //for all the lights in the scene
				{
					if (!inShadow(scene.objects, scene.lights[k], intersect))     //if the point isn't in a shadow
					{
						//cout << "not in shadow" << endl;
						col = col + lambertian(hit, scene.lights[k], intersect) + blinn_phong(hit, scene.lights[k], intersect); // the color is the current color plus the lambertian and blin-phong for this light
					}
				}

				image->setColor(i, (image->getHeight() - j) - 1, col + (hit->mat.diffuseColor * ambientColor * ambientIntensity));
			}

		}
	}
#if _DEBUG
	cout << "done\n";
#endif
}

ofColor RenderCam::lambertian(SceneObject * obj, Light * light, IntersectInfo intersect)
{
	ofColor col = obj->mat.diffuseColor *
		(light->intensity / std::pow(glm::length(light->position - intersect.point), 2)) *
		std::max(0.0f, glm::dot(glm::normalize(intersect.normal), glm::normalize(light->position - intersect.point))) *
		light->mat.diffuseColor;
	return col;
}

ofColor RenderCam::blinn_phong(SceneObject *obj, Light *light, IntersectInfo intersect)
{
	glm::vec3 v = glm::normalize(this->position - intersect.point);
	glm::vec3 l = glm::normalize(light->position - intersect.point);
	return obj->mat.specularColor *
		(light->intensity / std::pow(glm::length(light->position - intersect.point), 2)) *
		std::pow(std::max(0.0f, glm::dot(glm::normalize(intersect.normal), glm::normalize((l + v) / glm::length(l + v)))), obj->mat.p) *
		light->mat.diffuseColor;
}

bool RenderCam::inShadow(vector<SceneObject*> objects, Light * light, IntersectInfo intersect)
{
	for (int i = 0; i < objects.size(); i++)
	{
		IntersectInfo info;
		float dist = glm::length(light->position - intersect.point); //distance from intersect point to the light
		if (objects[i]->intersect(Ray((intersect.point + intersect.normal * .00005), glm::normalize(light->position - intersect.point)), info))
		{
			if (info.dist < dist)
			{
				return true;
			}
		}
	}
	return false;
}

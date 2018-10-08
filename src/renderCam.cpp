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
				ofColor col;

				for (int i = 0; i < lights.size(); i++)
				{
					col = objects[index]->diffuseColor * 
						(lights[i]->intensity/std::pow( glm::length(lights[i]->position - hitPoint), 2) ) *
							std::max(0.0f, glm::dot(glm::normalize(hitNormal), glm::normalize(lights[i]->position - hitPoint)));
					
					/*cout << "original Color: " << objects[index]->diffuseColor <<
							", intensity: " << (lights[i]->intensity / std::pow(glm::length(lights[i]->position - hitPoint), 2))  <<
							", dot product: (surface normal: " << hitNormal <<
							", light direction: " << glm::normalize(lights[i]->position - hitPoint) <<
							", product: "<< std::max(0.0f, glm::dot(glm::normalize(hitNormal), glm::normalize(lights[i]->position - hitPoint))) <<
							"), final color: " << col << "\n";*/
				}

				image->setColor(i, (image->getHeight() - j) - 1,
					col  + (col * ambientIntensity));
			}
			
		}
	}
}

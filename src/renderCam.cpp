//Austin Anderson
//010640955
//from code given by professor
#include "renderCam.h"



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

void RenderCam::renderImage(Scene scene, ofImage *image, bool antiAlias = false, bool multiThread = false)
{
#if _DEBUG
	std::cout << "rendering with resolution: " << image->getWidth() << " x " << image->getHeight() << "\n";
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
#endif
	if (multiThread)
	{
		vector<std::thread> threads;
		std::queue<glm::vec4> pieces;
		for (int i = 0; i < image->getWidth(); i += 16)
		{
			for (int j = 0; j < image->getHeight(); j += 16)
			{
				int endWidth = i + 16;
				int endHeight = j + 16;
				if (endWidth > image->getWidth())
					endWidth = endWidth > image->getWidth();
				if (endHeight > image->getHeight())
					endHeight = image->getHeight();

				pieces.push(glm::vec4(i, j, endWidth, endHeight));
			}
		}
#if _DEBUG
		std::cout << "number of Pieces: " << pieces.size() << endl;
#endif
		
		for (int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			threads.push_back(std::thread([&]() 
			{
				bool notEmpty = pieces.size() != 0;
				while (notEmpty)
				{
					popLock.lock();
					glm::vec4 piece = pieces.front();
					pieces.pop();
					popLock.unlock();
					renderImagePiece(scene, image, piece[0], piece[1], piece[2], piece[3], antiAlias);

					popLock.lock();
					notEmpty = pieces.size() != 0;
					popLock.unlock();
				}
			}));

			SetThreadPriority(threads[threads.size() - 1].native_handle(), THREAD_MODE_BACKGROUND_BEGIN);
			SetThreadPriority(threads[threads.size() - 1].native_handle(), -3);
		}

		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
	}

	else
	{
		for (int i = 0; i < image->getWidth(); i++)
		{
			for (int j = 0; j < image->getHeight(); j++)
			{
				if (antiAlias)
				{
					vector<ofColor> colors;
					for (float a = 0; a <= 1; a += .5)
					{
						for (float b = 0; b <= 1; b += .5)
						{
							float u = (i + a) / image->getWidth();
							float v = (j + b) / image->getHeight();

							colors.push_back(getColor(scene, u, v));
						}
					}
					image->setColor(i, (image->getHeight() - j) - 1, averageColors(colors));
				}
				else
				{
					float u = (i + .5) / image->getWidth();
					float v = (j + .5) / image->getHeight();

					ofColor col = this->getColor(scene, u, v);

					image->setColor(i, (image->getHeight() - j) - 1, col);
				}
			}
		}
	}

#if _DEBUG
	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	cout << "done, rendered in " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / (float)1000000000 << " seconds" << endl;
#endif
}

void RenderCam::renderImagePiece(Scene scene, ofImage *image, int startWidth, int startHeight, int endWidth, int endHeight, bool antiAlias)
{
	for (int i = startWidth; i < endWidth; i++)
	{
		for (int j = startHeight; j < endHeight; j++)
		{
			if (antiAlias)
			{
				vector<ofColor> colors;
				for (float a = 0; a <= 1; a += .5)
				{
					for (float b = 0; b <= 1; b += .5)
					{
						float u = ((i + a) / image->getWidth());
						float v = ((j + b) / image->getHeight());

						colors.push_back(getColor(scene, u, v));
					}
				}
				writeLock.lock();
				image->setColor(i, (image->getHeight() - j) - 1, averageColors(colors));
				writeLock.unlock();
			}
			else
			{
				float u = (i + .5) / image->getWidth();
				float v = (j + .5) / image->getHeight();

				ofColor col = this->getColor(scene, u, v);

				writeLock.lock();
				image->setColor(i, (image->getHeight() - j) - 1, col);
				writeLock.unlock();
			}
		}
	}
}


ofColor RenderCam::getColor(Scene scene, float u, float v)
{
	IntersectInfo intersect;
	SceneObject* hit = scene.intersect(this->getRay(u, v), intersect);


	if (!hit)  //if no object was hit, record the background color
		return ofGetBackgroundColor();
	else
	{
		ofColor col = ofColor(0, 0, 0);
		for (int k = 0; k < scene.lights.size(); k++)    //for all the lights in the scene
		{
			if (!inShadow(scene.objects, scene.lights[k], intersect))     //if the point isn't in a shadow
			{
				col = col + lambertian(hit, scene.lights[k], intersect) + blinn_phong(hit, scene.lights[k], intersect); // the color is the current color plus the lambertian and blin-phong for this light
			}
		}

		return col + (hit->mat.diffuseColor * ambientColor * ambientIntensity);
	}
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

void RenderCam::lambertian_t(ofColor col, SceneObject * obj, Light * light, IntersectInfo intersect)
{
	ofColor lamCol = obj->mat.diffuseColor *
		(light->intensity / std::pow(glm::length(light->position - intersect.point), 2)) *
		std::max(0.0f, glm::dot(glm::normalize(intersect.normal), glm::normalize(light->position - intersect.point))) *
		light->mat.diffuseColor;
	colLock.lock();
	col = col + lamCol;
	colLock.unlock();
}

void RenderCam::blinn_phong_t(ofColor col, SceneObject *obj, Light *light, IntersectInfo intersect)
{
	glm::vec3 v = glm::normalize(this->position - intersect.point);
	glm::vec3 l = glm::normalize(light->position - intersect.point);
	ofColor bpCol = obj->mat.specularColor *
		(light->intensity / std::pow(glm::length(light->position - intersect.point), 2)) *
		std::pow(std::max(0.0f, glm::dot(glm::normalize(intersect.normal), glm::normalize((l + v) / glm::length(l + v)))), obj->mat.p) *
		light->mat.diffuseColor;
	colLock.lock();
	col = col + bpCol;
	colLock.unlock();

}

bool RenderCam::inShadow(vector<SceneObject*> objects, Light * light, IntersectInfo intersect)
{
	float dist = glm::length(light->position - intersect.point); //distance from intersect point to the light
	for (int i = 0; i < objects.size(); i++)
	{
		IntersectInfo info;
		
		if (objects[i]->intersect(Ray((intersect.point + glm::normalize(intersect.normal) * .00005), glm::normalize(light->position - intersect.point)), info))
		{
			if (info.dist < dist)
			{
				return true;
			}
		}
	}
	return false;
}

ofColor RenderCam::averageColors(vector<ofColor> colors)
{
	ofColor col = colors[0];

	for (int i = 1; i < colors.size(); i++)
	{
		col = ofColor((col.r + colors[i].r) / 2, (col.g + colors[i].g) / 2, (col.b + colors[i].b) / 2, (col.a + colors[i].a) / 2);
	}
	return col;
}

//Austin Anderson
//010640955
#include "renderCam.h"


//get a ray from camera origin to point on viewplane, given by professor
Ray RenderCam::getRay(float u, float v)
{
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
}

//draw the view frustum, given by professor
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

//render the scene
void RenderCam::renderImage(Scene* scene, ofImage* image, bool antiAlias = false, bool multiThread = false)
{
	std::cout << "rendering with resolution: " << image->getWidth() << " x " << image->getHeight() << "\n" << endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	if (multiThread)
	{	
		//initilize the lists of threads and image pieces
		vector<std::thread> threads;
		std::queue<glm::vec4> pieces;
		
		//split the images into 16px x 16px pieces
		for (int i = 0; i < image->getWidth(); i += 16)
		{
			for (int j = 0; j < image->getHeight(); j += 16)
			{
				int endWidth = i + 16;
				int endHeight = j + 16;
				if (endWidth > image->getWidth())
					endWidth = image->getWidth();
				if (endHeight > image->getHeight())
					endHeight = image->getHeight();

				pieces.push(glm::vec4(i, j, endWidth, endHeight));
			}
		}
		pieceCount = pieces.size();

#if _DEBUG
		std::cout << "number of Pieces: " << pieces.size() << endl;
#endif
		//initilize threads to render the pieces, as many as the processor can handle
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

					cout << "\r[";
					for (int j = 0; j < (int)(50 * ((pieceCount-pieces.size())/pieceCount)); j++)
					{
						cout << "|";
					}
					for (int j = 0; j < 50 - (int)(50 * ((pieceCount - pieces.size()) / pieceCount)); j++)
					{
						cout << "-";
					}
					cout << "]";

					popLock.unlock();
				}
			}));

			//set thread priority lower so that it doesn't inturrupt other tasks, removed if in release
			SetThreadPriority(threads[threads.size() - 1].native_handle(), THREAD_MODE_BACKGROUND_BEGIN);
			SetThreadPriority(threads[threads.size() - 1].native_handle(), -10);
		}

		//join all the threads
		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
	}

	else
	{
		//render the whole image
		renderImagePiece(scene, image, 0, 0, image->getWidth(), image->getHeight(), antiAlias);
	}


	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / (float)1000000000;
	cout << "done, rendered in " << time << " seconds" << endl;
	image->save("render - [" + std::to_string((int)(image->getWidth())) + "x" + std::to_string((int)(image->getHeight())) + "] - " + std::to_string(time) + ".png", OF_IMAGE_QUALITY_BEST);
	image->load("render.png");
}
void RenderCam::renderImagePiece(Scene* scene, ofImage *image, int startWidth, int startHeight, int endWidth, int endHeight, bool antiAlias)
{
	for (int i = startWidth; i < endWidth; i++)
	{
		for (int j = startHeight; j < endHeight; j++)
		{
			if (antiAlias)
			{
				//initialize a vector to hold all the colors
				vector<ofColor> colors;

				//create 9 rays per pixel and get the colors for all of them
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
				//save the average for all those colors to the pixel
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

ofColor RenderCam::getColor(Scene* scene, float u, float v)
{
	IntersectInfo intersect;
	SceneObject* hit = scene->intersect(this->getRay(u, v), intersect);


	if (!hit)  //if no object was hit, record the background color
		return ofGetBackgroundColor();
	else
	{
		ofColor col = ofColor(0, 0, 0);
		for (int k = 0; k < scene->lights.size(); k++)    //for all the lights in the scene
		{
			if (!inShadow(scene->objects, scene->lights[k], &intersect))     //if the point isn't in a shadow
			{
				col = col + lambertian(hit, scene->lights[k], &intersect) + blinn_phong(hit, scene->lights[k], &intersect); // the color is the current color plus the lambertian and blin-phong for this light
			}
		}

		return col + (hit->mat.diffuseColor * ambientColor * ambientIntensity);
	}
}


ofColor RenderCam::lambertian(SceneObject* obj, Light* light, IntersectInfo* intersect)
{
	ofColor col = obj->mat.diffuseColor *
		(light->intensity / std::pow(glm::length(light->position - intersect->point), 2)) *
		std::max(0.0f, glm::dot(glm::normalize(intersect->normal), glm::normalize(light->position - intersect->point))) *
		light->mat.diffuseColor;
	return col;
}

ofColor RenderCam::blinn_phong(SceneObject* obj, Light* light, IntersectInfo* intersect)
{
	glm::vec3 v = glm::normalize(this->position - intersect->point);
	glm::vec3 l = glm::normalize(light->position - intersect->point);
	return obj->mat.specularColor *
		(light->intensity / std::pow(glm::length(light->position - intersect->point), 2)) *
		std::pow(std::max(0.0f, glm::dot(glm::normalize(intersect->normal), glm::normalize((l + v) / glm::length(l + v)))), obj->mat.p) *
		light->mat.diffuseColor;
}

bool RenderCam::inShadow(vector<SceneObject*> objects, Light * light, IntersectInfo* intersect)
{
	float dist = glm::length(light->position - intersect->point); //distance from intersect point to the light
	Ray toLight = Ray((intersect->point + glm::normalize(intersect->normal) * .00005), glm::normalize(light->position - intersect->point));
	for (int i = 0; i < objects.size(); i++)
	{
		IntersectInfo info;
		
		if (objects[i]->intersect(toLight, info))
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
	int r = colors[0].r;
	int g = colors[0].g;
	int b = colors[0].b;

	for (int i = 1; i < colors.size(); i++)
	{
		r += colors[i].r;
		g += colors[i].g;
		b += colors[i].b;
	}
	return ofColor(r/colors.size(), g/colors.size(), b/colors.size());
}
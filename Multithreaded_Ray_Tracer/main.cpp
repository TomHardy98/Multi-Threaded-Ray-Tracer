/// @file main.cpp
/// @brief Contains all the main functionality of the program

#include <iostream>   // Allows for the use of output to the command console
#include <fstream>   // Allows for the use of output to an image file
#include <memory>   // Allows for the use of shared pointers
#include <algorithm>   // Allows for the use of min and max functions
#include <vector>   // Allows for the use of a vector list
#include <glm.hpp>   // Allows for the use of vec3's
#include <math.h>   // Allows for the use of dot/cross product functions
#include <ppl.h>   // Allows for the use of parallel for loops
#include <thread>   // Allows for the use threads
#include <ctime>   // Allows for the use of the clock function

#include "Sphere.h"   // Sphere class include
#include "Plane.h"   // Plane class include
#include "Shape.h"   // Shape class include
#include "Ray.h"   // Ray class include

#define WINDOW_WIDTH (800)   // Macro for window width
#define WINDOW_HEIGHT (800)   // Macro for window height

void GameLoop(int _threadChoice);

std::vector<std::shared_ptr<Shape>> CreateShapes(std::vector<std::shared_ptr<Shape>> _shapeVector);

glm::vec3** DrawPixel(int _minX, int _maxX, int _minY, int _maxY, std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image);

void CreateAndJoinThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image, int _threadChoice);

void OutputImage(glm::vec3 **_image);

void UseOneThread(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image);

void UseFourThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image);

void UseSixteenThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image);

void UseSixtyFourThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image);

int main()
{
	std::cout << "Welcome to Tom Hardy's Multi-Threaded Ray Tracer" << std::endl;
	std::cout << "\n" << std::endl;

	int threadChoice = 0;

	std::cout << "How many threads would you like to run on? 1, 4, 16 or 64" << std::endl;
	std::cout << "\n" << std::endl;
	std::cin >> threadChoice;
	std::cout << "\n" << std::endl;

	switch (threadChoice)
	{
	case 1:
		GameLoop(threadChoice);
		break;
	case 4:
		GameLoop(threadChoice);
		break;
	case 16:
		GameLoop(threadChoice);
		break;
	case 64:
		GameLoop(threadChoice);
		break;
	default:
		std::cout << "Incorrect amount of threads chosen. Shutting down" << std::endl;
		std::cout << "\n" << std::endl;
		break;
	}

	system("PAUSE");

	return 0;
}

void GameLoop(int _threadChoice)
{
	std::clock_t startTimer = clock();

	std::vector<std::shared_ptr<Shape>> shapeVector = CreateShapes(shapeVector);   // Create a vector of shape data

	glm::vec3 **image = new glm::vec3*[WINDOW_WIDTH];

	for ( int i = 0; i < WINDOW_WIDTH; ++i )
	{
		image[i] = new glm::vec3[WINDOW_HEIGHT];
	}

	std::cout << "Firing rays.." << std::endl;
	std::cout << "\n" << std::endl;

	CreateAndJoinThreads(shapeVector, image, _threadChoice);   // Call the creation of threads

	std::cout << "Outputting image to folder.." << std::endl;
	std::cout << "\n" << std::endl;

	OutputImage(image);   // Call the output image function

	std::clock_t endTimer = clock();

	std::clock_t clockTicksTaken = endTimer - startTimer;

	float timeInSeconds = clockTicksTaken / (float)CLOCKS_PER_SEC;

	std::cout << "Time taken: " << timeInSeconds << " seconds" << std::endl;
	std::cout << "\n" << std::endl;
}

std::vector<std::shared_ptr<Shape>> CreateShapes(std::vector<std::shared_ptr<Shape>> _shapeVector)
{
	_shapeVector.push_back(std::make_shared<Sphere>(glm::vec3(-2.0, 0, -7), 2, glm::vec3(0.82f, 1.00f, 0.87f)));   // Green
	_shapeVector.push_back(std::make_shared<Sphere>(glm::vec3(0.5, 0, -10), 2, glm::vec3(1.00f, 0.95f, 0.82f)));   // Yellow
	_shapeVector.push_back(std::make_shared<Sphere>(glm::vec3(3.5, 0, -13), 2, glm::vec3(1.00f, 0.87f, 0.82f)));   // Orange
	_shapeVector.push_back(std::make_shared<Sphere>(glm::vec3(6.5, 0, -16), 2, glm::vec3(0.87f, 0.82f, 1.00f)));   // Purple
	_shapeVector.push_back(std::make_shared<Plane>(glm::vec3(0, -4, 0), glm::vec3(0, 1, 0), glm::vec3(0.57f, 0.57f, 0.57f)));   // Floor

	return _shapeVector;
}

glm::vec3** DrawPixel(int _minX, int _maxX, int _minY, int _maxY, std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image)
{
	for ( int x = _minX; x < _maxX; ++x )   // Each pixel is looping parallel to one another to decrease rendering time
	{
		for ( int y = _minY; y < _maxY; ++y )
		{
			float pixNormalX = (x + 0.5f) / WINDOW_WIDTH;   // Normalising pixel position so ray passes through center of pixel
			float pixNormalY = (y + 0.5f) / WINDOW_HEIGHT;   // Normalising pixel position so ray passes through center of pixel

			float pixRemapX = (2.0f * pixNormalX - 1.0f);   // Remap coordinates to reverse the direction of the y axis
			float pixRemapY = 1.0f - 2.0f * pixNormalY;   // Remap coordinates to reverse the direction of the y axis

			float pixCameraX = pixRemapX * tan(glm::radians(90.0f) / 2.0f);   // Create a field of view with the camera at 90 (Standard for games)
			float pixCameraY = pixRemapY * tan(glm::radians(90.0f) / 2.0f);   // Create a field of view with the camera at 90 (Standard for games)

			glm::vec3 pCameraSpace = glm::vec3(pixCameraX, pixCameraY, -1);   // The point lies 1 unit away from the camera origin

			std::shared_ptr<Ray> ray = std::make_shared<Ray>();
			ray->setOrigin(glm::vec3(0, 0, 0));
			ray->setDirection(glm::normalize(pCameraSpace - ray->getOrigin()));

			float minT = INFINITY;
			int shapeHit = -1;
			float t0 = 0.0f;

			for ( int k = 0; k < _shapeVector.size(); ++k )   // Loop through the shape vector
			{
				bool hit = _shapeVector[k]->Intersection(ray->getOrigin(), ray->getDirection(), &t0);   // If there is an intersection return true on hit

				if ( hit && t0 < minT )
				{
					minT = t0;
					shapeHit = k;
				}

				if ( shapeHit != -1 )   // If a shape is hit
				{
					// Calculating 'Phong lighting' using specular and diffuse

					glm::vec3 p0 = ray->getOrigin() + (minT * ray->getDirection());

					glm::vec3 lightPosition = glm::vec3(25, 155, -2);
					glm::vec3 lightIntensity = glm::vec3(1.0, 1.0, 1.0);

					glm::vec3 diffuseColour = glm::vec3(0, 0, 0);
					glm::vec3 specularColour = glm::vec3(0, 0, 0);
					int shininess = 0;


					glm::vec3 normal = glm::normalize(_shapeVector[shapeHit]->CalculateNormal(p0, &shininess, &diffuseColour, &specularColour));

					glm::vec3 lightRay = glm::normalize(lightPosition - p0);

					glm::vec3 diffuse = diffuseColour * lightIntensity * glm::max(0.0f, dot(lightRay, normal));

					glm::vec3 reflection = glm::normalize(2 * (dot(lightRay, normal)) * normal - lightRay);

					float maxCalc = glm::max(0.0f, dot(reflection, glm::normalize(ray->getOrigin() - p0)));

					glm::vec3 specular = specularColour * lightIntensity * pow(maxCalc, shininess);

					int lightHitShape = 0;

					for ( int i = 0; i < _shapeVector.size(); ++i )
					{
						bool lightingHit = _shapeVector[i]->Intersection(p0 + (1e-4f * normal), lightRay, &t0);

						if ( lightingHit && t0 < minT )
						{
							minT = t0;
							lightHitShape = 1;
						}
					}

					if ( lightHitShape != 0 )
					{
						_image[x][y] = glm::vec3(0.1, 0.1, 0.1);   // Setting it to almost black for the shadows
					}
					else
					{
						_image[x][y] = diffuse + specular;
					}
				}
				else
				{
					_image[x][y] = glm::vec3(0.76, 0.93, 0.93);   // If there is no object data and no collision has occured then set pixel to sky blue
				}
			}
		}
	}

	return _image;
}

void CreateAndJoinThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image, int _threadChoice)
{
	if (_threadChoice == 1)
	{
		UseOneThread(_shapeVector, _image);
	}

	if (_threadChoice == 4)
	{
		UseFourThreads(_shapeVector, _image);
	}

	if (_threadChoice == 16)
	{
		UseSixteenThreads(_shapeVector, _image);
	}

	if (_threadChoice == 64)
	{
		UseSixtyFourThreads(_shapeVector, _image);
	}
}

void OutputImage(glm::vec3 **_image)
{
	std::ofstream ofs("../RayTracingImage.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";

	for ( unsigned y = 0; y < WINDOW_HEIGHT; ++y )
	{
		for ( unsigned x = 0; x < WINDOW_WIDTH; ++x )
		{
			ofs << (unsigned char)(std::min((float)1, (float)_image[x][y].x) * 255) <<
				(unsigned char)(std::min((float)1, (float)_image[x][y].y) * 255) <<
				(unsigned char)(std::min((float)1, (float)_image[x][y].z) * 255);
		}
	}
	ofs.close();
}

void UseOneThread(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image)
{
	// Did attempt the use of for loops to clean the code up but it slowed down the program to the point where 4 threads were faster than 64

	std::thread t1(DrawPixel, 0, 800, 0, 800, _shapeVector, _image);

	t1.join();
}

void UseFourThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image)
{
	// Did attempt the use of for loops to clean the code up but it slowed down the program to the point where 4 threads were faster than 64

	std::vector<std::shared_ptr<std::thread>> threadVector;   // Create a vector to hold all the chunks of the screen

	// Split screen into quads
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 400, 0, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 800, 0, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 400, 400, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 800, 400, 800, _shapeVector, _image));
	
	for ( int i = 0; i < threadVector.size(); ++i )   // Loop through the thread vector
	{
		threadVector.at(i)->join();   // Call join on all the threads
	}
}

void UseSixteenThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image)
{
	// Did attempt the use of for loops to clean the code up but it slowed down the program to the point where 4 threads were faster than 64

	std::vector<std::shared_ptr<std::thread>> threadVector;   // Create a vector to hold all the chunks of the screen

	// Top row (1)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 200, 0, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 400, 0, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 600, 0, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 800, 0, 200, _shapeVector, _image));

	// Middle row (2)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 200, 200, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 400, 200, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 600, 200, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 800, 200, 400, _shapeVector, _image));

	// Middle row (3)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 200, 400, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 400, 400, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 600, 400, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 800, 400, 600, _shapeVector, _image));

	// Bottom row (4)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 200, 600, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 400, 600, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 600, 600, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 800, 600, 800, _shapeVector, _image));

	for ( int i = 0; i < threadVector.size(); ++i )   // Loop through the thread vector
	{
		threadVector.at(i)->join();   // Call join on all the threads
	}
}

void UseSixtyFourThreads(std::vector<std::shared_ptr<Shape>> _shapeVector, glm::vec3 **_image)
{
	// Did attempt the use of for loops to clean the code up but it slowed down the program to the point where 4 threads were faster than 64

	std::vector<std::shared_ptr<std::thread>> threadVector;   // Create a vector to hold all the chunks of the screen

	// Top row (1)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 0, 100, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 0, 100, _shapeVector, _image));
	
	// Middle row (2)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 100, 200, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 100, 200, _shapeVector, _image));

	// Middle row (3)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 200, 300, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 200, 300, _shapeVector, _image));

	// Middle row (4)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 300, 400, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 300, 400, _shapeVector, _image));

	// Middle row (5)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 400, 500, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 400, 500, _shapeVector, _image));

	// Middle row (6)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 500, 600, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 500, 600, _shapeVector, _image));

	// Middle row (7)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 600, 700, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 600, 700, _shapeVector, _image));

	// Bottom row (8)
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 0, 100, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 100, 200, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 200, 300, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 300, 400, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 400, 500, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 500, 600, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 600, 700, 700, 800, _shapeVector, _image));
	threadVector.push_back(std::make_shared<std::thread>(DrawPixel, 700, 800, 700, 800, _shapeVector, _image));

	for ( int i = 0; i < threadVector.size(); ++i )   // Loop through the thread vector
	{
		threadVector.at(i)->join();   // Call join on all the threads
	}
}
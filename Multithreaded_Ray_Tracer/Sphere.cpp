/// @file Sphere.cpp
/// @brief Contains functions for sphere object/class

#include <glm.hpp>

#include "Sphere.h"

Sphere::Sphere()
{
	setPosition(glm::vec3(0, 0, 0));
	m_radius = 0;
	setColour(glm::vec3(0, 0, 0));
}

Sphere::Sphere(glm::vec3 _position, float _radius, glm::vec3 _colour)
{
	setPosition(_position);
	m_radius = _radius;
	setColour(_colour);
}

/// http://sci.tuomastonteri.fi/programming/sse/example3 used for help with Sphere Intersection function
bool Sphere::Intersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float *t)
{
	glm::vec3 L = getPosition() - rayOrigin;

	float delta = dot(L, rayDirection);

	if (delta < 0)   // There is no intersection
	{
		return false;
	}
	else
	{
		float s2 = (dot(L, L)) - (delta * delta);
		float s = sqrt(s2);

		if (s > m_radius)   // There is no intersection
		{
			return false;
		}
		else   // There has been an intersection
		{
			float thc = sqrt((m_radius * m_radius) - s2);
			float t0 = delta - thc;
			*t = t0;

			return true;
		}
	}
}

glm::vec3 Sphere::CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour)
{
	*shininess = 64;
	*diffuseColour = getColour();
	*specularColour = glm::vec3(0.7, 0.7, 0.7);
	return (_p0 - getPosition());
}

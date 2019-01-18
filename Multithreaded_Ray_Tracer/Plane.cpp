/// @file Plane.cpp
/// @brief Contains functions for Plane object/class

#include <glm.hpp>

#include "Plane.h"

Plane::Plane()
{
	m_planeNormal = glm::vec3(0, 0, 0);
}

Plane::Plane(glm::vec3 _position, glm::vec3 _normal, glm::vec3 _colour)
{
	setPosition(_position);
	m_planeNormal = _normal;
	setColour(_colour);
}

/// http://www.geomalgorithms.com/a05-_intersect-1.html used for help with Plane Intersection function
bool Plane::Intersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float *t)
{
	float denom = dot(rayDirection, m_planeNormal);   // Calculates dot product between rayDirection and planeNormal

	if (abs(denom) < 0.000001f)   // Returns the absolute value of denom
	{
		return false;
	}
	else
	{
		float result = dot((getPosition() - rayOrigin), m_planeNormal) / denom;
		*t = result;

		return *t >= 0;
	}
}

glm::vec3 Plane::CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour)
{
	*shininess = 10;
	*diffuseColour = glm::vec3(0.3, 0.3, 0.3);
	*specularColour = getColour();
	return m_planeNormal;
}
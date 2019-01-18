/// \file Sphere.h
/// \brief Class for the 'Sphere' object
/// \author Thomas Hardy

#ifndef SPHERE_H
#define SPHERE_H

#include <glm.hpp>

#include "Shape.h"

class Sphere : public Shape
{
public:

	Sphere();

	Sphere(glm::vec3 _position, float _radius, glm::vec3 _colour);

	bool Intersection(glm::vec3 _rayOrigin, glm::vec3 _rayDirection, float *t);

	glm::vec3 CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour);

	float getRadius() { return m_radius; }
	void setRadius( float _radius ) { m_radius = _radius; }

private:

	float m_radius;
};
#endif
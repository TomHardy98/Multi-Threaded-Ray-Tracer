/// \file Plane.h
/// \brief Class for the 'Plane' object
/// \author Thomas Hardy

#ifndef PLANE_H
#define PLANE_H

#include <glm.hpp>

#include "Shape.h"

class Plane : public Shape
{
public:

	Plane();

	Plane(glm::vec3 _position, glm::vec3 _normal, glm::vec3 _colour);

	bool Intersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float *t);

	glm::vec3 CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour);

	glm::vec3 getPlaneNormal() { return m_planeNormal; }
	void setPlaneNormal( glm::vec3 _planeNormal ) { m_planeNormal = _planeNormal; }

private:

	glm::vec3 m_planeNormal;

};
#endif
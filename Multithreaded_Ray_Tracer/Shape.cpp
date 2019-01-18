/// @file Shape.cpp
/// @brief Contains functions that get overriden for Shape class

#include <glm.hpp>

#include "Shape.h"

Shape::Shape()
{
	m_position = glm::vec3(0, 0, 0);
	m_colour = glm::vec3(0, 0, 0);
	m_normal = glm::vec3(0, 0, 0);
}

Shape::Shape(glm::vec3 _position, glm::vec3 _normal, glm::vec3 _colour)
{
	m_position = _position;
	m_colour = _colour;
	m_normal = _normal;
}

bool Shape::Intersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float *t)
{
	return false;
}

glm::vec3 Shape::CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour)
{
	return m_normal;
}
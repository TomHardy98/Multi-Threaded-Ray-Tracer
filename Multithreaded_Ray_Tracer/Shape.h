/// \file Shape.h
/// \brief Class for the 'Shape' which all objects inherit from
/// \author Thomas Hardy

#ifndef SHAPE_H
#define SHAPE_H

#include <glm.hpp>

class Shape
{
public:

	Shape();

	Shape(glm::vec3 _position, glm::vec3 _normal, glm::vec3 _colour);

	virtual bool Intersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float *t);

	virtual glm::vec3 CalculateNormal(glm::vec3 _p0, int *shininess, glm::vec3 *diffuseColour, glm::vec3 *specularColour);   // Virtual function to be overriden by inheritance

	glm::vec3 getPosition() { return m_position; }
	void setPosition( glm::vec3 _position ) { m_position = _position; }

	glm::vec3 getColour() { return m_colour; }
	void setColour( glm::vec3 _colour ) { m_colour = _colour; }

	glm::vec3 getNormal() { return m_normal; }
	void setNormal( glm::vec3 _normal ) { m_normal = _normal; }

private:

	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec3 m_normal;
};
#endif
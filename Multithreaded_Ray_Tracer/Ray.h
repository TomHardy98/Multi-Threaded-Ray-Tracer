/// \file Ray.h
/// \brief Class for the 'Ray' object
/// \author Thomas Hardy

#ifndef RAY_H
#define RAY_H

#include <glm.hpp>

class Ray
{
public:

	Ray();
	~Ray();

	glm::vec3 getDirection() { return m_direction; }
	void setDirection( glm::vec3 _direction ) { m_direction = _direction; }

	glm::vec3 getOrigin() { return m_origin; }
	void setOrigin( glm::vec3 _origin ) { m_origin = _origin; }

private:

	glm::vec3 m_direction;
	glm::vec3 m_origin;
};
#endif
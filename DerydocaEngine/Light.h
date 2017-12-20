#pragma once
#include "glm\glm.hpp"
#include "GameComponent.h"

class Light : public GameComponent
{
public:
	/* Defines a light's type */
	enum LightType
	{
		/* A orthagonal light infinitely far away */
		Directional,
		/* A light projected in a frustum */
		Spotlight,
		/* A light radiating in all directions */
		Point
	};

	Light();
	~Light();

	void setLightType(LightType type) { m_lightType = type; }
	LightType getLightType() { return m_lightType; }

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
private:
	LightType m_lightType = Point;
};


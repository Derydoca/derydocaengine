#include "Light.h"
#include "LightManager.h"


Light::Light()
{
	LightManager::getInstance().addLight(this);
}


Light::~Light()
{
	LightManager::getInstance().removeLight(this);
}

glm::mat4 Light::getProjectionMatrix()
{
	return glm::mat4();
}

glm::mat4 Light::getViewMatrix()
{
	return glm::mat4();
}

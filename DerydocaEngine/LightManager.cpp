#include "LightManager.h"
#include "CameraManager.h"
#include "glm\glm.hpp"

void LightManager::bindLightsToShader(Transform* objectTransform, Shader* shader)
{
	assert(objectTransform);
	assert(shader);

	static float rotationVal = 0.0f;

	rotationVal += 0.05f;
	float x = cos(rotationVal);

	// Get a list of lights that will affect the object being sent in
	std::list<Light*> lights = getLights(objectTransform);

	// Loop through each light and bind them to the shader
	int lightIndex = 0;
	for each (Light* light in lights)
	{
		// Set the intensity
		std::string intensityName = "lights[" + std::to_string(lightIndex) + "].Intensity";
		shader->setColorRGB(intensityName, light->getColor());

		// Set the position
		glm::vec3 lightWorldPos = light->getGameObject()->getTransform()->getWorldPos();
		glm::vec4 worldPosition4 = glm::vec4(lightWorldPos, 1);
		Camera* currentCamera = CameraManager::getInstance().getCurrentCamera();
		glm::mat4 lightMvp = currentCamera->getProjectionMatrix() * currentCamera->getViewMatrix();
		glm::vec4 screenPosition = lightMvp * worldPosition4;
		screenPosition *= -1;
		std::string positionName = "lights[" + std::to_string(lightIndex) + "].Position";
		shader->setVec3(positionName, screenPosition);

		//HACK
		if (lightIndex == 0)
		{
			shader->setVec4("LightPosition", worldPosition4);
			shader->setColorRGB("Kd", light->getColor());
			//auto camPos = CameraManager::getInstance().getCurrentCamera()->getGameObject()->getTransform()->getPos();
			//printf("CAMERA: %f, %f, %f\n", camPos.x, camPos.y, camPos.z);
			printf("LIGHT:  %f, %f, %f\n", worldPosition4.x, worldPosition4.y, worldPosition4.z);
		}

		// Increase our light index
		lightIndex++;
	}
}

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

std::list<Light*> LightManager::getLights(Transform * objectTransform)
{
	std::list<Light*> lights = std::list<Light*>();
	int numLights = 0;
	for each (Light* light in m_lights)
	{
		lights.push_back(light);
		numLights++;
		if (numLights >= MAX_LIGHTS) {
			break;
		}
	}
	return lights;
}

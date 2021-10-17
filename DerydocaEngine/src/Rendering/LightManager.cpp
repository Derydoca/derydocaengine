#include "EnginePch.h"
#include "Rendering\LightManager.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "GameObject.h"
#include "Components\Light.h"
#include "Rendering\Shader.h"
#include "Components\Transform.h"

namespace DerydocaEngine::Rendering
{

	void LightManager::bindLightsToShader(const std::shared_ptr<Rendering::Shader>& shader)
	{
		shader->bindUniformBuffer("LightCollection", m_lightUniformBuffer.getRendererId());
	}

	void LightManager::uploadLightUniformBufferData(const std::shared_ptr<Components::Transform> objectTransform)
	{
		auto currentCamera = CameraManager::getInstance().getCurrentCamera();
		glm::mat4 cameraModelMat = currentCamera->getGameObject()->getTransform()->getModel();
		glm::mat4 viewMat = currentCamera->getProjection().getViewMatrix(cameraModelMat);

		auto lights = getLights(objectTransform);

		int limit = lights.size() > MAX_LIGHTS ? MAX_LIGHTS : static_cast<int>(lights.size());
		for (int i = 0; i < limit; i++)
		{
			auto l = lights[i];
			auto ld = &m_lightData.Lights[i];
			auto lightTransform = l->getGameObject()->getTransform();


			glm::vec3 lightWorldPos = l->getGameObject()->getTransform()->getWorldPosition();
			glm::vec4 lightPositionEyeCoords = viewMat * l->getGameObject()->getTransform()->getWorldModel() * glm::vec4(lightWorldPos, 1);

			ld->Cutoff = l->getSpotlightCutoff();
			ld->Direction = glm::vec4(glm::normalize(glm::vec3(viewMat * lightTransform->getWorldModel() * glm::vec4(0, 1, 0, 0))), 1.0f);
			ld->Exponent = l->getSpotlightExponent();
			ld->Intensity = l->getColor().toVec4();
			ld->Position = lightPositionEyeCoords;
			ld->Type = static_cast<int>(l->getLightType());
		}
		m_lightData.NumLights = limit;

		m_lightUniformBuffer.uploadData(&m_lightData);
	}

	LightManager::LightManager()
	{
		m_lightUniformBuffer.create();
	}

	LightManager::~LightManager()
	{
		m_lightUniformBuffer.destroy();
	}

	std::vector<std::shared_ptr<Components::Light>> LightManager::getLights(std::shared_ptr<Components::Transform> const& cameraTransform) const
	{
		// Create a list to store the lights
		auto lights = std::vector<std::shared_ptr<Components::Light>>();

		// Go through each light
		int numLights = 0;
		for each (auto light in m_lights)
		{
			// TODO: Only include lights that would potentially effect this object

			// Add the light to the list
			lights.push_back(light.lock());

			// If we are at the maximum number of supported lights, lets end it early
			numLights++;
			if (numLights >= MAX_LIGHTS) {
				break;
			}
		}

		// Return the list of lights
		return lights;
	}

}

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

	void LightManager::bindShadowDataToShader(const std::shared_ptr<Rendering::Shader> shader, const glm::mat4& modelMatrix)
	{
		auto lights = getLights(nullptr);

		int lightIndex = 0;
		char indexStringBuffer[3];
		for each (auto light in lights)
		{
			if (light->isCastingShadows())
			{
				_itoa(lightIndex, indexStringBuffer, 10);

				// Set the shadow map
				std::string shadowMapName = std::string("ShadowMap[").append(indexStringBuffer).append("]");
				int shadowMapTextureUnit = 10 + lightIndex; // Find a better way to get an index for this
				shader->setTexture(shadowMapName, shadowMapTextureUnit, GL_TEXTURE_2D, light->getShadowMap());

				// Set the shadow softness
				std::string shadowSoftnessName = std::string("ShadowSoftness[").append(indexStringBuffer).append("]");
				shader->setFloat(shadowSoftnessName, light->getShadowSoftness());
				
				// Set the shadow matrix
				std::string shadowMatrixName = std::string("ShadowMatrix[").append(indexStringBuffer).append("]");
				shader->setMat4(shadowMatrixName, light->getShadowMatrix() * modelMatrix);

				// Increase our light index
				lightIndex++;
			}
		}

		// Set the shadow jitter texture
		int shadowJitterTextureUnit = 30; // Find a better way to get an index for this
		shader->setTexture("ShadowJitterTex", shadowJitterTextureUnit, GL_TEXTURE_3D, m_shadowJitterTexture);

		// Set the shadow jitter texture size
		shader->setVec3("ShadowJitterTexSize", m_shadowJitterTextureSize);
	}

	void LightManager::renderShadowMaps(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, std::shared_ptr<Components::Transform> cameraTransform)
	{
		// Get a list of lights that are visible by the camera
		auto lights = getLights(cameraTransform);

		// Render the shadown map for each light
		for each (auto light in lights)
		{
			if (light->isCastingShadows())
			{
				light->renderShadowMap(scenes);
			}
		}
	}

	void LightManager::uploadLightUniformBufferData(const std::shared_ptr<Components::Transform> objectTransform)
	{
		auto currentCamera = CameraManager::getInstance().getCurrentCamera();
		if (currentCamera == nullptr)
		{
			return;
		}
		glm::mat4 cameraModelMat = currentCamera->getGameObject()->getTransform()->getModel();
		glm::mat4 viewMat = currentCamera->getProjection().getViewMatrix(cameraModelMat);

		auto lights = getLights(objectTransform);

		int limit = lights.size() > MAX_LIGHTS ? MAX_LIGHTS : lights.size();
		for (int i = 0; i < limit; i++)
		{
			auto l = lights[i];
			auto ld = &m_lightData.Lights[i];
			auto lightTransform = l->getGameObject()->getTransform();


			glm::vec3 lightWorldPos = l->getGameObject()->getTransform()->getWorldPos();
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
		buildOffsetTex(8, 4, 8);
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

	float jitter()
	{
		return ((float)rand() / RAND_MAX) - 0.5f;
	}

	void LightManager::buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV)
	{
		int size = texSize;
		int samples = samplesU * samplesV;
		int bufSize = size * size * samples * 2;
		float *data = new float[bufSize];

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < samples; k += 2)
				{
					int x1, y1, x2, y2;
					x1 = k % (samplesU);
					y1 = (samples - 1 - k) / samplesU;
					x2 = (k + 1) % samplesU;
					y2 = (samples - 1 - k - 1) / samplesU;

					glm::vec4 v;
					// Center on grid and jitter
					v.x = (x1 + 0.5f) + jitter();
					v.y = (y1 + 0.5f) + jitter();
					v.z = (x2 + 0.5f) + jitter();
					v.w = (y2 + 0.5f) + jitter();

					// Scale between 0 and 1
					v.x /= samplesU;
					v.y /= samplesV;
					v.z /= samplesU;
					v.w /= samplesV;

					// Warp to disk
					int cell = ((k / 2) * size * size + j * size + i) * 4;
					data[cell + 0] = sqrtf(v.y) * cosf(glm::two_pi<float>() * v.x);
					data[cell + 1] = sqrtf(v.y) * sinf(glm::two_pi<float>() * v.x);
					data[cell + 2] = sqrtf(v.w) * cosf(glm::two_pi<float>() * v.z);
					data[cell + 3] = sqrtf(v.w) * sinf(glm::two_pi<float>() * v.z);
				}
			}
		}

		glActiveTexture(GL_TEXTURE9);
		glGenTextures(1, &m_shadowJitterTexture);

		glBindTexture(GL_TEXTURE_3D, m_shadowJitterTexture);
		glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA32F, size, size, samples / 2);
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, size, size, samples / 2, GL_RGBA, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		delete[] data;

		m_shadowJitterTextureSize = glm::vec3(texSize, texSize, samplesU * samplesV / 2.0f);
	}

}

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

	void LightManager::bindLightsToShader(
		std::shared_ptr<Rendering::MatrixStack> const& matrixStack,
		std::shared_ptr<Components::Transform> const& objectTransform,
		std::shared_ptr<Rendering::Shader> const& shader
	)
	{
		assert(objectTransform);
		assert(shader);

		// Get a list of lights that will affect the object being sent in
		auto lights = getLights(objectTransform);

		// Cache some things
		auto currentCamera = CameraManager::getInstance().getCurrentCamera();
		glm::mat4 cameraModelMat = currentCamera->getGameObject()->getTransform()->getModel();
		glm::mat4 viewMat = currentCamera->getProjection().getViewMatrix(cameraModelMat);

		// Loop through each light and bind them to the shader
		int lightIndex = 0;
		for each (auto light in lights)
		{
			Components::Light::LightType lightType = light->getLightType();

			if (lightType == Components::Light::Directional || lightType == Components::Light::Spotlight)
			{
				// Set the light direction
				std::string typeName = "Lights[" + std::to_string(lightIndex) + "].Direction";
				glm::vec3 lightDirection = glm::normalize(glm::vec3(viewMat * light->getGameObject()->getTransform()->getWorldModel() * glm::vec4(0, 1, 0, 0)));
				shader->setVec3(typeName, lightDirection);
			}

			if (lightType == Components::Light::Spotlight)
			{
				// Set the spotlight exponent
				std::string exponentName = "Lights[" + std::to_string(lightIndex) + "].Exponent";
				shader->setFloat(exponentName, light->getSpotlightExponent());

				// Set the spotlight cutoff
				std::string cutoffName = "Lights[" + std::to_string(lightIndex) + "].Cutoff";
				shader->setFloat(cutoffName, light->getSpotlightCutoff());
			}

			// Set the light type
			std::string typeName = "Lights[" + std::to_string(lightIndex) + "].Type";
			shader->setInt(typeName, (int)lightType);

			// Set the Intensity
			std::string intensityName = "Lights[" + std::to_string(lightIndex) + "].Intensity";
			shader->setColorRGBA(intensityName, light->getColor());

			// Set the Ambient
			std::string ambientName = "Lights[" + std::to_string(lightIndex) + "].La";
			shader->setColorRGBA(ambientName, Color(1.0, 1.0, 1.0, 1.0));

			// Set the Diffuse
			std::string diffuseName = "Lights[" + std::to_string(lightIndex) + "].Ld";
			shader->setColorRGBA(diffuseName, light->getColor());

			// Set the Specular
			std::string specularName = "Lights[" + std::to_string(lightIndex) + "].Ls";
			shader->setColorRGBA(specularName, Color(1.0, 1.0, 1.0, 1.0));

			// Set the position
			glm::vec3 lightWorldPos = light->getGameObject()->getTransform()->getWorldPos();
			glm::vec4 lightPositionEyeCoords = viewMat * light->getGameObject()->getTransform()->getWorldModel() * glm::vec4(lightWorldPos, 1);
			std::string positionName = "Lights[" + std::to_string(lightIndex) + "].Position";
			shader->setVec4(positionName, lightPositionEyeCoords);

			// If the light is casting shadows, set the related uniforms for it
			if (light->isCastingShadows())
			{
				// Set the shadow map
				std::string shadowMapName = "ShadowMaps[" + std::to_string(lightIndex) + "]";
				int shadowMapTextureUnit = 10 + lightIndex; // Find a better way to get an index for this
				shader->setTexture(shadowMapName, shadowMapTextureUnit, GL_TEXTURE_2D, light->getShadowMap());

				// Set the shadow softness
				std::string shadowSoftnessName = "Lights[" + std::to_string(lightIndex) + "].ShadowSoftness";
				shader->setFloat(shadowSoftnessName, light->getShadowSoftness());

				// Set the shadow matrix
				if (matrixStack)
				{
					std::string shadowMatrixName = "Lights[" + std::to_string(lightIndex) + "].ShadowMatrix";
					shader->setMat4(shadowMatrixName, light->getShadowMatrix(matrixStack->getMatrix()));
				}
			}

			// Increase our light index
			lightIndex++;
		}

		// Set the shadow jitter texture
		int shadowJitterTextureUnit = 30; // Find a better way to get an index for this
		shader->setTexture("ShadowJitterTex", shadowJitterTextureUnit, GL_TEXTURE_3D, m_shadowJitterTexture);

		// Set the shadow jitter texture size
		shader->setVec3("ShadowJitterTexSize", m_shadowJitterTextureSize);

		shader->setInt("LightCount", (int)lights.size());
	}

	void LightManager::renderShadowMaps(std::shared_ptr<Components::Transform> const& objectTransform)
	{
		// Get a list of lights that will affect the object being sent in
		auto lights = getLights(objectTransform);

		for each (auto light in lights)
		{
			if (light->isCastingShadows())
			{
				light->renderShadowMap(objectTransform->getGameObject());
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	LightManager::LightManager()
	{
		buildOffsetTex(8, 4, 8);
	}

	LightManager::~LightManager()
	{
	}

	std::list<std::shared_ptr<Components::Light>> LightManager::getLights(std::shared_ptr<Components::Transform> const& objectTransform) const
	{
		// Create a list to store the lights
		auto lights = std::list<std::shared_ptr<Components::Light>>();

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

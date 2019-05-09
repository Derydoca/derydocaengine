#pragma once
#include <glm/vec3.hpp>
#include <list>
#include <memory>
#include "Scenes\Scene.h"
#include "UniformBuffer.h"

namespace DerydocaEngine {
	namespace Components {
		class Light;
		struct Transform;
	}
	namespace Rendering {
		class MatrixStack;
		class Shader;
	}
}

namespace DerydocaEngine::Rendering
{

	struct ubo_light_data
	{
		glm::vec4 Direction;
		glm::vec4 Position;
		glm::vec4 Intensity;
		int Type;
		float Cutoff;
		float Exponent;
		float _padding;
	};

	struct ubo_light_collection
	{
		ubo_light_data Lights[10];
		int NumLights;
	};

	class LightManager
	{
	public:
		static LightManager& getInstance()
		{
			static LightManager instance;
			return instance;
		}

		void addLight(std::weak_ptr<Components::Light> const& light) { m_lights.push_back(light); }
		void bindLightsToShader(const std::shared_ptr<Rendering::Shader>& shader);
		void bindShadowDataToShader(const std::shared_ptr<Rendering::Shader> shader, const glm::mat4& modelMatrix);
		void removeLight(std::weak_ptr<Components::Light> const& light) {
			if (m_lights.size() == 0)
			{
				return;
			}
			auto lightRef = light.lock();
			m_lights.remove_if([lightRef](std::weak_ptr<Components::Light> l) {
				auto otherLightRef = l.lock();
				if (lightRef && otherLightRef)
				{
					return lightRef == otherLightRef;
				}
				return false;
			});
		}
		void renderShadowMaps(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, std::shared_ptr<Components::Transform> cameraTransform);
		void uploadLightUniformBufferData(const std::shared_ptr<Components::Transform> objectTransform);

		void operator=(LightManager const&) = delete;

	public:
		static const int MAX_LIGHTS = 10;

	private:
		std::list<std::weak_ptr<Components::Light>> m_lights;
		unsigned int m_shadowJitterTexture;
		glm::vec3 m_shadowJitterTextureSize;
		UniformBuffer<ubo_light_collection> m_lightUniformBuffer;
		ubo_light_collection m_lightData;

		LightManager();
		LightManager(LightManager const&);
		~LightManager();

		void buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV);
		std::vector<std::shared_ptr<Components::Light>> getLights(std::shared_ptr<Components::Transform> const& objectTransform) const;
	};

}

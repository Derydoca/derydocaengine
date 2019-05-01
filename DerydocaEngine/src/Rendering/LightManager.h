#pragma once
#include <glm/vec3.hpp>
#include <list>
#include <memory>
#include "Scenes\Scene.h"

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

	class LightManager
	{
	public:
		static LightManager& getInstance()
		{
			static LightManager instance;
			return instance;
		}

		void addLight(std::weak_ptr<Components::Light> const& light) { m_lights.push_back(light); }
		void bindLightsToShader(
			std::shared_ptr<Components::Transform> const& objectTransform,
			std::shared_ptr<Rendering::Shader> const& shader
		);
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

		void operator=(LightManager const&) = delete;
	private:
		const int MAX_LIGHTS = 10;

		std::list<std::weak_ptr<Components::Light>> m_lights;
		unsigned int m_shadowJitterTexture;
		glm::vec3 m_shadowJitterTextureSize;

		LightManager();
		LightManager(LightManager const&);
		~LightManager();

		void buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV);
		std::list<std::shared_ptr<Components::Light>> getLights(std::shared_ptr<Components::Transform> const& objectTransform) const;
	};

}

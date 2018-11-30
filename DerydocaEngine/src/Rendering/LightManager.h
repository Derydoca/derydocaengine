#pragma once
#include <glm/vec3.hpp>
#include <list>
#include <memory>

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

		void addLight(Components::Light* const& light) { m_lights.push_back(light); }
		void bindLightsToShader(
			std::shared_ptr<Rendering::MatrixStack> const& matrixStack,
			std::shared_ptr<Components::Transform> const& objectTransform,
			std::shared_ptr<Rendering::Shader> const& shader
		);
		void removeLight(Components::Light* const& light) { m_lights.remove(light); }
		void renderShadowMaps(std::shared_ptr<Components::Transform> const& objectTransform);

		void operator=(LightManager const&) = delete;
	private:
		const int MAX_LIGHTS = 10;

		std::list<Components::Light*> m_lights;
		unsigned int m_shadowJitterTexture;
		glm::vec3 m_shadowJitterTextureSize;

		LightManager();
		LightManager(LightManager const&);
		~LightManager();

		void buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV);
		std::list<Components::Light*> getLights(std::shared_ptr<Components::Transform> const& objectTransform) const;
	};

}

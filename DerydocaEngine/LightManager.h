#pragma once
#include <list>
#include "Light.h"
#include "Transform.h"
#include "Shader.h"

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

		void addLight(DerydocaEngine::Components::Light* const& light) { m_lights.push_back(light); }
		void removeLight(DerydocaEngine::Components::Light* const& light) { m_lights.remove(light); }

		void bindLightsToShader(Rendering::MatrixStack* const& matrixStack, Components::Transform* const& objectTransform, Rendering::Shader* const& shader);
		void renderShadowMaps(Components::Transform* const& objectTransform);

		void operator=(LightManager const&) = delete;
	private:
		LightManager();
		LightManager(LightManager const&);
		~LightManager();

		void buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV);
		std::list<DerydocaEngine::Components::Light*> getLights(Components::Transform* const& objectTransform);

		const int MAX_LIGHTS = 10;

		std::list<DerydocaEngine::Components::Light*> m_lights;
		GLuint m_shadowJitterTexture;
		glm::vec3 m_shadowJitterTextureSize;
	};

}

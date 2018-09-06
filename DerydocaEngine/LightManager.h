#pragma once
#include <list>
#include "Light.h"
#include "Transform.h"
#include "Shader.h"

class LightManager
{
public:
	static LightManager& getInstance()
	{
		static LightManager instance;
		return instance;
	}

	void addLight(Light* const& light) { m_lights.push_back(light); }
	void removeLight(Light* const& light) { m_lights.remove(light); }

	void bindLightsToShader(MatrixStack* const& matrixStack, Transform* const& objectTransform, Shader* const& shader);
	void renderShadowMaps(Transform* const& objectTransform);

	void operator=(LightManager const&) = delete;
private:
	LightManager();
	LightManager(LightManager const&);
	~LightManager();

	void buildOffsetTex(int const& texSize, int const& samplesU, int const& samplesV);
	std::list<Light*> getLights(Transform* const& objectTransform);

	const int MAX_LIGHTS = 10;

	std::list<Light*> m_lights;
	GLuint m_shadowJitterTexture;
	glm::vec3 m_shadowJitterTextureSize;
};
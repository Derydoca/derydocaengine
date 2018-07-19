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

	void addLight(Light* light) { m_lights.push_back(light); }
	void removeLight(Light* light) { m_lights.remove(light); }

	void bindLightsToShader(MatrixStack* matrixStack, Transform* objectTransform, Shader* shader);
	void renderShadowMaps(Transform* objectTransform);

	void operator=(LightManager const&) = delete;
private:
	LightManager();
	LightManager(LightManager const&);
	~LightManager();

	void buildOffsetTex(int texSize, int samplesU, int samplesV);
	std::list<Light*> getLights(Transform* objectTransform);

	const int MAX_LIGHTS = 10;

	std::list<Light*> m_lights;
	GLuint m_shadowJitterTexture;
	glm::vec3 m_shadowJitterTextureSize;
};
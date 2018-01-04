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

	void bindLightsToShader(Transform* objectTransform, Shader* shader);

	void operator=(LightManager const&) = delete;
private:
	LightManager();
	~LightManager();

	std::list<Light*> getLights(Transform* objectTransform);

	LightManager(LightManager const&);
	std::list<Light*> m_lights;
	const int MAX_LIGHTS = 4;
};


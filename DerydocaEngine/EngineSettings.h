#pragma once
#include "yaml-cpp\yaml.h"
#include <assert.h>
#include "glm/glm.hpp"
#include "YamlTools.h"
#include "boost\uuid\uuid.hpp"

using namespace boost::uuids;

class EngineSettings
{
public:
	EngineSettings();
	~EngineSettings();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	glm::vec3 getCamPos() const { return m_camPos; }
	float getFOV() const { return m_fov; }
	uuid getSkyboxId() { return m_skyboxId; }

	bool isSkyboxDefined() { return m_skyboxId != uuid(); }
private:
	int m_width;
	int m_height;
	glm::vec3 m_camPos;
	float m_fov;
	uuid m_skyboxId;
};


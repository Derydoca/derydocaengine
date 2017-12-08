#pragma once
#include "yaml-cpp\yaml.h"
#include <assert.h>
#include "glm/glm.hpp"
#include "YamlTools.h"

class EngineSettings
{
public:
	EngineSettings();
	~EngineSettings();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	glm::vec3 getCamPos() const { return m_camPos; }
	float getFOV() const { return m_fov; }
private:
	int m_width;
	int m_height;
	glm::vec3 m_camPos;
	float m_fov;
};


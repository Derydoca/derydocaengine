#pragma once
#include "yaml-cpp\yaml.h"
#include <assert.h>
#include "glm/glm.hpp"
#include "YamlTools.h"
#include "boost\uuid\uuid.hpp"
#include <string>

using namespace boost::uuids;
using namespace std;

class EngineSettings
{
public:
	EngineSettings(string configFilePath);
	~EngineSettings();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	glm::vec3 getCamPos() const { return m_camPos; }
	float getFOV() const { return m_fov; }
	uuid getSkyboxId() { return m_skyboxId; }
	string getEngineResourceDirectory() const { return m_engineResourceDirectory; }
	string getProjectDirectory() const { return m_projectDirectory; }

	bool isSkyboxDefined() { return m_isSkyboxDefined; }
private:
	int m_width;
	int m_height;
	glm::vec3 m_camPos;
	float m_fov;
	uuid m_skyboxId;
	bool m_isSkyboxDefined = false;
	string m_engineResourceDirectory;
	string m_projectDirectory;
};


#pragma once
#include "yaml-cpp\yaml.h"
#include <assert.h>
#include <boost\filesystem.hpp>
#include <boost\uuid\uuid.hpp>
#include <string>
#include "glm/glm.hpp"
#include "YamlTools.h"
#include "Camera.h"

class EngineSettings
{
public:
	EngineSettings(std::string const& configFilePath);
	~EngineSettings();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	glm::vec3 getCamPos() const { return m_camPos; }
	float getFOV() const { return m_fov; }
	boost::uuids::uuid getSkyboxId() { return m_skyboxId; }
	std::string getEngineResourceDirectory() const { return m_engineResourceDirectory; }
	DerydocaEngine::Components::Camera::RenderingMode getCamRenderMode() { return m_renderingMode; }

	bool isSkyboxDefined() { return m_isSkyboxDefined; }
private:
	int m_width;
	int m_height;
	glm::vec3 m_camPos;
	float m_fov;
	DerydocaEngine::Components::Camera::RenderingMode m_renderingMode;
	boost::uuids::uuid m_skyboxId;
	bool m_isSkyboxDefined = false;
	std::string m_engineResourceDirectory;
	boost::filesystem::path m_settingsFilePath;
};


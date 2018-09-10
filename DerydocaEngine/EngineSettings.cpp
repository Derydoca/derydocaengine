#include "EngineSettings.h"
#include <iostream>

EngineSettings::EngineSettings(std::string const& configFilePath)
{
	m_settingsFilePath = boost::filesystem::absolute(configFilePath);

	// Load the configuration file
	YAML::Node root = YAML::LoadFile(m_settingsFilePath.string());

	YAML::Node engineNode = root["Engine"];
	if (engineNode)
	{
		m_engineResourceDirectory = engineNode["Resources"].as<std::string>();
	}
	 
	YAML::Node windowNode = root["Window"];
	if (windowNode)
	{
		m_width = YamlTools::getIntSafe(windowNode, "Width", 800);
		m_height = YamlTools::getIntSafe(windowNode, "Height", 600);
	}
	else
	{
		m_width = 800;
		m_height = 600;
	}

	YAML::Node cameraNode = root["Camera"];
	if (cameraNode)
	{
		m_camPos = cameraNode["Position"].as<glm::vec3>();
		m_fov = YamlTools::getFloatSafe(cameraNode, "FOV", 70.0f);
		
		YAML::Node skyboxIdNode = cameraNode["Skybox"];
		if (skyboxIdNode)
		{
			m_skyboxId = skyboxIdNode.as<boost::uuids::uuid>();
			m_isSkyboxDefined = true;
		}

		YAML::Node renderingModeNode = cameraNode["RenderMode"];
		if (renderingModeNode)
		{
			int rm = renderingModeNode.as<int>();
			m_renderingMode = static_cast<Camera::RenderingMode>(rm);
		}
	}
	else
	{
		m_camPos = glm::vec3(0, 0, 0);
		m_fov = 70.0f;
	}
}

EngineSettings::~EngineSettings()
{
}

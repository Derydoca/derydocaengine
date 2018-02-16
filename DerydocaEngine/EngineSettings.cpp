#include "EngineSettings.h"

EngineSettings::EngineSettings(string configFilePath)
{
	// Load the configuration file
	YAML::Node root = YAML::LoadFile(configFilePath);

	YAML::Node engineNode = root["Engine"];
	if (engineNode)
	{
		m_engineResourceDirectory = engineNode["Resources"].as<string>();
		m_projectDirectory = engineNode["Project"].as<string>();
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
			m_skyboxId = skyboxIdNode.as<uuid>();
			m_isSkyboxDefined = true;
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

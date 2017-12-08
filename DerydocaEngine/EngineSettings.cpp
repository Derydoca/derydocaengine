#include "EngineSettings.h"


EngineSettings::EngineSettings()
{
	YAML::Node config = YAML::LoadFile("../res/engineconfig.yaml");

	YAML::Node window = config["window"];

	m_width = YamlTools::getInt(window, "width", 800);
	m_height = YamlTools::getInt(window, "height", 600);

	YAML::Node camera = config["camera"];

	//m_camPos = YamlTools::getVec3(camera, "position", glm::vec3());
	m_camPos = camera["position"].as<glm::vec3>();
	m_fov = YamlTools::getFloat(camera, "fov", 70.0f);
}

EngineSettings::~EngineSettings()
{
}

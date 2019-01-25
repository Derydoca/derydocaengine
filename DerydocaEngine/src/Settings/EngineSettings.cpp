#include "EnginePch.h"
#include "Settings\EngineSettings.h"

namespace DerydocaEngine::Settings
{

	EngineSettings::EngineSettings(const std::string& configFilePath) :
		m_settingsFilePath(),
		m_width(800),
		m_height(600),
		m_engineResourceDirectory(),
		m_editorComponentsSceneIdentifier()
	{
		m_settingsFilePath = boost::filesystem::absolute(configFilePath);

		// Load the configuration file
		YAML::Node root = YAML::LoadFile(m_settingsFilePath.string());

		YAML::Node engineNode = root["Engine"];
		if (engineNode)
		{
			m_engineResourceDirectory = engineNode["Resources"].as<std::string>();

			YAML::Node editorComponentsSceneNode = engineNode["EditorComponentsScene"];
			if (editorComponentsSceneNode)
			{
				m_editorComponentsSceneIdentifier = editorComponentsSceneNode.as<std::string>();
			}

			YAML::Node editorGuiSceneNode = engineNode["EditorGuiScene"];
			if (editorGuiSceneNode)
			{
				m_editorGuiSceneIdentifier = editorGuiSceneNode.as<std::string>();
			}
		}

		YAML::Node windowNode = root["Window"];
		if (windowNode)
		{
			m_width = YamlTools::getIntSafe(windowNode, "Width", 800);
			m_height = YamlTools::getIntSafe(windowNode, "Height", 600);
		}

	}

	EngineSettings::~EngineSettings()
	{
	}

}
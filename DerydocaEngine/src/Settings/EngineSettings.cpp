#include "EnginePch.h"
#include "Settings\EngineSettings.h"
#include <cereal/types/memory.hpp>

namespace DerydocaEngine::Settings
{
	std::shared_ptr<EngineSettings> EngineSettings::s_Instance = std::make_shared<EngineSettings>();
	const char* s_FilePath = ".\\EngineSettings.json";

	EngineSettings::EngineSettings() :
		m_size(800, 600),
		m_fullScreen(false),
		m_maximized(false),
		m_engineResourceDirectory(),
		m_editorComponentsSceneIdentifier(),
		m_editorGuiSceneIdentifier(),
		m_editorSkyboxMaterialIdentifier()
	{
	}

	EngineSettings::~EngineSettings()
	{
	}

	bool EngineSettings::Initialize()
	{
		EngineSettings::s_Instance = std::make_shared<EngineSettings>();

		std::ifstream fs(s_FilePath);
		{
			cereal::JSONInputArchive iarchive(fs);
			iarchive(SERIALIZE_NAMED("m_engineSettings", EngineSettings::s_Instance));
		}
		fs.close();
		return true;
	}

	void EngineSettings::Save()
	{
		std::ofstream fs(s_FilePath);
		{
			cereal::JSONOutputArchive oarchive(fs);
			oarchive(SERIALIZE_NAMED("m_engineSettings", EngineSettings::s_Instance));
		}
		fs.close();
	}
}
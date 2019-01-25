#pragma once
#include "yaml-cpp\yaml.h"
#include <assert.h>
#include <boost\filesystem.hpp>
#include <boost\uuid\uuid.hpp>
#include <string>
#include "glm/glm.hpp"
#include "Helpers\YamlTools.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Settings
{

	class EngineSettings
	{
	public:
		EngineSettings(const std::string& configFilePath);
		~EngineSettings();

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		std::string getEngineResourceDirectory() const { return m_engineResourceDirectory; }
		std::string getEditorComponentsSceneIdentifier() const { return m_editorComponentsSceneIdentifier; }
		std::string getEditorWindowsSceneIdentifier() const { return m_editorWindowsSceneIdentifier; }
	private:
		boost::filesystem::path m_settingsFilePath;
		int m_width;
		int m_height;
		std::string m_engineResourceDirectory;
		std::string m_editorComponentsSceneIdentifier;
		std::string m_editorWindowsSceneIdentifier;
	};

}

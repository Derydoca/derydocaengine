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
		EngineSettings();
		~EngineSettings();
		static bool Initialize();
		static void Save();
		static std::shared_ptr<EngineSettings> getInstance() { return EngineSettings::s_Instance; }
		int2 getSize() const { return m_size; }
		bool shouldBeFullScreen() const { return m_fullScreen; }
		bool shouldBeMaximized() const { return m_maximized; }
		std::string getEngineResourceDirectory() const { return m_engineResourceDirectory; }
		std::string getEditorComponentsSceneIdentifier() const { return m_editorComponentsSceneIdentifier; }
		std::string getEditorGuiSceneIdentifier() const { return m_editorGuiSceneIdentifier; }
		std::string getEditorSkyboxMaterialIdentifier() const { return m_editorSkyboxMaterialIdentifier; }

		void setWindowProperties(const int2 size, const bool fullScreen, const bool maximized)
		{
			m_size = size;
			m_fullScreen = fullScreen;
			m_maximized = maximized;
		}

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				SERIALIZE(m_size),
				SERIALIZE(m_fullScreen),
				SERIALIZE(m_maximized),
				SERIALIZE(m_engineResourceDirectory),
				SERIALIZE(m_editorComponentsSceneIdentifier),
				SERIALIZE(m_editorGuiSceneIdentifier),
				SERIALIZE(m_editorSkyboxMaterialIdentifier)
			);
		}
		static std::shared_ptr<EngineSettings> s_Instance;
	private:

	private:

		int2 m_size;
		bool m_fullScreen;
		bool m_maximized;
		std::string m_engineResourceDirectory;
		std::string m_editorComponentsSceneIdentifier;
		std::string m_editorGuiSceneIdentifier;
		std::string m_editorSkyboxMaterialIdentifier;
	};

}

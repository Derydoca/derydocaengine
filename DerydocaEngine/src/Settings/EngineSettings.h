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
		std::string getEngineResourceDirectory() const { return m_engineResourceDirectory; }
		std::string getEditorComponentsSceneIdentifier() const { return m_editorComponentsSceneIdentifier; }
		std::string getEditorGuiSceneIdentifier() const { return m_editorGuiSceneIdentifier; }
		std::string getEditorSkyboxMaterialIdentifier() const { return m_editorSkyboxMaterialIdentifier; }
		
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				SERIALIZE(m_engineResourceDirectory),
				SERIALIZE(m_editorComponentsSceneIdentifier),
				SERIALIZE(m_editorGuiSceneIdentifier),
				SERIALIZE(m_editorSkyboxMaterialIdentifier)
			);
		}
		static std::shared_ptr<EngineSettings> s_Instance;
	private:

	private:

		std::string m_engineResourceDirectory;
		std::string m_editorComponentsSceneIdentifier;
		std::string m_editorGuiSceneIdentifier;
		std::string m_editorSkyboxMaterialIdentifier;
	};

}

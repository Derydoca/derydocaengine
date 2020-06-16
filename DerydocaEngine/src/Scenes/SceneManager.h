#pragma once
#include "Scene.h"
#include "Resources\LevelResource.h"

namespace DerydocaEngine::Scenes
{

	class SceneManager
	{
	public:
		static SceneManager& getInstance()
		{
			static SceneManager instance;
			return instance;
		}

		std::shared_ptr<Scene> getActiveScene() { return m_ActiveScene; }

		void LoadScene(const boost::uuids::uuid & levelId);
		void LoadScene(const std::shared_ptr<Resources::LevelResource> levelResource);
		void SaveScene(const std::string& outputPath);
		void UnloadScene();

	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager&);

		std::shared_ptr<Scene> m_ActiveScene;
		std::shared_ptr<Resources::LevelResource> m_ActiveLevelResource;
	};

}
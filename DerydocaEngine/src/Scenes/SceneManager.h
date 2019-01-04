#pragma once
#include "Scene.h"
#include "Resources\Resource.h"

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

		std::shared_ptr<Scene> getActiveScene() { return m_activeScene; }

		void loadScene(const std::shared_ptr<Resources::Resource> levelResource);
		void unloadScene();

	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager&);

		std::shared_ptr<Scene> m_activeScene;
	};

}
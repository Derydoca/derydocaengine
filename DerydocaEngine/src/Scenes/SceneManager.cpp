#include "EnginePch.h"
#include "SceneManager.h"
#include "SerializedScene.h"

namespace DerydocaEngine::Scenes
{

	SceneManager::SceneManager() :
		m_activeScene()
	{
	}

	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::loadScene(const std::shared_ptr<Resources::Resource> levelResource)
	{
		unloadScene();

		auto scene = std::make_shared<Scenes::SerializedScene>();
		scene->LoadFromFile(levelResource->getSourceFilePath());
		scene->setUp();
		m_activeScene = scene;
	}

	void SceneManager::unloadScene()
	{
		if (m_activeScene == nullptr)
		{
			return;
		}

		m_activeScene->tearDown();
		m_activeScene = nullptr;
	}

}
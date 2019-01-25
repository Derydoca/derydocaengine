#include "EnginePch.h"
#include "SceneManager.h"
#include "SerializedScene.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Scenes
{

	SceneManager::SceneManager() :
		m_activeScene()
	{
	}

	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::loadScene(const boost::uuids::uuid& levelId)
	{
		auto resource = ObjectLibrary::getInstance().getResource<Resources::LevelResource>(levelId);
		
		if (resource == nullptr)
		{
			std::cout << "Unable to find a scene with ID of '" << levelId << "'!\n";
			return;
		}

		loadScene(resource);
	}

	void SceneManager::loadScene(const std::shared_ptr<Resources::LevelResource> levelResource)
	{
		unloadScene();

		auto scene = std::make_shared<Scenes::SerializedScene>();
		scene->LoadFromFile(levelResource->getSourceFilePath());
		scene->setUp();
		scene->getRoot()->init();
		scene->getRoot()->postInit();
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
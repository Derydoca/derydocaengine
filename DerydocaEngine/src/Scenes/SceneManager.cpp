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
			D_LOG_ERROR("Unable to find a scene with ID of '{}.", boost::uuids::to_string(levelId));
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

	void SceneManager::saveScene(const std::string& outputPath)
	{
		auto ss = std::static_pointer_cast<Scenes::SerializedScene>(m_activeScene);
		if (ss == NULL)
		{
			return;
		}
		ss->SaveToFile(outputPath);
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
#include "EnginePch.h"
#include "SceneManager.h"
#include "SerializedScene.h"
#include "ObjectLibrary.h"
#include "Files\FileUtils.h"
#include "GameObject.h"

namespace DerydocaEngine::Scenes
{

	SceneManager::SceneManager() :
		m_ActiveScene()
	{
	}

	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::LoadScene(const boost::uuids::uuid& levelId)
	{
		auto resource = ObjectLibrary::getInstance().getResource<Resources::LevelResource>(levelId);
		
		if (resource == nullptr)
		{
			D_LOG_ERROR("Unable to find a scene with ID of '{}.", boost::uuids::to_string(levelId));
			return;
		}

		LoadScene(resource);
	}

	void SceneManager::LoadScene(const std::shared_ptr<Resources::LevelResource> levelResource)
	{
		UnloadScene();

		m_ActiveLevelResource = levelResource;

		std::string path = m_ActiveLevelResource->getSourceFilePath();
		auto sceneObj = Files::Utils::ReadFromDisk<Scenes::SerializedScene>(path);
		auto scene = std::make_shared<Scenes::SerializedScene>(sceneObj);
		scene->setUp();
		scene->getRoot()->init();
		scene->getRoot()->postInit();
		m_ActiveScene = scene;
	}

	void SceneManager::SaveScene(const std::string& outputPath)
	{
		auto ss = std::static_pointer_cast<Scenes::SerializedScene>(m_ActiveScene);
		if (ss == NULL)
		{
			return;
		}
		Files::Utils::WriteToDisk(*ss, outputPath);
	}

	void SceneManager::UnloadScene()
	{
		if (m_ActiveScene == nullptr)
		{
			return;
		}

		m_ActiveScene->tearDown();
		m_ActiveScene = nullptr;
	}

}
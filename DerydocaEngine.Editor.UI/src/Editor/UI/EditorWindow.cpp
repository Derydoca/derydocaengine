#include "EditorPch.h"
#include "Editor\UI\EditorWindow.h"

#include <boost\filesystem.hpp>
#include <iostream>
#include <sdl2\SDL.h>

#include "Input\InputManager.h"
#include "ObjectLibrary.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Display.h"
#include "Scenes\SerializedScene.h"
#include "Settings\EngineSettings.h"
#include "Timing\Clock.h"

namespace DerydocaEngine::Editor::UI
{

	std::shared_ptr<Resources::Resource> getLevelResource(const std::string& levelIdentifier, const std::string& levelType)
	{
		// Load the editor components
		if (levelIdentifier.empty())
		{
			std::cout << "No editor components level identifier was provided!\n";
			return nullptr;
		}
		auto resource = ObjectLibrary::getInstance().getResource(levelIdentifier);
		if (resource == nullptr)
		{
			std::cout << "Unable to find the " << levelType << " level with ID of '" << levelIdentifier << "'!\n";
			return nullptr;
		}

		return resource;
	}

	std::shared_ptr<Scenes::SerializedScene> SetUpLevelObject(
		const std::shared_ptr<Resources::Resource> levelResource,
		const std::string& levelType
	)
	{
		auto scene = std::make_shared<Scenes::SerializedScene>();
		scene->LoadFromFile(levelResource->getSourceFilePath());
		scene->setUp();

		return scene;
	}

	bool getLastModifiedTime(const std::string& filePath, std::time_t& time)
	{
		if (!boost::filesystem::exists(filePath))
		{
			return false;
		}

		time = boost::filesystem::last_write_time(filePath);
		return true;
	}

	int EditorWindow::Run(const std::string& projectPath, const std::string& levelIdentifier)
	{
		auto settings = std::make_unique<Settings::EngineSettings>(".\\engineSettings.yaml");

		// Load the project file
		ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), projectPath);

		// Initialize the clock to this machine
		Timing::Clock::init();
		auto clock = std::make_unique<Timing::Clock>();

		auto display = std::make_shared<Rendering::Display>(settings->getWidth(), settings->getHeight(), "Derydoca Engine");

		// Divisor defines minimum frames per second
		unsigned long minFrameTime = 1000 / 60;

		auto editorSceneResource = getLevelResource(settings->getEditorComponentsSceneIdentifier(), "editor");
		auto editorScene = SetUpLevelObject(editorSceneResource, "editor");

		auto levelResource = getLevelResource(levelIdentifier, "scene");
		auto scene = SetUpLevelObject(levelResource, "main");
		getLastModifiedTime(levelResource->getSourceFilePath(), m_levelLoadTime);

		// Initialize all components in the scene before rendering anything
		editorScene->getRoot()->init();
		scene->getRoot()->init();

		// Run the post initialization routine on all components
		editorScene->getRoot()->postInit();
		scene->getRoot()->postInit();

		std::vector<std::shared_ptr<Scenes::Scene>> scenes =
		{
			editorScene,
			scene
		};

		std::time_t levelLastModifiedTime;

		// Render loop
		while (!display->isClosed())
		{

			// Tick the clock forward the number of ms it took since the last frame rendered
			editorScene->getRoot()->update(clock->getDeltaTime());
			scene->getRoot()->update(clock->getDeltaTime());

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(scenes);

			// Let the scene objects do whatever it is they need to do after rendering has completed this frame
			editorScene->getRoot()->postRender();
			scene->getRoot()->postRender();

			// Let the display respond to any input events
			display->update();

			// Update the mouse inputs
			Input::InputManager::getInstance().getMouse()->update();

			// Pause the code execution if we are running faster than our capped frame rate
			unsigned long msToWait = (unsigned long)(minFrameTime - clock->getRenderTimeMS());
			if (msToWait > 0)
			{
				if (msToWait > minFrameTime)
				{
					msToWait = minFrameTime;
				}
				SDL_Delay(msToWait);
			}
			clock->update();

			// If the file has been updated since we last loaded it
			if (getLastModifiedTime(levelResource->getSourceFilePath(), levelLastModifiedTime) &&
				levelLastModifiedTime > m_levelLoadTime)
			{
				// Store the time that we reloaded the level
				m_levelLoadTime = levelLastModifiedTime;

				// Rebuild the scene components
				scene->tearDown();
				scene = SetUpLevelObject(levelResource, "main");
				scenes =
				{
					editorScene,
					scene
				};

				// Initialize the new scene
				scene->getRoot()->init();
				scene->getRoot()->postInit();
			}
		}

		// Clean up the scene
		editorScene->tearDown();
		scene->tearDown();

		return 0;
	}

}

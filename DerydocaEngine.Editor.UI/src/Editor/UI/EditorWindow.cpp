#include "EditorPch.h"
#include "Editor\UI\EditorWindow.h"

#include <boost\filesystem.hpp>
#include <iostream>
#include <sdl2\SDL.h>
#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "Timing\Clock.h"
#include "Resources\CubemapResource.h"
#include "Rendering\Display.h"
#include "Settings\EngineSettings.h"
#include "Input\InputManager.h"
#include "Rendering\Material.h"
#include "ObjectLibrary.h"
#include "Scenes\SerializedScene.h"
#include "Components\ScreenshotUtil.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Skybox.h"
#include "Components\Transform.h"
#include "Components\WasdMover.h"

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
		const std::string& levelType,
		const std::shared_ptr<GameObject> rootSceneObject
	)
	{
		auto scene = std::make_shared<Scenes::SerializedScene>();
		scene->LoadFromFile(levelResource->getSourceFilePath());
		scene->setUp(rootSceneObject);

		return scene;
	}

	bool getLastModifiedTime(std::string const& filePath, std::time_t &time)
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

		auto editorSceneRoot = std::make_shared<GameObject>("__EDITOR_SCENE_ROOT__");
		auto editorSceneResource = getLevelResource(settings->getEditorComponentsSceneIdentifier(), "editor");
		std::shared_ptr<Scenes::SerializedScene> editorScene = SetUpLevelObject(editorSceneResource, "editor", editorSceneRoot);

		std::shared_ptr<GameObject> sceneRoot = std::make_shared<GameObject>("__SCENE_ROOT__");
		auto levelResource = getLevelResource(levelIdentifier, "editor");
		std::shared_ptr<Scenes::Scene> scene = SetUpLevelObject(levelResource, "main", sceneRoot);

		// Initialize all components in the scene before rendering anything
		editorSceneRoot->init();
		sceneRoot->init();

		// Run the post initialization routine on all components
		editorSceneRoot->postInit();
		sceneRoot->postInit();

		std::vector<std::shared_ptr<GameObject>> roots = {
			editorSceneRoot,
			sceneRoot
		};

		std::time_t levelLastModifiedTime;

		// Render loop
		while (!display->isClosed()) {

			// Tick the clock forward the number of ms it took since the last frame rendered
			editorSceneRoot->update(clock->getDeltaTime());
			sceneRoot->update(clock->getDeltaTime());

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(roots);

			// Let the scene objects do whatever it is they need to do after rendering has completed this frame
			editorSceneRoot->postRender();
			sceneRoot->postRender();

			// Let the display respond to any input events
			display->update();

			// Update the mouse inputs
			Input::InputManager::getInstance().getMouse()->update();

			// Pause the code execution if we are running faster than our capped frame rate
			unsigned long msToWait = (unsigned long)(minFrameTime - clock->getRenderTimeMS());
			if (msToWait > 0) {
				if (msToWait > minFrameTime) {
					msToWait = minFrameTime;
				}
				SDL_Delay(msToWait);
			}
			clock->update();

			//// If the file has been updated since we last loaded it
			//if (getLastModifiedTime(levelResource->getSourceFilePath(), levelLastModifiedTime) &&
			//	levelLastModifiedTime > m_levelLoadTime)
			//{
			//	// Store the time that we reloaded the level
			//	m_levelLoadTime = levelLastModifiedTime;

			//	// Rebuild the scene components
			//	
			//	sceneRoot = std::make_shared<GameObject>("__SCENE_ROOT_UPDATED__");
			//	scene->tearDown(sceneRoot);
			//	scene = SetUpLevelObject(editorSceneResource, "editor", editorSceneRoot);

			//	// Initialize the new scene
			//	sceneRoot->init();
			//	sceneRoot->postInit();
			//}
		}

		// Clean up the scene
		editorScene->tearDown(editorSceneRoot);
		scene->tearDown(sceneRoot);

		return 0;
	}

}

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

	std::shared_ptr<Scenes::SerializedScene> EditorWindow::SetUpLevelObject(const std::string& levelIdentifier, const std::string& levelType, const std::shared_ptr<GameObject> rootSceneObject)
	{
		// Load the editor components
		if (!levelIdentifier.empty())
		{
			std::cout << "No editor components level identifier was provided!\n";
			return nullptr;
		}
		auto editorComponentsLevelResource = ObjectLibrary::getInstance().getResource(levelIdentifier);
		if (editorComponentsLevelResource == nullptr)
		{
			std::cout << "Unable to find the " << levelType << " level with ID of '" << levelIdentifier << "'!\n";
			return nullptr;
		}

		auto scene = std::make_shared<Scenes::SerializedScene>();
		scene->LoadFromFile(editorComponentsLevelResource->getSourceFilePath());
		scene->setUp(rootSceneObject);

		return scene;
	}

	int EditorWindow::Run(const std::string& projectPath, const std::string& editorComponentsLevelIdentifier, const std::string& levelIdentifier)
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
		auto editorScene = SetUpLevelObject(editorComponentsLevelIdentifier, "editor", editorSceneRoot);

		auto sceneRoot = std::make_shared<GameObject>("__SCENE_ROOT__");
		auto scene = SetUpLevelObject(editorComponentsLevelIdentifier, "main", sceneRoot);

		// Initialize all components in the scene before rendering anything
		editorSceneRoot->init();
		sceneRoot->init();

		// Run the post initialization routine on all components
		editorSceneRoot->postInit();
		sceneRoot->postInit();

		// Render loop
		while (!display->isClosed()) {

			// Tick the clock forward the number of ms it took since the last frame rendered
			editorSceneRoot->update(clock->getDeltaTime());
			sceneRoot->update(clock->getDeltaTime());

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(editorSceneRoot);
			Rendering::CameraManager::getInstance().render(sceneRoot);

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
		}

		// Clean up the scene
		editorScene->tearDown(editorSceneRoot);
		scene->tearDown(sceneRoot);

		return 0;
	}

}

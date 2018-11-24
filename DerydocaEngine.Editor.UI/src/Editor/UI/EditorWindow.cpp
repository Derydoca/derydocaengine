#include "Editor\UI\EditorWindow.h"

#include <boost\filesystem.hpp>
#include <iostream>
#include <sdl2\SDL.h>
#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "Clock.h"
#include "Resources\CubemapResource.h"
#include "Rendering\Display.h"
#include "EngineSettings.h"
#include "GameObject.h"
#include "Input\InputManager.h"
#include "Rendering\Material.h"
#include "ObjectLibrary.h"
#include "SerializedScene.h"
#include "Components\ScreenshotUtil.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Skybox.h"
#include "Components\Transform.h"
#include "Components\WasdMover.h"

namespace DerydocaEngine::Editor::UI
{

	int EditorWindow::Run(std::string const& projectPath, std::string const& levelIdentifier)
	{
		Settings::EngineSettings* settings = new Settings::EngineSettings(".\\engineSettings.yaml");

		// Load the project file
		ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), projectPath);

		// Initialize the clock to this machine
		Timing::Clock::init();
		Timing::Clock* clock = new Timing::Clock();

		Rendering::Display* display = new Rendering::Display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");

		GameObject* sceneRoot = new GameObject();

#pragma region Editor specific game objects

		// Keep this here as a simple way to grab screenshots of the engine
		Components::ScreenshotUtil* screenshotUtil = new Components::ScreenshotUtil(display, Input::InputManager::getInstance().getKeyboard());
		sceneRoot->addComponent(screenshotUtil);

		// This is the editor camera
		GameObject* editorCameraObject = new GameObject("__editorCamera");
		Components::Transform* editorCameraTransform = editorCameraObject->getTransform();
		editorCameraTransform->setPos(settings->getCamPos());
		Components::Camera* editorCamera = new Components::Camera(settings->getFOV(), display->getAspectRatio(), 0.01f, 1000.0f);
		editorCamera->setDisplay(display);
		editorCamera->setRenderingMode(settings->getCamRenderMode());
		if (settings->isSkyboxDefined())
		{
			// If a skybox is defined, build the skybox material and assign it to the editor camera
			Rendering::Shader* skyboxShader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\cubemapShader");
			Resources::CubemapResource* cubemapResource = (Resources::CubemapResource*)ObjectLibrary::getInstance().getResource(settings->getSkyboxId());
			Rendering::Texture* skyboxTexture = (Rendering::Texture*)cubemapResource->getResourceObject();
			Rendering::Material* skyboxMaterial = new Rendering::Material();
			skyboxMaterial->setShader(skyboxShader);
			skyboxMaterial->setTextureSlot(0, skyboxTexture);
			editorCamera->setSkybox(skyboxMaterial);
			editorCamera->setClearMode(Components::Camera::ClearMode::SkyboxClear);
		}
		else
		{
			// By default, clear the screen with a deep red
			editorCamera->setClearMode(Components::Camera::ClearMode::ColorClear);
			editorCamera->setClearColor(Color(0.5, 0, 0));
		}
		editorCameraObject->addComponent(editorCamera);
		editorCameraObject->addComponent(new Components::WasdMover(Input::InputManager::getInstance().getKeyboard(), Input::InputManager::getInstance().getMouse()));
		sceneRoot->addChild(editorCameraObject);

#pragma endregion

		// If a scene was provided, load it
		Scenes::SerializedScene* scene = nullptr;
		if (!levelIdentifier.empty())
		{
			Resources::Resource* levelResource = ObjectLibrary::getInstance().getResource(levelIdentifier);

			if (levelResource == nullptr)
			{
				std::cout << "No level resource with the ID of '" << levelIdentifier << "' could be found.\n";
			}
			else
			{
				scene = new Scenes::SerializedScene();
				scene->LoadFromFile(levelResource->getSourceFilePath());
				scene->setUp(sceneRoot);
			}
		}

		// Divisor defines minimum frames per second
		unsigned long minFrameTime = 1000 / 60;

		// Initialize all components in the scene before rendering anything
		sceneRoot->init();

		// Run the post initialization routine on all components
		sceneRoot->postInit();

		// Render loop
		while (!display->isClosed()) {

			// Tick the clock forward the number of ms it took since the last frame rendered
			sceneRoot->update(clock->getDeltaTime());

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(sceneRoot);

			// Let the scene objects do whatever it is they need to do after rendering has completed this frame
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
		if (scene != nullptr)
		{
			scene->tearDown(sceneRoot);
		}

		// Clean up all other objects
		delete sceneRoot;
		delete display;
		delete settings;
		delete clock;
		delete scene;

		return 0;
	}

}

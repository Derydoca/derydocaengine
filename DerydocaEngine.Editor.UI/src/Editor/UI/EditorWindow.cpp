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

	int EditorWindow::Run(std::string const& projectPath, std::string const& levelIdentifier)
	{
		Settings::EngineSettings* settings = new Settings::EngineSettings(".\\engineSettings.yaml");

		// Load the project file
		ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), projectPath);

		// Initialize the clock to this machine
		Timing::Clock::init();
		Timing::Clock* clock = new Timing::Clock();

		Rendering::Display* display = new Rendering::Display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");

		m_sceneRoot = std::make_shared<GameObject>("__SCENE_ROOT__");

#pragma region Editor specific game objects

		// Keep this here as a simple way to grab screenshots of the engine
		auto screenshotUtil = std::make_shared<Components::ScreenshotUtil>(display, Input::InputManager::getInstance().getKeyboard());
		m_sceneRoot->addComponent(screenshotUtil);

		// This is the editor camera
		std::shared_ptr<GameObject> editorCameraObject = std::make_shared<GameObject>("__editorCamera");
		std::shared_ptr<Components::Transform> editorCameraTransform = editorCameraObject->getTransform();
		editorCameraTransform->setPos(settings->getCamPos());
		auto editorCamera = std::make_shared<Components::Camera>(settings->getFOV(), display->getAspectRatio(), 0.01f, 1000.0f);
		editorCamera->setDisplay(display);
		editorCamera->setRenderingMode(settings->getCamRenderMode());
		if (settings->isSkyboxDefined())
		{
			// If a skybox is defined, build the skybox material and assign it to the editor camera
			auto skyboxShader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\cubemapShader");
			auto cubemapResource = std::static_pointer_cast<Resources::CubemapResource>(ObjectLibrary::getInstance().getResource(settings->getSkyboxId()));
			auto skyboxTexture = std::static_pointer_cast<Rendering::Texture>(cubemapResource->getResourceObjectPointer());
			auto skyboxMaterial = std::make_shared<Rendering::Material>();
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
		editorCameraObject->addComponent(std::make_shared<Components::WasdMover>(Input::InputManager::getInstance().getKeyboard(), Input::InputManager::getInstance().getMouse()));
		m_sceneRoot->addChild(editorCameraObject);

#pragma endregion

		// If a scene was provided, load it
		Scenes::SerializedScene* scene = nullptr;
		if (!levelIdentifier.empty())
		{
			auto levelResource = ObjectLibrary::getInstance().getResource(levelIdentifier);

			if (levelResource == nullptr)
			{
				std::cout << "No level resource with the ID of '" << levelIdentifier << "' could be found.\n";
			}
			else
			{
				scene = new Scenes::SerializedScene();
				scene->LoadFromFile(levelResource->getSourceFilePath());
				scene->setUp(m_sceneRoot);
			}
		}

		// Divisor defines minimum frames per second
		unsigned long minFrameTime = 1000 / 60;

		// Initialize all components in the scene before rendering anything
		m_sceneRoot->init();

		// Run the post initialization routine on all components
		m_sceneRoot->postInit();

		// Render loop
		while (!display->isClosed()) {

			// Tick the clock forward the number of ms it took since the last frame rendered
			m_sceneRoot->update(clock->getDeltaTime());

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(m_sceneRoot);

			// Let the scene objects do whatever it is they need to do after rendering has completed this frame
			m_sceneRoot->postRender();

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
			scene->tearDown(m_sceneRoot);
		}

		// Clean up all other objects
		delete display;
		delete settings;
		delete clock;
		delete scene;

		return 0;
	}

}

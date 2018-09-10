#include "Editor.h"

#include "Display.h"
#include "Clock.h"
#include "EngineSettings.h"
#include "GameObject.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "Camera.h"
#include "WasdMover.h"
#include "SerializedScene.h"
#include "ObjectLibrary.h"
#include "ScreenshotUtil.h"
#include "CubemapResource.h"
#include "Skybox.h"
#include "ShaderLibrary.h"
#include <iostream>
#include <boost\filesystem.hpp>

int Editor::Run(std::string const& projectPath, std::string const& levelIdentifier)
{
	EngineSettings* settings = new EngineSettings(".\\engineSettings.yaml"); 
	
	// Load the project file
	ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), projectPath);

	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	Display* display = new Display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");

	GameObject* sceneRoot = new GameObject();

#pragma region Editor specific game objects

	// Keep this here as a simple way to grab screenshots of the engine
	ScreenshotUtil* screenshotUtil = new ScreenshotUtil(display, InputManager::getInstance().getKeyboard());
	sceneRoot->addComponent(screenshotUtil);

	// This is the editor camera
	GameObject* editorCameraObject = new GameObject("__editorCamera");
	Transform* editorCameraTransform = editorCameraObject->getTransform();
	editorCameraTransform->setPos(settings->getCamPos());
	Camera* editorCamera = new Camera(settings->getFOV(), display->getAspectRatio(), 0.01f, 1000.0f);
	editorCamera->setDisplay(display);
	editorCamera->setRenderingMode(settings->getCamRenderMode());
	if (settings->isSkyboxDefined())
	{
		// If a skybox is defined, build the skybox material and assign it to the editor camera
		Shader* skyboxShader = ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\cubemapShader");
		CubemapResource* cubemapResource = (CubemapResource*)ObjectLibrary::getInstance().getResource(settings->getSkyboxId());
		Texture* skyboxTexture = (Texture*)cubemapResource->getResourceObject();
		Material* skyboxMaterial = new Material();
		skyboxMaterial->setShader(skyboxShader);
		skyboxMaterial->setTextureSlot(0, skyboxTexture);
		editorCamera->setSkybox(skyboxMaterial);
		editorCamera->setClearMode(Camera::ClearMode::SkyboxClear);
	}
	else
	{
		// By default, clear the screen with a deep red
		editorCamera->setClearMode(Camera::ClearMode::ColorClear);
		editorCamera->setClearColor(Color(0.5, 0, 0));
	}
	editorCameraObject->addComponent(editorCamera);
	editorCameraObject->addComponent(new WasdMover(InputManager::getInstance().getKeyboard(), InputManager::getInstance().getMouse()));
	sceneRoot->addChild(editorCameraObject);

#pragma endregion

	// If a scene was provided, load it
	SerializedScene* scene = nullptr;
	if (!levelIdentifier.empty())
	{
		Resource* levelResource = ObjectLibrary::getInstance().getResource(levelIdentifier);

		if (levelResource == nullptr)
		{
			std::cout << "No level resource with the ID of '" << levelIdentifier << "' could be found.\n";
		}
		else
		{
			scene = new SerializedScene();
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
		CameraManager::getInstance().render(sceneRoot);

		// Let the scene objects do whatever it is they need to do after rendering has completed this frame
		sceneRoot->postRender();

		// Let the display respond to any input events
		display->update();

		// Update the mouse inputs
		InputManager::getInstance().getMouse()->update();

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

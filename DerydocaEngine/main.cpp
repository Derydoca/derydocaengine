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

int main()
{
	// Load the project file
	ObjectLibrary::getInstance().initialize("../proj/");

	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	EngineSettings* settings = new EngineSettings();

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
	editorCamera->setClearMode(Camera::ClearMode::ColorClear);
	editorCamera->setClearColor(Color(0.5, 0, 0));
	editorCameraObject->addComponent(editorCamera);
	editorCameraObject->addComponent(new WasdMover(InputManager::getInstance().getKeyboard(), InputManager::getInstance().getMouse()));
	sceneRoot->addChild(editorCameraObject);

#pragma endregion

	// Load a scene from a file
	SerializedScene* scene = new SerializedScene();
	scene->LoadFromFile("../proj/threeSquirrels.derylevel");
	scene->setUp(sceneRoot);

	// Divisor defines minimum frames per second
	unsigned long minFrameTime = 1000 / 60;

	// Initialize all components in the scene before rendering anything
	sceneRoot->init();

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
	scene->tearDown(sceneRoot);

	// Clean up all other objects
	delete(sceneRoot);
	delete(display);
	delete(settings);
	delete(clock);

	return 0;
}
#include "Display.h"
#include "Clock.h"
#include "Keyboard.h"
#include "EngineSettings.h"
#include "GameObject.h"
#include "CameraManager.h"
#include "Oculus.h"
#include "ShaderManager.h"

#if _DEBUG
// Debug only headers
#include "DebugVisualizer.h"
#endif

// TODO: remove these header files after scene loading is implemented
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Texture.h"
#include "WasdMover.h"
#include "Skybox.h"
#include "ScreenshotUtil.h"
#include "Terrain.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Rotator.h"
#include "ButtonState.h"
#include "RenderTexture.h"
#include "Light.h"
#include "KeyboardMover.h"

#include "SquirrelTerrainScene.h"
#include "DiffuseTestScene.h"
#include "SerializedScene.h"

int main()
{
	ButtonState* bs = new ButtonState(0);

	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	EngineSettings* settings = new EngineSettings();

	Keyboard* keyboard = new Keyboard();
	keyboard->init();

	Mouse* mouse = new Mouse();

	Display* display = new Display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");
	display->setKeyboard(keyboard);

	Mesh* sphereMesh = new Mesh("../res/plane.obj");

	Material* mat = new Material();
	{
		Shader* shader = new Shader("../res/basicShader");
		mat->setShader(shader);
		Texture* grassTexture = new Texture("../res/grass.png");
		mat->setTextureSlot(0, grassTexture);
	}

	//---

	Shader shader("../res/basicShader");

	GameObject* goRoot = new GameObject();
	ScreenshotUtil* screenshotUtil = new ScreenshotUtil(display, keyboard);
	goRoot->addComponent(screenshotUtil);

#if _DEBUG
	DebugVisualizer dVis;
	goRoot->addComponent(&dVis);
#endif

	Scene* sceneBase = new DiffuseTestScene();
	sceneBase->setUp(goRoot, settings, display, keyboard, mouse);

	SerializedScene* scene = new SerializedScene();
	scene->LoadFromFile("../res/generatedLevel.yaml");
	//scene->SaveToFile("../res/generatedLevel.yaml");
	scene->setUp(goRoot, settings, display, keyboard, mouse);

	// Divisor defines minimum frames per second
	unsigned long minFrameTime = 1000 / 60;

	goRoot->init();

	while (!display->isClosed()) {

		goRoot->update(clock->getDeltaTime());
		CameraManager::getInstance().render(goRoot);
		goRoot->postRender();

		display->update();
		mouse->update();

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

	return 0;
}
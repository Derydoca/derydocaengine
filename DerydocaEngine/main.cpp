#include "Display.h"
#include "Clock.h"
#include "Keyboard.h"
#include "EngineSettings.h"
#include "GameObject.h"
#include "CameraManager.h"

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

	Display display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");
	display.setKeyboard(keyboard);

	//---

	Shader shader("../res/basicShader");

	GameObject* goRoot = new GameObject();
	ScreenshotUtil* screenshotUtil = new ScreenshotUtil(&display, keyboard);
	goRoot->addComponent(screenshotUtil);

#if _DEBUG
	DebugVisualizer dVis;
	goRoot->addComponent(&dVis);
#endif

	GameObject* goCamera = new GameObject();
	Shader skyShader("../res/cubemapShader");
	Texture* sky = new Texture("../res/cubemap-xpos.png", "../res/cubemap-xneg.png", "../res/cubemap-ypos.png", "../res/cubemap-yneg.png", "../res/cubemap-zpos.png", "../res/cubemap-zneg.png");
	Material* skyMaterial = new Material();
	skyMaterial->setShader(&skyShader);
	skyMaterial->setTextureSlot(0, sky);
	Camera camera(settings->getFOV(), display.getAspectRatio(), 0.01f, 1000.0f);
	camera.setSkybox(skyMaterial);
	camera.setClearMode(Camera::ClearMode::SkyboxClear);
	WasdMover mover(keyboard, mouse);
	goCamera->addComponent(&camera);
	goCamera->addComponent(&mover);
	goRoot->addChild(goCamera);

	Texture grassTexture("../res/grass.png");
	Terrain* terrain = new Terrain("../res/heightmap2.png", 0.2f, 15.0f);
	terrain->setTextureSlot(0, &grassTexture);
	GameObject* goTerrain = new GameObject();
	goTerrain->getTransform()->setPos(glm::vec3(-50.0f, -3.0f, -50.0f));
	goTerrain->addComponent(terrain);
	goRoot->addChild(goTerrain);

	Texture texture("../res/rebel.jpg");
	Material* matSquirrel = new Material();
	matSquirrel->setShader(&shader);
	matSquirrel->setTextureSlot(0, &texture);

	Mesh* squirrel = new Mesh("../res/rebel.obj");
	Rotator* rotSquirrel = new Rotator(1);
	MeshRenderer* mrSquirrel = new MeshRenderer(squirrel, matSquirrel);
	GameObject* goSquirrel = new GameObject();
	goSquirrel->addComponent(rotSquirrel);
	goSquirrel->addComponent(mrSquirrel);
	goSquirrel->getTransform()->setPos(glm::vec3(0, 0, 0));
	goRoot->addChild(goSquirrel);

	MeshRenderer* mrSquirrel2 = new MeshRenderer(squirrel, matSquirrel);
	Rotator* rotSquirrel2 = new Rotator(2);
	GameObject* goSquirrel2 = new GameObject();
	goSquirrel2->addComponent(rotSquirrel2);
	goSquirrel2->addComponent(mrSquirrel2);
	goSquirrel2->getTransform()->setPos(glm::vec3(1, 0, 0));
	goSquirrel->addChild(goSquirrel2);

	MeshRenderer* mrSquirrel3 = new MeshRenderer(squirrel, matSquirrel);
	Rotator* rotSquirrel3 = new Rotator(3);
	GameObject* goSquirrel3 = new GameObject();
	goSquirrel3->addComponent(rotSquirrel3);
	goSquirrel3->addComponent(mrSquirrel3);
	goSquirrel3->getTransform()->setPos(glm::vec3(0.5f, 0, 0));
	goSquirrel2->addChild(goSquirrel3);

	// Divisor defines minimum frames per second
	unsigned long minFrameTime = 1000 / 60;

	goRoot->init();

	while (!display.isClosed()) {

		goRoot->update(clock->getDeltaTime());
		CameraManager::getInstance().render(goRoot);
		goRoot->postRender();

		display.update();
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
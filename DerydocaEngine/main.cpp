#include <iostream>
#include <GL/glew.h>
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Clock.h"
#include "ButtonState.h"
#include "DebugVisualizer.h"
#include "Keyboard.h"
#include "WasdMover.h"
#include "CubeMap.h"
#include "Skybox.h"
#include "ScreenshotUtil.h"
#include "Terrain.h"
#include "EngineSettings.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "MatrixStack.h"
#include "Rotator.h"

int main()
{
	EngineSettings* settings = new EngineSettings();

	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	Keyboard* keyboard = new Keyboard();
	keyboard->init();

	Mouse* mouse = new Mouse();

	Display display(settings->getWidth(), settings->getHeight(), "Derydoca Engine");
	display.setKeyboard(keyboard);

	Shader skyShader("../res/cubemapShader");
	Texture texture("../res/rebel.jpg");

	Transform cameraTransform(settings->getCamPos());
	Camera camera(&cameraTransform, settings->getFOV(), display.getAspectRatio(), 0.01f, 1000.0f);
	WasdMover mover(&cameraTransform, keyboard, mouse);

	CubeMap sky = CubeMap("../res/cubemap-xpos.png", "../res/cubemap-xneg.png", "../res/cubemap-ypos.png", "../res/cubemap-yneg.png", "../res/cubemap-zpos.png", "../res/cubemap-zneg.png");
	Skybox* skybox = new Skybox();

	ScreenshotUtil* screenshotUtil = new ScreenshotUtil(&display, keyboard);
	
	DebugVisualizer dVis;

	glm::vec3 camPos = cameraTransform.getPos();

	//---

	Shader shader("../res/basicShader");

	GameObject* goRoot = new GameObject();
	goRoot->addComponent(screenshotUtil);

	Texture grassTexture("../res/grass.png");
	Material* matTerrain = new Material();
	matTerrain->setShader(&shader);
	matTerrain->setTextureSlot(0, &texture);

	Terrain* terrain = new Terrain("../res/heightmap2.png", 0.2f, 15.0f);
	terrain->setTextureSlot(0, &grassTexture);
	GameObject* goTerrain = new GameObject();
	goTerrain->getTransform()->setPos(glm::vec3(-50.0f, -3.0f, -50.0f));
	goTerrain->addComponent(terrain);
	goRoot->addChild(goTerrain);

	MatrixStack* matStack = new MatrixStack();

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
		// Clear the display buffer
		display.clear(0.0f, 0.0f, 1.0f, 1.0f);

		// Simple camera movement
		mover.update(clock->getDeltaTime());

		goRoot->update(clock->getDeltaTime());
		goRoot->render(&camera, matStack);
		goRoot->postRender();

		skyShader.bind();
		sky.bind(0);
		skyShader.update(camera.getRotationProjection());
		skybox->getMesh()->draw();

		dVis.draw(camera.getViewProjection());

		display.update();
		mouse->update();

		// Pause the code execution if we are running faster than our capped frame rate
		unsigned long msToWait = (unsigned long)(minFrameTime - (clock->getRenderTime() * 1000.0f));
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
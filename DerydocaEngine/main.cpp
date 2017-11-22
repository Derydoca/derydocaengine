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

int main()
{
	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	Keyboard* keyboard = new Keyboard();
	keyboard->init();

	Display display(800, 600, "Derydoca Engine");
	display.setKeyboard(keyboard);

	Mesh mesh2("../res/rebel.obj");
	Shader shader("../res/basicShader");
	Texture texture("../res/rebel.jpg");
	Transform cameraTransform(glm::vec3(0, 0, -30));
	Camera camera(&cameraTransform, 70.0f, display.getAspectRatio(), 0.01f, 1000.0f);
	Transform transform;
	Transform transform1;

	DebugVisualizer dVis;

	glm::vec3 camPos = cameraTransform.getPos();

	while (!display.isClosed()) {
		// Clear the display buffer
		display.clear(0.0f, 0.0f, 0.0f, 1.0f);

		// Simple camera movement
		float moveSpeed = 10.0f * clock->getDeltaTime();
		if (keyboard->isKeyDown(SDLK_w)) {
			camPos.z += moveSpeed;
		}
		if (keyboard->isKeyDown(SDLK_a)) {
			camPos.x += moveSpeed;
		}
		if (keyboard->isKeyDown(SDLK_s)) {
			camPos.z -= moveSpeed;
		}
		if (keyboard->isKeyDown(SDLK_d)) {
			camPos.x -= moveSpeed;
		}
		if (keyboard->isKeyDown(SDLK_q)) {
			camPos.y += moveSpeed;
		}
		if (keyboard->isKeyDown(SDLK_e)) {
			camPos.y -= moveSpeed;
		}
		cameraTransform.setPos(camPos);

		// Move one of the models around
		float sinCounter = sinf(clock->getTime());
		float cosCounter = cosf(clock->getTime());
		transform1.getPos().x = -cosCounter;
		transform1.getPos().y = -cosCounter;
		transform1.setEulerAngles(glm::vec3(sinCounter, sinCounter, sinCounter));

		shader.bind();
		texture.bind(0);

		shader.update(transform, camera);
		mesh2.draw();

		shader.update(transform1, camera);
		mesh2.draw();

		dVis.draw();

		display.update();

		clock->update();
	}

	return 0;
}
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

int main()
{
	// Initialize the clock to this machine
	Clock::init();
	Clock* clock = new Clock();

	Keyboard* keyboard = new Keyboard();
	keyboard->init();

	Mouse* mouse = new Mouse();

	Display display(1920, 1080, "Derydoca Engine");
	display.setKeyboard(keyboard);

	Mesh mesh2("../res/rebel.obj");
	Shader shader("../res/basicShader");
	Texture texture("../res/rebel.jpg");
	Transform cameraTransform(glm::vec3(0, 0, -10));
	Camera camera(&cameraTransform, 70.0f, display.getAspectRatio(), 0.01f, 1000.0f);
	WasdMover mover(&cameraTransform, keyboard, mouse);
	Transform transform;
	Transform transform1;

	DebugVisualizer dVis;

	glm::vec3 camPos = cameraTransform.getPos();

	while (!display.isClosed()) {
		// Clear the display buffer
		display.clear(0.0f, 0.0f, 0.0f, 1.0f);

		// Simple camera movement
		mover.update(clock->getDeltaTime());

		// Move one of the models around
		float sinCounter = sinf(clock->getTime());
		float cosCounter = cosf(clock->getTime());
		transform1.getPos().x = -cosCounter;
		transform1.getPos().y = -cosCounter;
		transform1.setEulerAngles(glm::vec3(clock->getTime(), clock->getTime(), 0));

		shader.bind();
		texture.bind(0);

		shader.update(transform, camera);
		mesh2.draw();

		shader.update(transform1, camera);
		mesh2.draw();

		dVis.draw();

		display.update();
		mouse->update();

		clock->update();
	}

	return 0;
}
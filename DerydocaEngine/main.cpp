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

int main()
{
	// Initialize the clock to this machine
	Clock::init();
	Clock clock();

	//ButtonState keyboard(SDL_GetKeyboardState());
	Display display(800, 600, "Derydoca Engine");

	Mesh mesh2("../res/rebel.obj");
	Shader shader("../res/basicShader");
	Texture texture("../res/rebel.jpg");
	Transform cameraTransform(glm::vec3(0, 0, -30));
	Camera camera(cameraTransform, 70.0f, display.getAspectRatio(), 0.01f, 1000.0f);
	Transform transform;
	Transform transform1;

	unsigned long startTime, lastFrameTime, currentTime, deltaTime;
	startTime = lastFrameTime = currentTime = SDL_GetTicks();

	float counter = 0.0f;

	DebugVisualizer dVis;
	
	while (!display.isClosed()) {

		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastFrameTime;

		display.clear(0.0f, 0.0f, 0.0f, 1.0f);

		float sinCounter = sinf(currentTime * 0.001f);
		float cosCounter = cosf(currentTime * 0.001f);

		// Test the FOV updating
		//camera.setFov((sinCounter * 1.0f) + 70.0f);

		transform.getPos().x = sinCounter;
		transform.getPos().z = sinCounter ;
		transform.setEulerAngles(glm::vec3(cosCounter, cosCounter, cosCounter));

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
		counter += 0.001f;

	}

	return 0;
}
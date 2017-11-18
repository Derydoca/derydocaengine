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
	Camera camera(glm::vec3(0, 0, -3), 70.0f, display.GetAspectRatio(), 0.01f, 1000.0f);
	Transform transform;
	Transform transform1;

	unsigned long startTime, lastFrameTime, currentTime, deltaTime;
	startTime = lastFrameTime = currentTime = SDL_GetTicks();

	float counter = 0.0f;

	while (!display.IsClosed()) {

		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastFrameTime;

		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);

		float sinCounter = sinf(currentTime * 0.001f);
		float cosCounter = cosf(currentTime * 0.001f);

		transform.GetPos().x = sinCounter;
		transform.GetPos().z = sinCounter ;
		transform.GetRot().x = currentTime * 0.001f;
		transform.GetRot().y = currentTime * 0.001f;
		transform.GetRot().z = currentTime * 0.001f;

		transform1.GetPos().x = -cosCounter;
		transform1.GetPos().y = -cosCounter;
		transform1.GetRot().x = currentTime * -0.001f;
		transform1.GetRot().y = currentTime * -0.001f;
		transform1.GetRot().z = currentTime * -0.001f;

		shader.Bind();
		texture.Bind(0);

		shader.Update(transform, camera);
		mesh2.Draw();

		shader.Update(transform1, camera);
		mesh2.Draw();

		display.Update();
		counter += 0.001f;

	}

	return 0;
}
#include <iostream>
#include <GL/glew.h>
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

int main()
{
	Display display(800, 600, "Derydoca Engine");

	// Define test triangle mesh
	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0,0.0)),
						  Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5,1.0)),
						  Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0,0.0)) };
	unsigned int indices[] = { 0, 1, 2 };
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

	Mesh mesh2("../res/monkey3.obj");
	Shader shader("../res/basicShader");
	Texture texture("../res/bricks.jpg");
	Camera camera(glm::vec3(0, 0, -3), 70.0f, display.GetAspectRatio(), 0.01f, 1000.0f);
	Transform transform;

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
		transform.GetPos().z = sinCounter;
		transform.GetRot().x = currentTime * 0.001f;
		transform.GetRot().y = currentTime * 0.001f;
		transform.GetRot().z = currentTime * 0.001f;

		shader.Bind();
		texture.Bind(0);
		shader.Update(transform, camera);

		mesh2.Draw();

		display.Update();
		counter += 0.001f;
/*
		SDL_Event event;

		int quit = 0;
		while (!quit) {
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					display.Close();
					quit = 1;
					break;
				default:
					break;
				}
			}
		}*/
	}

	return 0;
}
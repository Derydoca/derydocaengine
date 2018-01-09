#include "DiffuseTestScene.h"

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
#include "ShaderManager.h"
#include "MaterialRefresher.h"



DiffuseTestScene::DiffuseTestScene()
{
}


DiffuseTestScene::~DiffuseTestScene()
{
}

void DiffuseTestScene::setUp(GameObject * root, EngineSettings * settings, Display * display, Keyboard * keyboard, Mouse * mouse)
{
	// Camera with Cubemap
	GameObject* goCamera = new GameObject();
	Shader* skyShader = new Shader("../res/cubemapShader");
	Texture* sky = new Texture("../res/cubemap-xpos.png", "../res/cubemap-xneg.png", "../res/cubemap-ypos.png", "../res/cubemap-yneg.png", "../res/cubemap-zpos.png", "../res/cubemap-zneg.png");
	Material* skyMaterial = new Material();
	skyMaterial->setShader(skyShader);
	skyMaterial->setTextureSlot(0, sky);
	Camera* camera = new Camera(settings->getFOV(), display->getAspectRatio(), 0.01f, 1000.0f);
	camera->setDisplay(display);
	camera->setSkybox(skyMaterial);
	camera->setClearMode(Camera::ClearMode::SkyboxClear);
	WasdMover* mover = new WasdMover(keyboard, mouse);
	goCamera->addComponent(camera);
	goCamera->addComponent(mover);
	root->addChild(goCamera);

	// Grass material
	Material* grassMaterial = new Material();
	Shader* shader = new Shader("../res/basicShader");
	grassMaterial->setShader(shader);
	Texture* grassTexture = new Texture("../res/grass.png");
	grassMaterial->setTextureSlot(0, grassTexture);

	// Point light with small grassy sphere representation
	Mesh* sphere = new Mesh("../res/sphere.obj");
	Light* pointLight = new Light();
	pointLight->setColor(new Color(0, 1, 0));
	GameObject* goPointLight = new GameObject("Point Light");
	goPointLight->addComponent(pointLight);
	KeyboardMover* lightMover = new KeyboardMover(keyboard, 5.0f, SDLK_t, SDLK_g, SDLK_f, SDLK_h, SDLK_r, SDLK_y);
	goPointLight->addComponent(lightMover);
	MeshRenderer* mrSphere = new MeshRenderer(sphere, grassMaterial);
	goPointLight->addComponent(mrSphere);
	goPointLight->getTransform()->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
	root->addChild(goPointLight);

	// Diffuse material
	std::string diffuseShaderPath = "../res/diffuse2";
	Material* diffuseMaterial = new Material();
	Shader* diffuseShader = new Shader(diffuseShaderPath);
	diffuseMaterial->setShader(diffuseShader);

	// Manual mesh
	Vertex* verts = new Vertex[4]();
	verts[0] = Vertex(glm::vec3(0, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1));
	verts[1] = Vertex(glm::vec3(0, 1, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1));
	verts[2] = Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1));
	verts[3] = Vertex(glm::vec3(1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1));
	unsigned int ind[6] = {
		0, 2, 1, 0, 3, 2
	};
	Mesh* m = new Mesh(verts, (unsigned int)4, ind, (unsigned int)6);

	// Test mesh with diffuse material applied
	Mesh* testMesh = new Mesh("../res/sphere.obj");
	//MeshRenderer* mrSquirrel = new MeshRenderer(squirrel, matSquirrel);
	MeshRenderer* mrDiffuseTest = new MeshRenderer(testMesh, diffuseMaterial);
	MaterialRefresher* refresher = new MaterialRefresher(keyboard, mrDiffuseTest);
	GameObject* goDiffuseTest = new GameObject("Diffuse Test Mesh");
	goDiffuseTest->addComponent(mrDiffuseTest);
	goDiffuseTest->addComponent(refresher);
	goDiffuseTest->getTransform()->setPos(glm::vec3(0, 0, -0.5));
	goDiffuseTest->getTransform()->setEulerAngles(glm::vec3(0, 0, 90));
	root->addChild(goDiffuseTest);
}

void DiffuseTestScene::tearDown(GameObject * root)
{
}

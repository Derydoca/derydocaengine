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



DiffuseTestScene::DiffuseTestScene()
{
}


DiffuseTestScene::~DiffuseTestScene()
{
}

void DiffuseTestScene::setUp(GameObject * root, EngineSettings * settings, Display * display, Keyboard * keyboard, Mouse * mouse)
{
	Material* mat = new Material();
	Shader* shader = new Shader("../res/basicShader");
	mat->setShader(shader);
	Texture* grassTexture = new Texture("../res/grass.png");
	mat->setTextureSlot(0, grassTexture);


	GameObject* goCamera = new GameObject();
	Shader skyShader("../res/cubemapShader");
	Texture* sky = new Texture("../res/cubemap-xpos.png", "../res/cubemap-xneg.png", "../res/cubemap-ypos.png", "../res/cubemap-yneg.png", "../res/cubemap-zpos.png", "../res/cubemap-zneg.png");
	Material* skyMaterial = new Material();
	skyMaterial->setShader(&skyShader);
	skyMaterial->setTextureSlot(0, sky);
	Camera camera(settings->getFOV(), display->getAspectRatio(), 0.01f, 1000.0f);
	camera.setDisplay(display);
	camera.setSkybox(skyMaterial);
	camera.setClearMode(Camera::ClearMode::SkyboxClear);
	WasdMover mover(keyboard, mouse);
	goCamera->addComponent(&camera);
	goCamera->addComponent(&mover);
	root->addChild(goCamera);

	Mesh* sphere = new Mesh("../res/sphere.obj");
	Light* pointLight = new Light();
	pointLight->setColor(new Color(0, 1, 0));
	GameObject* goPointLight = new GameObject("Point Light");
	goPointLight->addComponent(pointLight);
	KeyboardMover* lightMover = new KeyboardMover(keyboard, 5.0f, SDLK_t, SDLK_g, SDLK_f, SDLK_h, SDLK_r, SDLK_y);
	goPointLight->addComponent(lightMover);
	MeshRenderer* mrSphere = new MeshRenderer(sphere, mat);
	goPointLight->addComponent(mrSphere);
	goPointLight->getTransform()->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
	root->addChild(goPointLight);

	Material* diffuseMaterial = new Material();
	Shader* diffuseShader = new Shader("../res/diffuseWorldPos");
	diffuseMaterial->setShader(diffuseShader);
	diffuseMaterial->setColorRGB("Kd", Color(1, 0, 0));
	diffuseMaterial->setColorRGB("Ld", Color(1, 1, 1));

	Mesh* squirrel = new Mesh("../res/sphere.obj");
	Rotator* rotSquirrel = new Rotator(0);
	//MeshRenderer* mrSquirrel = new MeshRenderer(squirrel, matSquirrel);
	MeshRenderer* mrSquirrel = new MeshRenderer(squirrel, diffuseMaterial);
	GameObject* goSquirrel = new GameObject("Squirrel 1");
	goSquirrel->addComponent(rotSquirrel);
	goSquirrel->addComponent(mrSquirrel);
	goSquirrel->getTransform()->setPos(glm::vec3(0, 0, -5));
	root->addChild(goSquirrel);
}

void DiffuseTestScene::tearDown(GameObject * root)
{
}

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
	Shader* shader = new Shader("../res/diffuseFrag");
	grassMaterial->setShader(shader);
	Texture* grassTexture = new Texture("../res/grass.png");
	grassMaterial->setTextureSlot(0, grassTexture);

	// Point light with small grassy sphere representation
	Mesh* sphere = new Mesh("../res/sphere.obj");
	Light* pointLight = new Light();
	pointLight->setColor(new Color(0, 1, 0.5));
	GameObject* goPointLight = new GameObject("Point Light");
	goPointLight->addComponent(pointLight);
	KeyboardMover* lightMover = new KeyboardMover(keyboard, 5.0f, SDLK_t, SDLK_g, SDLK_f, SDLK_h, SDLK_r, SDLK_y);
	goPointLight->addComponent(lightMover);
	MeshRenderer* mrSphere = new MeshRenderer(sphere, grassMaterial);
	goPointLight->addComponent(mrSphere);
	goPointLight->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	root->addChild(goPointLight);

	// Point light with small grassy sphere representation
	Light* pointLight2 = new Light();
	pointLight2->setColor(new Color(1, 0.5, 0));
	GameObject* goPointLight2 = new GameObject("Point Light 2");
	goPointLight2->addComponent(pointLight2);
	KeyboardMover* lightMover2 = new KeyboardMover(keyboard, 5.0f, SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_u, SDLK_o);
	goPointLight2->addComponent(lightMover2);
	MeshRenderer* mrSphere2 = new MeshRenderer(sphere, grassMaterial);
	goPointLight2->addComponent(mrSphere2);
	goPointLight2->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	root->addChild(goPointLight2);

	// Point light with small grassy sphere representation
	Light* pointLight3 = new Light();
	pointLight3->setColor(new Color(0.5, 0, 1));
	GameObject* goPointLight3 = new GameObject("Point Light 3");
	goPointLight3->addComponent(pointLight3);
	KeyboardMover* lightMover3 = new KeyboardMover(keyboard, 5.0f, SDLK_x, SDLK_z, SDLK_c, SDLK_v, SDLK_b, SDLK_n);
	goPointLight3->addComponent(lightMover3);
	MeshRenderer* mrSphere3 = new MeshRenderer(sphere, grassMaterial);
	goPointLight3->addComponent(mrSphere3);
	goPointLight3->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	root->addChild(goPointLight3);

	// Directional light
	Light* directionalLight = new Light();
	directionalLight->setLightType(Light::Directional);
	directionalLight->setColor(new Color(0.3, 0.3, 0.3));
	GameObject* goDirectionalLight = new GameObject("Directional Light");
	goDirectionalLight->addComponent(directionalLight);
	goDirectionalLight->getTransform()->setEulerAngles(glm::vec3(-30, -30, 0));
	root->addChild(goDirectionalLight);

	// Diffuse material
	std::string diffuseShaderPath = "../res/diffuseFrag";
	Material* diffuseMaterial = new Material();
	Shader* diffuseShader = new Shader(diffuseShaderPath);
	diffuseMaterial->setShader(diffuseShader);
	Texture* rebelMaterial = new Texture("../res/rebel.jpg");
	diffuseMaterial->setTextureSlot(0, rebelMaterial);

	// Test mesh with diffuse material applied
	Mesh* testMesh = new Mesh("../res/sphere.obj");
	MeshRenderer* mrDiffuseTest = new MeshRenderer(testMesh, diffuseMaterial);
	MaterialRefresher* refresher = new MaterialRefresher(mrDiffuseTest);
	GameObject* goDiffuseTest = new GameObject("Diffuse Test Mesh");
	goDiffuseTest->addComponent(mrDiffuseTest);
	goDiffuseTest->addComponent(refresher);
	goDiffuseTest->getTransform()->setPos(glm::vec3(0, 0, -0.5));
	goDiffuseTest->getTransform()->setEulerAngles(glm::vec3(0, 0, 90));
	root->addChild(goDiffuseTest);


	MeshRenderer* mrDiffuseTest2 = new MeshRenderer(testMesh, diffuseMaterial);
	MaterialRefresher* refresher2 = new MaterialRefresher(mrDiffuseTest2);
	GameObject* goDiffuseTest2 = new GameObject("Diffuse Test Mesh");
	goDiffuseTest2->addComponent(mrDiffuseTest2);
	goDiffuseTest2->addComponent(refresher2);
	goDiffuseTest2->getTransform()->setPos(glm::vec3(2, 0, -0.5));
	goDiffuseTest2->getTransform()->setEulerAngles(glm::vec3(0, 0, 90));
	root->addChild(goDiffuseTest2);

	MeshRenderer* mrDiffuseTest3 = new MeshRenderer(testMesh, diffuseMaterial);
	MaterialRefresher* refresher3 = new MaterialRefresher(mrDiffuseTest3);
	GameObject* goDiffuseTest3 = new GameObject("Diffuse Test Mesh");
	goDiffuseTest3->addComponent(mrDiffuseTest3);
	goDiffuseTest3->addComponent(refresher3);
	goDiffuseTest3->getTransform()->setPos(glm::vec3(1, 1.75, -0.5));
	goDiffuseTest3->getTransform()->setEulerAngles(glm::vec3(0, 0, 90));
	root->addChild(goDiffuseTest3);
}

void DiffuseTestScene::tearDown(GameObject * root)
{
}

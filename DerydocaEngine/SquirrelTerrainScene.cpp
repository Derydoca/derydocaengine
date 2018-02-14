#include "SquirrelTerrainScene.h"
#include "GameObject.h"

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
#include "DisplayManager.h"
#include "InputManager.h"

#include "ShaderManager.h"


SquirrelTerrainScene::SquirrelTerrainScene()
{
}


SquirrelTerrainScene::~SquirrelTerrainScene()
{
}

void SquirrelTerrainScene::setUp(GameObject* root)
{
	Material* mat = new Material();
	Shader* shader = new Shader("../res/basicShader");
	mat->setShader(shader);
	Texture* grassTexture = new Texture("../res/grass.png");
	mat->setTextureSlot(0, grassTexture);

	//GameObject* goCamera = new GameObject();
	Shader* skyShader = new Shader("../res/cubemapShader");
	Texture* sky = new Texture("../res/cubemap-xpos.png", "../res/cubemap-xneg.png", "../res/cubemap-ypos.png", "../res/cubemap-yneg.png", "../res/cubemap-zpos.png", "../res/cubemap-zneg.png");
	Material* skyMaterial = new Material();
	skyMaterial->setShader(skyShader);
	skyMaterial->setTextureSlot(0, sky);
	//Camera* camera = new Camera(settings->getFOV(), DisplayManager::getInstance().getDisplay(0)->getAspectRatio(), 0.01f, 1000.0f);
	//camera->setDisplay(DisplayManager::getInstance().getDisplay(0));
	//camera->setSkybox(skyMaterial);
	//camera->setClearMode(Camera::ClearMode::SkyboxClear);
	//WasdMover* mover = new WasdMover(InputManager::getInstance().getKeyboard(), InputManager::getInstance().getMouse());
	//goCamera->addComponent(camera);
	//goCamera->addComponent(mover);
	//root->addChild(goCamera);

	RenderTexture* renderTexture = new RenderTexture(512, 512);
	Camera* renderTextureCam = new Camera(70.0f, renderTexture->getAspectRatio(), 0.01f, 1000.0f);
	renderTextureCam->setDisplay(DisplayManager::getInstance().getDisplay(0));
	renderTextureCam->setSkybox(skyMaterial);
	renderTextureCam->setClearMode(Camera::ClearMode::SkyboxClear);
	renderTextureCam->setRenderTexture(renderTexture);
	renderTextureCam->setDisplayRect(0.5, 0.5, 0.5, 0.5);
	GameObject* goRenderTexture = new GameObject();
	Transform* renderTexCamTrans = goRenderTexture->getTransform();
	renderTexCamTrans->setPos(glm::vec3(0, -1.0f, -5.0f));
	renderTexCamTrans->setEulerAngles(glm::vec3(0.25f, 0.0f, 0.0f));
	goRenderTexture->addComponent(renderTextureCam);
	root->addChild(goRenderTexture);

	Terrain* terrain = new Terrain("../res/heightmap2.png", 0.2f, 15.0f);
	terrain->setTextureSlot(0, grassTexture);
	GameObject* goTerrain = new GameObject();
	goTerrain->getTransform()->setPos(glm::vec3(-50.0f, -3.0f, -50.0f));
	goTerrain->addComponent(terrain);
	root->addChild(goTerrain);

	GameObject* goRenderTextureScreen = new GameObject();
	Transform* rtScreenTrans = goRenderTextureScreen->getTransform();
	rtScreenTrans->setPos(glm::vec3(0.0f, 5.0f, 0.0f));
	rtScreenTrans->setEulerAngles(glm::vec3(90.0f, 0, 0));
	Mesh* plane = new Mesh("../res/plane.obj");
	Material* renderTextureMaterial = new Material();
	renderTextureMaterial->setShader(shader);
	renderTextureMaterial->setTextureSlot(0, renderTexture);
	MeshRenderer* planeRenderer = new MeshRenderer(plane, renderTextureMaterial);
	goRenderTextureScreen->addComponent(planeRenderer);
	root->addChild(goRenderTextureScreen);
	
	Texture texture("../res/rebel.jpg");
	Material* matSquirrel = new Material();
	matSquirrel->setShader(shader);
	matSquirrel->setTextureSlot(0, &texture);

	Mesh* squirrel = new Mesh("../res/rebel.obj");
	Rotator* rotSquirrel = new Rotator(0);
	MeshRenderer* mrSquirrel = new MeshRenderer(squirrel, matSquirrel);
	GameObject* goSquirrel = new GameObject("Squirrel 1");
	goSquirrel->addComponent(rotSquirrel);
	goSquirrel->addComponent(mrSquirrel);
	goSquirrel->getTransform()->setPos(glm::vec3(0, 0, -5));
	root->addChild(goSquirrel);

	MeshRenderer* mrSquirrel2 = new MeshRenderer(squirrel, matSquirrel);
	Rotator* rotSquirrel2 = new Rotator(2);
	GameObject* goSquirrel2 = new GameObject("Squirrel 2");
	goSquirrel2->addComponent(rotSquirrel2);
	goSquirrel2->addComponent(mrSquirrel2);
	goSquirrel2->getTransform()->setPos(glm::vec3(1, 0, 0));
	goSquirrel->addChild(goSquirrel2);

	MeshRenderer* mrSquirrel3 = new MeshRenderer(squirrel, matSquirrel);
	Rotator* rotSquirrel3 = new Rotator(3);
	GameObject* goSquirrel3 = new GameObject("Squirrel 3");
	goSquirrel3->addComponent(rotSquirrel3);
	goSquirrel3->addComponent(mrSquirrel3);
	goSquirrel3->getTransform()->setPos(glm::vec3(0.5f, 0, 0));
	goSquirrel2->addChild(goSquirrel3);
}

void SquirrelTerrainScene::tearDown(GameObject* root)
{
}
